#include <stdio.h>
#include <iostream>
#include <time.h>

#include "Layer.h"

using namespace std;

int main () {
    printf("%i\n", (int)CLOCKS_PER_SEC);
    Layer in(1, 1);
    for(int i = 0; i < in.size; ++i) {
        printf("\n\nID: %i\n BIAS %f\n THRESHOLD: %f\n", in.members[i].id, in.members[i].bias, in.members[i].threshold);
        printf("\n\nFiring Neuron %i: %f", i, in.members[i].fire(0.4));
    }
    cin.get();
    return 0;
}