#include <vector>
#include <stdlib.h>
#include "Layer.h"

#define MAX_NEURON_PL 30
#define MIN_LAYER_CNT 4
#define CONNECTION_JUMP 2

class Net {
    private:
     int size_;
    public:
     int size() const {
         return size_;
     }

     void size(int i) {
         size_ = i;
     }
     int neuronindex;
     int id;
     
     std::vector<Layer> members;

     Net(int i, int lc, int io[]);
     ~Net();

     void Bullshit();
};