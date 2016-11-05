#include "Neuron.h"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

Neuron::Neuron(int i, Neuron::NeuronType nt) {
    id = i;
    type = nt;
    bias = (double)rand() / (double)RAND_MAX;
    threshold = PREDEFINED_THRESHOLD;       //(double)rand() / (double)RAND_MAX + 1;
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
    printf("\nId: %i Got Input Weight: %f, Bias: %f Threshold: %f",this->id, weight, this->bias, this->threshold);

    value -= timepassed * fadetime;
    if(value < bias) {
        value = bias;
    }
    lastcall = t;
    value += weight;
    if(value >= threshold) {
       for(int i = 0; i < connections.size(); ++i) {
           if (connections[i].type == Out){
               conn_weight = connections[i].weight;
               printf("\nFiring Weight:%f to Id: %i",conn_weight, connections[i].partner->id);               
               connections[i].partner->fire(conn_weight);

           }  
       }
       value = bias;
       return 1;
    }
    return 0;
}

bool Neuron::isConnected() {
    if(connections.size() < 1) return false;
    if(connections.size() >= 1) {
        if(type == Input) return true;
        if(type == Output) return true;
    }
    if(type == Middle) {
        if(connections.size() == 1) return false;
        bool isConn = false;
        bool hasIn = false;
        bool hasOut = false;
        for(int i = 0; i < connections.size(); ++i) {
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
    if(connections.size() > 0) {
        for(int i = 0; i < connections.size(); ++i) {
            Connection * c = &connections[i];

            Neuron * p = connections[i].partner;
            if(connections[i].partner->id == target->id) return 2;
        }
    }
    double w = rand() / (double)RAND_MAX;
    //printf("\n Rand weight: %f \nRandMax:   %i ", w, RAND_MAX);               

    Connection conn;
    conn.partner = target;
    conn.weight = w;
    conn.type = Out;
    connections.push_back(conn);
    Connection connfortarget;
    connfortarget.weight = w;
    connfortarget.partner = this;
    connfortarget.type = In;
    target->connections.push_back(connfortarget);
    return -1;
}