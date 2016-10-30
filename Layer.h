#include <vector>

#include "Neuron.h"

class Layer {
    public:
     int id;
     int size;
     std::vector<Neuron> members;
     int type;

     Layer(int i, int t = 2);
     ~Layer();

     int Size();
};