#include "Neuron.h"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

Neuron::Neuron(int i, Neuron::NeuronType nt, int l_i_n) {
    id = i;
    type = nt;
    bias = (double)rand() / (double)RAND_MAX;
    threshold = PREDEFINED_THRESHOLD;       //(double)rand() / (double)RAND_MAX + 1;
    double t = (double)clock();
    lastcall = t;
    fadetime = 0.000001;
    layer_in_net = l_i_n;
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
    layer_in_net = old->layer_in_net;
}

Neuron::Neuron(int i, NeuronType nt, int n_layer, double n_bias, double n_threshold, double n_fadetime, int l_i_n) {
    id = i;
    type = nt;
    bias = n_bias;
    threshold = n_threshold;
    fadetime = n_fadetime;
    value = bias;
    layer = n_layer;
    layer_in_net = l_i_n;
}

Neuron::Neuron(int i, double n_bias, double n_threshold, std::vector<Connection> n_connections) {
    id = i;
    bias = n_bias;
    threshold = n_threshold;
    connections = n_connections;
    error = -1;
    double t = (double)clock();
    lastcall = t;
    fadetime = FADETIME_PER_CLOCK;
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
    //printf("\nId: %i Got Input Weight: %f, Bias: %f Threshold: %f, time: %f, CPS:%f",this->id, weight, this->bias, this->threshold, t, (double)CLOCKS_PER_SEC);

    value -= timepassed * fadetime;
    if(value < bias) {
        value = bias;
    }
    lastcall = t;
    value += weight;
    if(value >= threshold) {
        char message[BUFFERSIZE];
        int ret;
        ret = sprintf(message,"%s:%i:%i", CNTRL_CMD, CMD_FIRE, id);
        try {
            boost::interprocess::message_queue mq_send(boost::interprocess::open_only, "ctrl_send");
            mq_send.try_send(&message, sizeof(message), 0);
        }
        catch(boost::interprocess::interprocess_exception &ex){
            printf("Error sending neuron: %s\n", ex.what());
        }

        for(int i = 0; i < connections.size(); ++i) {
            if (connections[i].type == Out){
                conn_weight = connections[i].weight;
                //printf("\nFiring Weight:%f to Id: %i, time %f",conn_weight, connections[i].partner->id, t);               
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
            if(connections[i].partner->id == target->id) return 1;
        }
    }
    double w = (rand() / (double)RAND_MAX) - 0.5;
    printf("\n Conn id: %i Target id: %i ", this->id, target->id);               

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
    return 0;
}

int Neuron::connect(Neuron * target, double n_weight) {
    if(connections.size() > 0) {
        for(int i = 0; i < connections.size(); ++i) {
            Connection * c = &connections[i];

            Neuron * p = connections[i].partner;
            if(connections[i].partner->id == target->id) return 1;
        }
    }
    printf("\n Conn from db id: %i Target id: %i ", this->id, target->id);               

    Connection conn;
    conn.partner = target;
    conn.weight = n_weight;
    conn.type = Out;
    connections.push_back(conn);
    Connection connfortarget;
    connfortarget.weight = n_weight;
    connfortarget.partner = this;
    connfortarget.type = In;
    target->connections.push_back(connfortarget);
    return 0;
}

int Neuron::pipe_send(char const * mqname, int net_id, int layer){
    char message[BUFFERSIZE];
    int ret;
    
    ret = sprintf(message,"%s:%i:%i:%i:%i:%f:%f:%f:%i", BUILD_CMD, NEURON, net_id, layer, id, bias, threshold, fadetime, type);
    try {
        boost::interprocess::message_queue mq_send(boost::interprocess::open_only, mqname);
        mq_send.send(&message, sizeof(message), 0);
    }
    catch(boost::interprocess::interprocess_exception &ex){
        printf("Error sending neuron: %s\n", ex.what());
    }
}

int Neuron::publish(char const * mqname, int net_id, int layer) {
    pipe_send(mqname, net_id, layer);
    try {
        boost::interprocess::message_queue mq_send(boost::interprocess::open_only, mqname);
        for(int i = 0; i < connections.size(); ++i){
            if(connections[i].type == Out) {
                char message[BUFFERSIZE];
                int partner_id = connections[i].partner->id;
                double d_weight = connections[i].weight;
                int ret = sprintf(message,"%s:%i:%i:%i:%i:%f", BUILD_CMD, CONNECTION, net_id, id, partner_id, d_weight);
                mq_send.send(&message, sizeof(message), 0);
            }
        }
    }
    catch(boost::interprocess::interprocess_exception &ex){
        printf("Error sending neuron: %s\n", ex.what());
    }

}