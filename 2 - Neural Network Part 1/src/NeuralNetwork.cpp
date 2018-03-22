#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(int numOfInput, int numOfHiddenLayers, int numOfNeuronsInHiddenLayers, int numOfOutput) : numOfInput(numOfInput), numOfHiddenLayers(numOfHiddenLayers), numOfNeuronsPerHiddenLayer(numOfNeuronsPerHiddenLayer), numOfOutput(numOfOutput), bias(1)
{
    // Create first layer
    layers.push_back(NeuronLayer(numOfNeuronsPerHiddenLayer, numOfInput));
    
    // Create any other subsequent hidden layers
    for (int i = 0; i < numOfHiddenLayers - 1; i++)
    {
        // Input from first hidden layer
        layers.push_back(NeuronLayer(numOfNeuronsPerHiddenLayer,
                                     numOfNeuronsPerHiddenLayer));
    }
    
    // Output layer
    // Input from subsequent or first hidden layer
    layers.push_back(NeuronLayer(numOfOutput, numOfNeuronsPerHiddenLayer));
}

std::vector<float> NeuralNetwork::GetOutput(std::vector<float>& input)
{
    std::vector<float> inputList(input);
    
    // Output from each layer
    std::vector<float> outputs;
    
    int weightCount = 0;
    
    // Return empty if not corrent number of inputs
    if (inputList.size() != numOfInput)
    {
        std::cout << "NeuralNetwork: input count incorrect" << std::endl;
        return outputs;
    }
    
    // Each layer
    for (int i = 0; i < numOfHiddenLayers + 1; i++)
    {
        if (i > 0)
        {
            // Clear input and add output from previous layer
            inputList.clear();
            inputList.insert(inputList.end(), outputs.begin(), outputs.end());
        }
        
        outputs.clear();
        
        weightCount = 0;
        
        for (int j = 0; j < layers[i].numOfNeurons; ++j)
        {
            float netInput = 0.0f;
            
            int NumInputs = layers[i].neurons[j].numOfInput;
            
            // Each weight
            for (int k = 0; k < NumInputs - 1; ++k)
            {
                // Sum the weights x inputs
                netInput += layers[i].neurons[j].weights[k] *
                inputList[weightCount++];
            }
            
            //Add in the bias
            netInput += layers[i].neurons[j].weights[numOfInput - 1] * bias;
            
            //Store result in output
            outputs.push_back(ToSigmoid(netInput));
            
            weightCount = 0;
        }
    }
    
    return outputs;
}

std::vector<float> NeuralNetwork::GetWeights()
{
    std::vector<float> weights;
    
    // Each layer
    for (int i = 0; i < numOfHiddenLayers + 1; ++i)
    {
        // Each neuron
        for (int j = 0; j < layers[i].numOfNeurons; ++j)
        {
            // Each weight
            for (int k = 0; k < layers[i].neurons[j].numOfInput; ++k)
            {
                weights.push_back(layers[i].neurons[j].weights[k]);
            }
        }
    }
    
    return weights;
}

void NeuralNetwork::SetWeights(const std::vector<float>& weights)
{
    // Used to cycle through received weights
    int weightCount = 0;
    
    // Each layer
    for (int i = 0; i < numOfHiddenLayers + 1; ++i)
    {
        // Each neuron
        for (int j = 0; j < layers[i].numOfNeurons; ++j)
        {
            // Each weight
            for (int k = 0; k < layers[i].neurons[j].numOfInput; ++k)
            {
                layers[i].neurons[j].weights[k] = weights[weightCount++];
            }
        }
    }
}

int NeuralNetwork::GetNumberOfWeights()
{
    int weights = 0;
    
    // Each layer
    for (int i = 0; i < numOfHiddenLayers + 1; ++i)
    {
        // Eeach neuron
        for (int j = 0; j < layers[i].numOfNeurons; ++j)
        {
            // Each weight
            for (int k = 0; k < layers[i].neurons[j].numOfInput; ++k)
            {
                weights++;
            }
        }
    }
    
    return weights;
}

float NeuralNetwork::ToSigmoid(float input)
{
    return (float) (1 / (1 + exp(-input)));
}
