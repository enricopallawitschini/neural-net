#include <vector>
#include <math.h>
#include <time.h>

class Neuron {
    public:
     enum ConnectionType {
         In,
         Out
     };

     enum NeuronType {
         Input,
         Middle,
         Output
     };

     struct Connection {
         Neuron * partner;
         ConnectionType type;
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
     NeuronType type;

     Neuron(int i, NeuronType nt);
     Neuron(int i, Neuron *);
     Neuron(int i, double n_bias, double n_threshold, std::vector<Connection> n_connections);
     ~Neuron();

     double fire(double weight);
     bool isConnected();
     int connect(Neuron * target);
};