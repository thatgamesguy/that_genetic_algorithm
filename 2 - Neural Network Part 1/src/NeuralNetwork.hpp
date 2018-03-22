#ifndef NeuralNetwork_hpp
#define NeuralNetwork_hpp

#include <math.h>
#include <iostream>

#include "NeuronLayer.hpp"

class NeuralNetwork
{
public:
    NeuralNetwork(int numOfInput, int numOfHiddenLayers, int numOfNeuronsInHiddenLayers, int numOfOutput);
    
    std::vector<float> GetOutput(std::vector<float>& input);
    
    std::vector<float> GetWeights();
    void SetWeights(const std::vector<float>& weights);
    
    int GetNumberOfWeights();
    
private:
    float ToSigmoid(float input);
    
    int numOfInput; // Number of inputs for each neuron
    int numOfOutput; // Number of outputs of each neuron
    int numOfHiddenLayers; // Number of hidden layers
    int numOfNeuronsPerHiddenLayer; // Number of neurons per hidden layer
    std::vector<NeuronLayer> layers;
    int bias;
};

#endif /* NeuralNetwork_hpp */
