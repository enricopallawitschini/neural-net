#include "Neuron.h"
#include <vector>

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
     std::vector<Neuron> neurons;
     LayerType type;

    Layer(int i, LayerType t, int s);
    Layer(int i, LayerType t, int nc, int sid);
    ~Layer();
};