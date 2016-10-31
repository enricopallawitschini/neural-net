#include <stdio.h>
#include <iostream>
#include <time.h>

#include "Layer.h"

using namespace std;

int main () {
    printf("%i\n", (int)CLOCKS_PER_SEC);
    Layer layer1(1, 1);
    double r[layer1.Size()];
    for(int i = 0; i < layer1.Size(); ++i) {
        printf("\n\nID: %i\nBIAS %f\nTHRESHOLD: %f\n", layer1.members[i].id, layer1.members[i].bias, layer1.members[i].threshold);
        r[i] = layer1.members[i].fire(0.4);
        printf("\n\nFiring Neuron %i: %f", i, r[i]);
    }
    printf("\nLayer Type: %i\nLayer Size: %i", layer1.type, layer1.Size());
    printf("\nNeurons that fired:\n");
    for(int i = 0; i < layer1.Size(); ++i) {
        if(r[i]) printf("%i\n", i);
    }
    cin.get();
    return 0;
}