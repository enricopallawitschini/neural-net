#include "Layer.h"

Layer::Layer(int i, Layer::LayerType t, int s) {
    id = i;
    type = t;
    size = s;
}

Layer::Layer(int i, Layer::LayerType t, int nc, int sid) {
    id = i;
    type = t;
    size = nc;
    neuronindex = sid;
    for(int n = sid; n < size+sid; ++n) {
        Neuron::NeuronType nt = Neuron::Middle;
        if(type == Input) nt = Neuron::Input;
        if(type == Output) nt = Neuron::Output;
        neurons.push_back(Neuron(neuronindex, nt));
        ++neuronindex;
    }
}

Layer::~Layer() {
    neurons.erase(neurons.begin(), neurons.end());
}