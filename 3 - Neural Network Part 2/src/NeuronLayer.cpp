#include "NeuronLayer.hpp"

NeuronLayer::NeuronLayer(int numOfNeurons, int numOfInput)
{
    this->numOfNeurons = numOfNeurons;
    
    //Adds neurons to neuron list
    for (int i = 0; i < numOfNeurons; ++i)
    {
        neurons.push_back(Neuron(numOfInput));
    }
}
