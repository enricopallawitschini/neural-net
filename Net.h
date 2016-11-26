#include <vector>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
//#include <sys/stat.h>
#include <unistd.h>

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
     int pipe_send(std::string pipename);

};