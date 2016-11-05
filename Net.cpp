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

    while(keepConnecting) {
        int ln = rand() % (size_ - 1);    //Selected layer
        Layer * curLayer = &members[ln];    //Current Layer
        
        int nn = rand() % curLayer->size; //Select random Neuron
        Neuron * curNeuron = &curLayer->members[nn];
        printf("\nId: Neuron to Connect from: %i ",this->id);

        std::vector<Neuron *> n2l;      //All Neurons from the next 2 Layers
        for(int ii = ln+1; ii < ln+CONNECTION_JUMP; ++ii) {
            if(ii > size_) break;
            int abc = members[ii].size;
            for(int iii = 0; iii < abc; ++iii) {
                n2l.push_back(&members[ii].members[iii]);
            }
        }

        int nn_in_n2l = rand() % n2l.size();
        printf("\nId: Neuron to Connect to: %i ",n2l[nn_in_n2l]->id);

        curNeuron->connect(n2l[nn_in_n2l]);

        ++n;
        if(n % 10 == 0) {
            bool isConn = true;
            for(int layer = 0; layer < size_; ++layer) {
                for(int neuron = 0; neuron < members[layer].size; ++neuron) {
                    Neuron * ne = &members[layer].members[neuron];
                    isConn = members[layer].members[neuron].isConnected();
                    if(!isConn) {
                        bool bla = ne->isConnected();
                        goto endloop;
                    }
                }
            }
            endloop:
            keepConnecting = !isConn;
        }
    }
}