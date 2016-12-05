#include <stdio.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <boost/interprocess/ipc/message_queue.hpp>

#include "Database.h"

int Bullshitterror;
Net * net1;
bool connected2cntrl, connected2pyrat;
int Askpipe, Sendpipe, p_Askpipe, p_Sendpipe;

int main ();
void create_randomNet(std::string cmd);
void save_Net(Net * net);
void load_Net(int i);
void print_Net(Net * n);
void fire_Random(Net * net);
bool isConnectedCtrl();
bool isConnectedPyrat();
int sendFIFO(int pipe,std::string content);
std::string readFIFO(int pipe);
void mainloop();


void mainloop() {
    bool fetch = true;
    remove(CNTRL_SEND_FIFO);
    boost::interprocess::message_queue::remove("ctrl_send");

    std::string cmd;
    std::string old_cmd;


    while(fetch){
        if(isConnectedCtrl()) {                 //both FIFO exist
            //                                      New code boost::interprocess::message_queue
            try {
                boost::interprocess::message_queue mq_ask(boost::interprocess::open_only, "ctrl_ask");
                while(mq_ask.get_num_msg() > 0) {
                    char message[BUFFERSIZE];
                    unsigned int priority;
                    boost::interprocess::message_queue::size_type recvd_size;
                    mq_ask.receive(&message, sizeof(message), recvd_size, priority);
                    printf("Server: %s", message);
                    cmd = message;
                    if(cmd.substr(0, 5) == CNTRL_CMD) {
                        cmd = cmd.substr(6);
                        if(cmd == "CLOSE") {
                            fetch = false;
                        }                        
                        if(cmd == "END") {
                            connected2cntrl = false;
                        }
                        if(cmd.substr(0, 1) == std::to_string(CMD_FIRE)) {
                            size_t pos1 = cmd.find(":", 0) + 1;
                            size_t pos2 = cmd.find(":", pos1);
                            std::string tok = cmd.substr(pos1, pos2 - pos1);
                            int i = stoi(tok);
                            pos1 = pos2 + 1;
                            tok = cmd.substr(pos1);
                            double w = stod(tok);
                            Neuron * tempNeuron = net1->get_Neuron(i);
                            tempNeuron->fire(w);
                        }
                        if(cmd == "GETNETS") {
                            Database db("database.db");
                            std::vector<std::string> nets;
                            db.GetNets(&nets);
                            for(int i = 0; i < nets.size(); ++i) {
                                std::string s;
                                s.append(CNTRL_CMD);
                                s.append(":");
                                s.append(std::to_string(NET));
                                s.append(":");
                                s.append(nets[i]);
                                try {
                                    char message[BUFFERSIZE];
                                    strcpy(message, s.c_str());
                                    boost::interprocess::message_queue mq_send(boost::interprocess::open_only, "ctrl_send");
                                    mq_send.send(&message, sizeof(message), 0);
                                }
                                catch(boost::interprocess::interprocess_exception &ex){
                                    printf("Error while sending nets :%s\n", ex.what());
                                }
                            }
                        }
                        if(cmd.substr(0, 11) == "RETRIEVENET") {
                            size_t pos = cmd.find(":",12);
                            int i = stoi(cmd.substr(12, pos - 12));
                            load_Net(i);
                        }
                        if(cmd.substr(0, 9) == "CREATE_RN") {
                            create_randomNet(cmd);
                        }
                    }
                }
            }
            catch(boost::interprocess::interprocess_exception &ex){
                printf("Error receiving from client: %s\n", ex.what());
            }
            continue;

        }

            //                                          Old cod using FIFO

        if(isConnectedPyrat()) {
            cmd = readFIFO(p_Askpipe).c_str();
            if(cmd == old_cmd) continue;
            old_cmd = cmd;
            printf("%s\n", cmd.c_str());
            if(cmd.length() < 1) continue;
            if(cmd == "Hi") {
                p_Sendpipe = open(PYRAT_SEND_FIFO, O_WRONLY);
                int i = sendFIFO(p_Sendpipe,"Ok");
                i = sendFIFO(p_Sendpipe,"");
            }
            if(cmd.length() < 5) continue;
            if(cmd.substr(0, 5) == CNTRL_CMD) {
                printf("%s\n", cmd.c_str());

                cmd = cmd.substr(6);
                if(cmd == "Close") {
                    fetch = false;
                }
            }
        }
    }
    ::close(Askpipe);
    ::close(Sendpipe);
    ::close(p_Askpipe);
    ::close(p_Sendpipe);
    remove(CNTRL_SEND_FIFO); /* Delete the created fifo */
    remove(PYRAT_SEND_FIFO);
    boost::interprocess::message_queue::remove("ctrl_send");

}

