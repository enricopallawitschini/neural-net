#ifndef LAYER_H
#define LAYER_H

#include "Neuron.h"
#include <vector>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <boost/interprocess/ipc/message_queue.hpp>

class Layer {
    public:
    enum LayerType {
        Input,
        Middle,
        Output
    };
    int id;
    int neuronindex;
    int size;
    int number_in_net;
    std::vector<Neuron> neurons;
    LayerType type;

    Layer(int i, LayerType t, int s, int n_i_n);
    Layer(int i, LayerType t, int nc, int sid, int n_i_n);
    ~Layer();
    int publish(char const * mqname, int net_id);
    int pipe_send(char const * mqname, int net_id);
};

#endif