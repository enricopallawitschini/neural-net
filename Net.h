#ifndef NET_H
#define NET_H

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <string.h>
//#include <sys/stat.h>
#include <unistd.h>
#include <boost/interprocess/ipc/message_queue.hpp>

#include "Layer.h"
#include "Macros.h"


class Net {
    public:
    int size;
    int neuronindex;
    int id;
    char name[20];
     
    std::vector<Layer> layers;

    Net();
    Net(int i, int lc, int io[]);
    ~Net();
     
     
    int get_neuronindex();
    Neuron * get_Neuron(int i);
    int Bullshit();
    int publish(char const * mqname);
    int pipe_send(char const * mqname);

};

#endif