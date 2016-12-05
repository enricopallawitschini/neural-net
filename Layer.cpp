#include "Layer.h"

Layer::Layer(int i, Layer::LayerType t, int s, int n_i_n) {
    id = i;
    type = t;
    size = s;
    number_in_net = n_i_n;
}

Layer::Layer(int i, Layer::LayerType t, int nc, int sid, int n_i_n) {
    id = i;
    type = t;
    size = nc;
    neuronindex = sid;
    number_in_net = n_i_n;

    for(int n = sid; n < size+sid; ++n) {
        Neuron::NeuronType nt = Neuron::Middle;
        if(type == Input) nt = Neuron::Input;
        if(type == Output) nt = Neuron::Output;
        neurons.push_back(Neuron(neuronindex, nt, number_in_net));
        ++neuronindex;
    }
}

Layer::~Layer() {
    neurons.erase(neurons.begin(), neurons.end());
}

int Layer::pipe_send(char const * mqname, int net_id){
    char out[BUFFERSIZE];
    int ret;
    ret = sprintf(out,"%s:%i:%i:%i:%i:%i", BUILD_CMD, LAYER, net_id, id, type, size);
    try {
        char message[BUFFERSIZE];
        strcpy(message, out);
        boost::interprocess::message_queue mq_send(boost::interprocess::open_only, mqname);
        mq_send.send(&message, sizeof(message), 0);
    }
    catch(boost::interprocess::interprocess_exception &ex){
        printf("Error sending layer: %s\n", ex.what());
    }
}

int Layer::publish(char const * mqname, int net_id) {
    pipe_send(mqname, net_id);
    for(int i = 0; i < size; ++i){
        neurons[i].publish(mqname, net_id, id);
    }
}