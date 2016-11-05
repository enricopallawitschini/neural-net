#include <vector>
#include <stdlib.h>
#include "Layer.h"
#include "Macros.h"



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