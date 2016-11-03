#include "Neuron.h"
#include <stdlib.h>
#include <algorithm>

Neuron::Neuron(int i, Neuron::NeuronType nt) {
    id = i;
    type = nt;
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
           connections[i].partner->fire(conn_weight);
       }
       value = bias;
       return 1;
    }
    return 0;
}

bool Neuron::isConnected() {
    if(connections.size() < 1) return false;
    if(connections.size() == 1) {
        if(type == Input) return true;
        if(type == Output) return true;
        return false;
    }
    if(type == Middle) {
        bool isConn = false;
        bool hasIn = false;
        bool hasOut = false;
        int i = 0;
        while(!isConn) {
            if(connections.size() > i-1) return false;
            hasIn = hasIn || connections[i].type == In;
            hasOut = hasOut || connections[i].type == Out;
            if(hasIn && hasOut) {
                isConn = true;
                return true;
            }
        }
    }
    return false;
}

int Neuron::connect(Neuron * target) {
    for(int i = 0; i < connections.size(); ++i) {
        if(connections[i].partner->id == target->id) return 2;
    }
    Connection conn;
    conn.partner = target;
    conn.weight = rand() % RAND_MAX;
    conn.type = Out;
    connections.push_back(conn);
    return -1;
}