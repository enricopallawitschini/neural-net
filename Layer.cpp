#include "Layer.h"

Layer::Layer(int i, Layer::LayerType t, int nc, int sid) {
    id = i;
    type_ = t;
    size = nc;
    neuronindex = sid;
    for(int n = sid; n < size+sid; ++n) {
        Neuron::NeuronType nt = Neuron::Middle;
        if(type_ == Input) nt = Neuron::Input;
        if(type_ == Output) nt = Neuron::Output;
        members.push_back(Neuron(neuronindex, nt));
        ++neuronindex;
    }
}

Layer::~Layer() {
    members.erase(members.begin(), members.end());
}