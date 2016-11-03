#include <stdio.h>
#include <iostream>
#include "Net.h"

Net::Net(int i, int lc, int io[]) {
    if(lc < MIN_LAYER_CNT) {
        size_ = MIN_LAYER_CNT;
    } else {
        size_ = lc;
    }
    id = i;
    neuronindex = 0;
    for(int n = 0; n < size_; ++n) {
        int neuroncount = rand() % MAX_NEURON_PL + 1;
        if(n == 0) neuroncount = io[0];
        if(n == size_-1) neuroncount = io[1];

        Layer::LayerType type = Layer::Middle;
        if(n == size_-1) type = Layer::Output;
        if(n == 0) type = Layer::Input;
        members.push_back(Layer(n, type, neuroncount, neuronindex));
        neuronindex = members[n].neuronindex;
    }
}

Net::~Net() {
    members.erase(members.begin(), members.end());
}

void Net::Bullshit() {
    bool keepConnecting = true;
    int n = 0;
    printf("while");
    std::cin.get();
    while(keepConnecting) {
        int ln = rand() % (size_ - 1);    //Selected layer
        Layer * curLayer = &members[ln];    //Current Layer
        
        int nn = rand() % curLayer->size - 1; //Select random Neuron
        Neuron * curNeuron = &curLayer->members[nn];

        std::vector<Neuron *> n2l;      //All Neurons from the next 2 Layers
        for(int ii = ln+1; ii < ln+CONNECTION_JUMP; ++ii) {
            if(ii > size_) break;
            for(int iii = 0; iii < members[ii].size; ++iii) {
                n2l.push_back(&members[ii].members[iii]);
            }
        }


    printf("nach erstem for size: %i", (int) n2l.size());
    std::cin.get();

        int nn_in_n2l = rand() % n2l.size() - 1;
        curNeuron->connect(n2l[nn_in_n2l]);

        ++n;
        /*if(n % 10 == 0) {
            bool isConn = true;
            for(int layer = 0; layer < size_; ++layer) {
                for(int neuron = 0; neuron < members[layer].size; ++neuron) {
                    isConn = isConn || members[layer].members[neuron].isConnected();
                    if(isConn) {
                        keepConnecting = false;
                    } else {
                        break;
                    }
                }
            }
        }*/

    printf("ende der schleife");
    std::cin.get();
    }
}