#ifndef Neuron_hpp
#define Neuron_hpp

#include <vector>

struct Neuron
{
    Neuron(int numOfInput);
    
    int numOfInput; // Number of inputs into the neuron.
    std::vector<float> weights; // Weight of each input determines activity of network.
};

#endif /* Neuron_hpp */
