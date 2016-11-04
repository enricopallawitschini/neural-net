#include <stdio.h>
#include <iostream>
#include <time.h>

#include "Net.h"

using namespace std;

int main () {
    srand(time(0));
    int io [] = {10, 8};
    Net net1(1, 4, io);
    for(int i = 0; i < net1.size(); ++i) {
        Layer layer1 = net1.members[i];
        double r[layer1.size];
        for(int i = 0; i < layer1.size; ++i) {
            printf("\n\nID: %i\nBIAS %f\nTHRESHOLD: %f\n", layer1.members[i].id, layer1.members[i].bias, layer1.members[i].threshold);
            //r[i] = layer1.members[i].fire(0.4);
            //printf("\n\nFiring Neuron %i: %f", i, r[i]);
        }
        printf("\nLayer Type: %i\nLayer Size: %i", layer1.type_, layer1.size);
        printf("\nNeurons that fired:\n");
        for(int i = 0; i < layer1.size; ++i) {
            if(r[i]) printf("%i\n", i);
        }

        int conncount = 0;
        net1.Bullshit();
        for(int la = 0; la < net1.size(); ++la) {
            Layer * curLayer = &net1.members[la];
            for(int neur = 0; neur < curLayer->size; ++neur) {
                Neuron * curNeuron = &curLayer->members[neur];
                printf("\n\nNEURON WITH ID %i IS CONNECTED TO:\n", curNeuron->id);
                for(int conn = 0; conn < curNeuron->connections.size(); ++conn) {
                    Neuron::Connection * curConn = &curNeuron->connections[conn];
                    printf("%i, ", curConn->partner->id);
                    ++conncount;
                }
            }
        }

        printf("\n\nTOTAL CONNECTIONS: %i\n\n", conncount);
    }

    for (int InputNeur = 0; InputNeur < net1.members[0].size; ++InputNeur){
        bool res = net1.members[0].members[InputNeur].fire(rand() % RAND_MAX);
    }
    cin.get();
    return 0;
}