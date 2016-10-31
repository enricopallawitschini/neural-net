#include "Layer.h"

Layer::Layer(int i, int t) {
    id = i;
    type = t;
    switch(type) {
        case 1: //INPUT
            size = 10;
            break;
        case 2: //MIDDLE
            size = 20;
            break;
        case 3: //OUTPUT
            size = 4;
            break;
        default:
            size = 20;
            break;
    }
    for(int ii = 0; ii < size; ++ii) {
        members.push_back(Neuron(ii));
    }
}

Layer::~Layer() {
    members.erase(members.begin(), members.end());
}

int Layer::Size() {
    return size;
}