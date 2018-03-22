#ifndef NeuronLayer_hpp
#define NeuronLayer_hpp

#include "Neuron.hpp"

struct NeuronLayer
{
    NeuronLayer(int numOfNeurons, int numOfInput);
    
    int numOfNeurons;
    std::vector<Neuron> neurons;
};

#endif /* NeuronLayer_hpp */
