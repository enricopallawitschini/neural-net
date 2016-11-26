#include <stdio.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include "Database.h"

int Bullshitterror;
Net * net1;
bool connected2cntrl;
int Askpipe, Sendpipe;

int main ();
void create_randomNet(std::string cmd);
void save_Net(Net * net);
void load_Net(int i);
void print_Net(Net * n);
void fire_Random(Net * net);
bool isConnected();
int sendFIFO(int pipe,std::string content);
std::string readFIFO(int pipe);
void mainloop();



void mainloop() {
    bool fetch = true;
    remove(CNTRL_SEND_FIFO);
    std::string cmd;


    while(fetch){
        if(isConnected()) {                 //both FIFO exist
            cmd = readFIFO(Askpipe).c_str();
            if(cmd == "Hi") {
                int i = sendFIFO(Sendpipe,"Hi");
            }
            if(cmd == "OK") {

            }
            if(cmd == "GetNets") {
                Database db("database.db");
                std::vector<std::string> nets;
                db.GetNets(&nets);
                for(int i = 0; nets.size(); ++i) {
                    sendFIFO(Sendpipe, NET + ":" + nets[i]);
                }
            }
            if(cmd == "Close") {
                fetch = false;
            }
            if(cmd.substr(0, 15) == "CreateRandomNet") {
                create_randomNet(cmd.substr(15));
            }   
        }
    }
    ::close(Askpipe);
    ::close(Sendpipe);
    remove(CNTRL_SEND_FIFO); /* Delete the created fifo */
}
bool isConnected() {
    if(connected2cntrl) {
        return true;
    } 
    connected2cntrl= access( CNTRL_SEND_FIFO, F_OK ) != -1 ;//have a look if CNTRL_SEND_FIFO exits
    if(!connected2cntrl) {
        int pipe = mkfifo (CNTRL_SEND_FIFO, 0666);
        connected2cntrl = pipe > -1;
    }
    if(connected2cntrl){
        connected2cntrl=connected2cntrl && access( CNTRL_ASK_FIFO, F_OK ) != -1 ;//have a look if CNTRL_ASK_FIFO exits
        if(connected2cntrl) {
            Sendpipe = open(CNTRL_SEND_FIFO, O_WRONLY);
            int i = sendFIFO(Sendpipe, "Hi");
            Askpipe = open (CNTRL_ASK_FIFO, O_RDONLY);
            if(readFIFO(Askpipe) == "Hi") {

            }
        }
        return connected2cntrl;
    }
    connected2cntrl = false;
    return false;
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

    if(pipe <= 0) printf("Fifo open error\n"); 
    result = read(pipe, buffer, 1024); 
    //printf("reading: %s\n", buffer);
    return buffer;
}

void create_randomNet(std::string cmd) {//cmd has format InputNeurons:OutputNeurons:Layercount
    int nextId;
    Database db("database.db");
    db.nextId("Nets",&nextId);
    std::size_t pos1 = cmd.find(":");
    int ip_N = atoi(cmd.substr(0,pos1-1).c_str());
    std::size_t pos2 = cmd.find(":", pos1);
    int op_N = atoi(cmd.substr(pos1, pos2-1).c_str());
    int lc = atoi(cmd.substr(pos2).c_str());
    int io [] = {ip_N, op_N};

    net1 = new Net(nextId, lc, io);
    Bullshitterror = net1->Bullshit();
    save_Net(net1);
}
void load_Net(int i) {

}

int main () {
    Bullshitterror = 0;
    connected2cntrl = false;

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
