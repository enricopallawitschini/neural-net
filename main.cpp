#include <stdio.h>
#include <iostream>
#include <time.h>

#include "Database.h"

int Bullshitterror = 0;

int main ();
void create_randomNet();
void save_Net(Net * net);
Net load_Net(int i);
void print_Net(Net * n);
void fire_Random(Net net);

using namespace std;



int main () {
    srand(time(0));
    Net net1;
    int netId;
    cin >> netId;
    if(netId == -1){
        int io [] = {10, 8};
        Net net1(1, 4, io);
        Bullshitterror = net1.Bullshit();
        print_Net(&net1);
        save_Net(&net1);
    }else{
        Database db("database.db");
        net1 = db.retrieveNet(netId, "");
        print_Net(&net1);

    }
    print_Net(&net1);
    
    return Bullshitterror;
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
    cin.get();
    cin.get();

}

void fire_Random(Net net){
    for (int InputNeur = 0; InputNeur < net.layers[0].size; ++InputNeur){
        bool res = net.layers[0].neurons[InputNeur].fire(rand() / (double)RAND_MAX);
    }
}
