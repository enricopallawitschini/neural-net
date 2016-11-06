#include <vector>
#include <stdlib.h>
#include "Layer.h"
#include "Macros.h"



class Net {
    public:
     int size;
     int neuronindex;
     int id;
     
     std::vector<Layer> layers;

     Net();
     Net(int i, int lc, int io[]);
     ~Net();

     int Bullshit();
};