bool isConnectedPyrat() {
    if(connected2pyrat) {
        return true;
    } 
    connected2pyrat = access(PYRAT_ASK_FIFO, F_OK ) != -1 ;//have a look if PYRAT_ASK_FIFO exits
    if(!connected2pyrat) return false;
    connected2pyrat= access(PYRAT_SEND_FIFO, F_OK ) != -1 ;//have a look if PYRAT_SEND_FIFO exits
    if(!connected2pyrat) {
        int pipe = mkfifo (PYRAT_SEND_FIFO, 0666);
        p_Askpipe = open(PYRAT_ASK_FIFO,O_RDONLY|O_NONBLOCK);
        connected2pyrat = pipe > -1;
    }
    return connected2pyrat;
}

bool isConnectedCtrl() {
    if(connected2cntrl) {
        return true;
    } 
    try{
        boost::interprocess::message_queue mq_send(boost::interprocess::open_or_create, "ctrl_send", 100, BUFFERSIZE);
        connected2cntrl = true;
    }
    catch(boost::interprocess::interprocess_exception &ex){
        //std::cout << ex.what() << std::endl;
        connected2cntrl = false;
    }
   if(connected2cntrl) {
       try{
           boost::interprocess::message_queue mq_ask(boost::interprocess::open_only, "ctrl_ask");
       }
       catch(boost::interprocess::interprocess_exception &ex){
            connected2cntrl = false;
       }
    }
    return connected2cntrl;
}

int sendFIFO(int pipe, std::string content) {
    char out[BUFFERSIZE];
    int ret;
    
    ret = sprintf(out,"%s", content.c_str());
    if(pipe <= 0){
        //printf("Could not open fifo");
        return -1;
    } 
    write(pipe, out, strlen(out)+1);
    return 0;
}

std::string readFIFO(int pipe) {
    int fifo;
    char buffer[1024];
    size_t result;

    if(pipe < 0) printf("Fifo open error\n"); 
    result = read(pipe, buffer, 1024); 
    //printf("reading: %s\n", buffer);
    return buffer;
}

void create_randomNet(std::string cmd) {//cmd has format InputNeurons:OutputNeurons:Layercount
    int nextId;
    Database db("database.db");
    db.nextId("nets", &nextId);
    std::size_t pos1 = cmd.find(":",0) + 1;
    cmd = cmd.substr(pos1);
    pos1 = cmd.find(":",0);
    std::string tok = cmd.substr(0, pos1);
    int ip_N = stoi(tok);
    std::size_t pos2 = cmd.find(":", pos1 + 1);
    tok = cmd.substr(pos1 + 1, pos2 - pos1);
    int op_N = stoi(tok);
    int lc = atoi(cmd.substr(pos2 + 1).c_str());
    int io [] = {ip_N, op_N};
    delete(net1);
    net1 = new Net(nextId, lc, io);
    Bullshitterror = net1->Bullshit();
    save_Net(net1);
}
void load_Net(int i) {
    delete(net1);
    net1 = new Net();
    Database db("database.db");
    db.retrieveNet(i, net1);
    net1->publish("ctrl_send");
}

int main () {
    Bullshitterror = 0;
    connected2cntrl = false;
    connected2pyrat = false;
    Database db("database.db");

    srand(time(0));
    int netId;
    
    mainloop();

    delete net1;
    exit(Bullshitterror);

}

void save_Net(Net * net){
    Database db("database.db");
    db.insertNet(net);
}

void print_Net(Net * n){
    /*for(int i = 0; i < n->size; ++i) {
        Layer layer1 = n->layers[i];
        //double r[layer1.size];
        for(int i = 0; i < layer1.size; ++i) {
            printf("\n\nID: %i\nBIAS %f\nTHRESHOLD: %f\n", layer1.neurons[i].id, layer1.neurons[i].bias, layer1.neurons[i].threshold);
        }
        printf("\nLayer Type: %i\nLayer Size: %i", layer1.type, layer1.size);
    }*/
    int conncount = 0;
    for(int la = 0; la < n->size; ++la) {
        Layer * curLayer = &n->layers[la];
        for(int neur = 0; neur < curLayer->size; ++neur) {
            Neuron * curNeuron = &curLayer->neurons[neur];
            printf("\n\nNEURON WITH ID %i IS CONNECTED TO:\n", curNeuron->id);
            for(int conn = 0; conn < curNeuron->connections.size(); ++conn) {
                Neuron::Connection * curConn = &curNeuron->connections[conn];
                printf("Id: %i Type : %i, ", curConn->partner->id, curConn->type);
                ++conncount;
            }
        }
    }

    printf("\n\nTOTAL CONNECTIONS: %i\n\n", conncount);
    std::cin.get();
    std::cin.get();

}

void fire_Random(Net * net){
    for (int InputNeur = 0; InputNeur < net->layers[0].size; ++InputNeur){
        bool res = net->layers[0].neurons[InputNeur].fire(rand() / (double)RAND_MAX);
    }
}
