#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <math.h>
#include <time.h>
#include "Macros.h"
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <boost/interprocess/ipc/message_queue.hpp>

class Neuron {
    public:
    enum ConnectionType {
        In,
        Out
    };

    enum NeuronType {
        Input,
        Middle,
        Output
    };

    struct Connection {
        Neuron * partner;
        ConnectionType type;
        double weight;
    };

    int id;
    double value;
    double bias;
    double threshold;
    std::vector<Connection> connections;
    double lastcall;
    double fadetime;
    double error;
    int layer;
    int layer_in_net;
    NeuronType type;

    Neuron(int i, NeuronType nt, int l_i_n);
    Neuron(int i, Neuron *);
    Neuron(int i, NeuronType nt, int n_layer, double n_bias, double n_threshold, double n_fadetime, int l_i_n);
    Neuron(int i, double n_bias, double n_threshold, std::vector<Connection> n_connections);
    ~Neuron();

    double fire(double weight);
    bool isConnected();
    int connect(Neuron * target);
    int connect(Neuron * target, double n_weight);
    int publish(char const * mqname, int net_id, int layer);
    int pipe_send(char const * mqname, int net_id, int layer);

};

#endif