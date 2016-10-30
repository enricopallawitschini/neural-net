#include <vector>
#include <math.h>
#include <time.h>

class Neuron {
    public:
    struct Connection {
         Neuron * target;
         double weight;
     };

     int id;
     double value;
     double bias;
     double threshold;
     std::vector<Connection> connections;
     double lastcall;
     double fadetime;
     double error;

     Neuron(int i);
     Neuron(int i, Neuron *);
     Neuron(int i, double n_bias, double n_threshold, std::vector<Connection> n_connections);
     ~Neuron();

     double fire(double weight);
};