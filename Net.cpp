
#include "Net.h"
#include "Database.h"

Net::Net() {
    // Uns is fad
}

Net::Net(int i, int lc, int io[]) {
    if(lc < MIN_LAYER_CNT) {
        size = MIN_LAYER_CNT;
    } else {
        size = lc;
    }
    id = i;
    sprintf(name, "net%i", id);
    int start_id;
    Database db("database.db");
    db.nextId("layers", &start_id);
    db.nextId("neurons", &neuronindex);
    int layer_index = 0;
    for(int n = start_id; n < start_id + size; ++n) {
        int neuroncount = rand() % MAX_NEURON_PL + 1;
        if(neuroncount < MIN_NEURON_PL) neuroncount = MIN_NEURON_PL;
        Layer::LayerType type = Layer::Middle;
        if(n == start_id) {
            type = Layer::Input;
            neuroncount = io[0];
        } 
        if(n == start_id + size-1) {
            type = Layer::Output;
            neuroncount = io[1];
        } 
        layers.push_back(Layer(n, type, neuroncount, neuronindex, layer_index));
        neuronindex = layers[layer_index].neuronindex;
        ++layer_index;
    }
}

Net::~Net() {
    layers.erase(layers.begin(), layers.end());
}

int Net::Bullshit() {
    /* INTERESTING:
    *  Total Connections are not a factor of 10 because:
    *  Sometimes Connecting Fails -> Connection already exists!
    */

    bool keepConnecting = true;
    int n = 0;
    int err = 0;

    while(keepConnecting) {
        int ln = rand() % (size - 1);    //Selected layer
        Layer * curLayer = &layers[ln];    //Current Layer
        
        int nn = rand() % curLayer->size; //Select random Neuron
        Neuron * curNeuron = &curLayer->neurons[nn];
        printf("\nId: Neuron to Connect from: %i ",curNeuron->id);

        std::vector<Neuron *> n2l;      //All Neurons from the next CONNECTION_JUMP Layers
        for(int l2n = ln+1; l2n < ln+CONNECTION_JUMP; ++l2n) {
            if(l2n > size) break;
            int abc = layers[l2n].size;
            for(int n2n = 0; n2n < abc; ++n2n) {
                n2l.push_back(&layers[l2n].neurons[n2n]);
            }
        }

        int nn_in_n2l = rand() % n2l.size();
        Neuron * connNeuron = n2l[nn_in_n2l];

        printf("\nId: Neuron to Connect to: %i ",connNeuron->id);

        err += curNeuron->connect(connNeuron);

        ++n;
        if(n % 1000 == 0) {
            bool isConn = true;
            for(int layer = 0; layer < size; ++layer) {
                for(int neuron = 0; neuron < layers[layer].size; ++neuron) {
                    Neuron * ne = &layers[layer].neurons[neuron];
                    isConn = layers[layer].neurons[neuron].isConnected();
                    if(!isConn) {
                        bool blabla = ne->isConnected();
                        goto endloop;
                    }
                }
            }
            endloop:
            keepConnecting = !isConn;
        }
        if(n > 1000000){
            for(int show_layers = 0; show_layers < layers.size(); ++show_layers){
                for(int show_neurons = 0; show_neurons < layers[show_layers].neurons.size(); ++show_neurons) {
                    if(!layers[show_layers].neurons[show_neurons].isConnected()) {
                        printf("Neuron not connected Id: %i, Layer: %i\n", layers[show_layers].neurons[show_neurons].id, show_layers);
                        return err;
                    }

                }
            }

        }
    }
    return err;
}

int Net::get_neuronindex(){
    if(neuronindex >0) return neuronindex;

    if(size == 0) return 0;
    neuronindex=0;
    for(int i=0; i < size; ++i){
        neuronindex += this->layers[i].size;
    }
    return neuronindex;
}

Neuron * Net::get_Neuron(int i){
    for(int layer = 0; layer < size; ++layer){
        Layer * curLayer = &layers[layer];
        for(int neuron = 0; neuron < curLayer->size; ++neuron){
            Neuron * curNeuron = &curLayer->neurons[neuron];
            if(curNeuron->id == i) return curNeuron;
        }

    }
}

int Net::pipe_send(char const * mqname){
    char out[BUFFERSIZE];
    int ret;
    ret = sprintf(out,"%s:%i:%i:%s:%i", BUILD_CMD, NET, id, name, size);
    try {
        char message[BUFFERSIZE];
        strcpy(message, out);
        boost::interprocess::message_queue mq_send(boost::interprocess::open_only, mqname);
        mq_send.send(&message, sizeof(message), 0);
    }
    catch(boost::interprocess::interprocess_exception &ex){
        printf("Error send net: %s\n", ex.what());
    }
}

int Net::publish(char const * mqname) {
    pipe_send(mqname);
    for(int i = 0; i < size; ++i){
        layers[i].publish(mqname, id);
    }
    char out[BUFFERSIZE];
    int ret;
    ret = sprintf(out,"%s:%i:%i:%s:%i", BUILD_CMD, NET, id, name, size);
    try {
        char message[BUFFERSIZE];
        strcpy(message, BUILD_CMD);
        strcat(message, ":ENDNET");
        boost::interprocess::message_queue mq_send(boost::interprocess::open_only, mqname);
        mq_send.send(&message, sizeof(message), 0);
    }
    catch(boost::interprocess::interprocess_exception &ex){
        printf("Error send ENDNET: %s\n", ex.what());
    }
}

