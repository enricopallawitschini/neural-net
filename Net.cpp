#include <stdio.h>
#include <iostream>
#include "Net.h"

Net::Net() {
    // Uns is fad
}

Net::Net(int i, int lc, int io[]) {
    if(lc < MIN_LAYER_CNT) {
        size = MIN_LAYER_CNT;
    } else {
        size = lc;
    }
    id = i;
    neuronindex = 0;
    sprintf(name, "net%i", id);
    for(int n = 0; n < size; ++n) {
        int neuroncount = rand() % MAX_NEURON_PL + 1;
        if(n == 0) neuroncount = io[0];
        if(n == size-1) neuroncount = io[1];

        Layer::LayerType type = Layer::Middle;
        if(n == size-1) type = Layer::Output;
        if(n == 0) type = Layer::Input;
        layers.push_back(Layer(n, type, neuroncount, neuronindex));
        neuronindex = layers[n].neuronindex;
    }
}

Net::~Net() {
    layers.erase(layers.begin(), layers.end());
}

int Net::Bullshit() {


    /* INTERESTING:
    *  Total Connections are not a factor of 10 because:
    *  Sometimes Connecting Fails -> Connection already exists!
    */

    bool keepConnecting = true;
    int n = 0;
    int err = 0;

    while(keepConnecting) {
        int ln = rand() % (size - 1);    //Selected layer
        Layer * curLayer = &layers[ln];    //Current Layer
        
        int nn = rand() % curLayer->size; //Select random Neuron
        Neuron * curNeuron = &curLayer->neurons[nn];
        printf("\nId: Neuron to Connect from: %i ",curNeuron->id);

        std::vector<Neuron *> n2l;      //All Neurons from the next CONNECTION_JUMP Layers
        for(int l2n = ln+1; l2n < ln+CONNECTION_JUMP; ++l2n) {
            if(l2n > size) break;
            int abc = layers[l2n].size;
            for(int n2n = 0; n2n < abc; ++n2n) {
                n2l.push_back(&layers[l2n].neurons[n2n]);
            }
        }

        int nn_in_n2l = rand() % n2l.size();
        Neuron * connNeuron = n2l[nn_in_n2l];

        printf("\nId: Neuron to Connect to: %i ",connNeuron->id);

        err += curNeuron->connect(connNeuron);

        ++n;
        if(n % 10 == 0) {
            bool isConn = true;
            for(int layer = 0; layer < size; ++layer) {
                for(int neuron = 0; neuron < layers[layer].size; ++neuron) {
                    Neuron * ne = &layers[layer].neurons[neuron];
                    isConn = layers[layer].neurons[neuron].isConnected();
                    if(!isConn) {
                        bool blabla = ne->isConnected();
                        goto endloop;
                    }
                }
            }
            endloop:
            keepConnecting = !isConn;
        }
    }
    return err;
}

int Net::get_neuronindex(){
    if(neuronindex >0) return neuronindex;

    if(size == 0) return 0;
    neuronindex=0;
    for(int i=0; i < size; ++i){
        neuronindex += this->layers[i].size;
    }
    return neuronindex;
}

Neuron * Net::get_Neuron(int i){
    for(int layer = 0; layer < size; ++layer){
        Layer * curLayer = &layers[layer];
        for(int neuron = 0; neuron < curLayer->size; ++neuron){
            Neuron * curNeuron = &curLayer->neurons[neuron];
            if(curNeuron->id == i) return curNeuron;
        }

    }
}

int Net::pipe_send(std::string pipename){
    char out[BUFFERSIZE];
    int ret;
    
    ret = sprintf(out,"%i:%i:%s:%i", NET, id, name, size);
    
    int p = open(pipename.c_str(), O_WRONLY);
    if(p == 0){
        printf("Could not open fifo");
    } 
    write(p, out, strlen(out)+1);
    close(p);

}

