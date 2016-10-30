#include "Neuron.h"

#include <stdlib.h>

Neuron::Neuron(int i) {
    id = i;
    bias = (double)rand() / RAND_MAX;
    threshold = (double)rand() / RAND_MAX + 1;
    double t = (double)clock();
    lastcall = t;
    fadetime = 0.000001;
}

Neuron::Neuron(int i, Neuron * old) {
    id = i;
    bias = old->bias;
    value = old->value;
    threshold = old->threshold;
    connections = old->connections;
    lastcall = old->lastcall;
    fadetime = old->fadetime;
    error = -1;
}

Neuron::Neuron(int i, double n_bias, double n_threshold, std::vector<Connection> n_connections) {
    id = i;
    bias = n_bias;
    threshold = n_threshold;
    connections = n_connections;
    error = -1;
    double t = (double)clock();
    lastcall = t;
    fadetime = 0.000001;
}

Neuron::~Neuron() {
    connections.erase(connections.begin(), connections.end());
    bias = 0;
    threshold = 0;
    id = 0;
    lastcall = 0;
    fadetime = 0;
    error = 0;
}

double Neuron::fire(double weight) {
    double conn_weight;
    double t = (double)clock();
    double timepassed = t - lastcall;
    value -= timepassed * fadetime;
    if(value < bias) {
        value = bias;
    }
    lastcall = t;
    value += weight;
    if(value >= threshold) {
       for(int i = 0; i < connections.size(); ++i) {
           conn_weight = connections[i].weight;
           connections[i].target->fire(conn_weight);
       }
       value = bias;
       return 1;
    }
    return 0;
}