#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(int numOfInput, int numOfHiddenLayers, int numOfNeuronsInHiddenLayers, int numOfOutput) : numOfInput(numOfInput), numOfHiddenLayers(numOfHiddenLayers), numOfNeuronsInHiddenLayers(numOfNeuronsInHiddenLayers), numOfOutput(numOfOutput), bias(-1), splitPoints(0)
{
    // Create first layer
    layers.push_back(NeuronLayer(numOfNeuronsInHiddenLayers, numOfInput));
    
    // Create any other subsequent hidden layers
    for (int i = 0; i < numOfHiddenLayers; i++)
    {
        // Input from first hidden layer
        layers.push_back(NeuronLayer(numOfNeuronsInHiddenLayers,
                                     numOfNeuronsInHiddenLayers));
    }

    // Output layer
    // Input from subsequent or first hidden layer
    layers.push_back(NeuronLayer(numOfOutput, numOfNeuronsInHiddenLayers));
    
    CalculateSplitPoints();
}

std::vector<float> NeuralNetwork::GetOutput(const std::vector<float>& input)
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
    for (int i = 0; i < layers.size(); i++)
    {
        if (i > 0)
        {
            // Clear input and add output from previous layer
            inputList.clear();
            inputList.insert(inputList.end(), outputs.begin(), outputs.end());
            outputs.clear();
            weightCount = 0;
        }

        for (int j = 0; j < layers[i].numOfNeurons; j++)
        {
            float netInput = 0.0f;
            
            int numInputs = layers[i].neurons[j].numOfInput;
            
            // Each weight
            for (int k = 0; k < numInputs - 1; k++)
            {
                // Sum the weights x inputs
                netInput += layers[i].neurons[j].weights[k] *
                inputList[weightCount++];
            }
            
            //Add in the bias
            netInput += layers[i].neurons[j].weights[numInputs - 1] * bias;
            
            //Store result in output
        //    float sigOutput = FastSigmoid(netInput);
            outputs.push_back(netInput);
            
            weightCount = 0;
        }
    }
    
    return outputs;
}

std::vector<float> NeuralNetwork::GetWeights() const
{
    std::vector<float> weights;
    
    // Each layer
    for (int i = 0; i < layers.size(); i++)
    {
        // Each neuron
        for (int j = 0; j < layers[i].numOfNeurons; j++)
        {
            // Each weight
            for (int k = 0; k < layers[i].neurons[j].numOfInput; k++)
            {
                weights.push_back(layers[i].neurons[j].weights[k]);
            }
        }
    }
    
    return weights;
}

void NeuralNetwork::SetWeights(const std::vector<float>& weights)
{
    if(weights.size() != GetWeights().size())
    {
        return;
    }
    
    // Used to cycle through received weights
    int weightCount = 0;
    
    // Each layer
    for (int i = 0; i < layers.size(); i++)
    {
        // Each neuron
        for (int j = 0; j < layers[i].numOfNeurons; j++)
        {
            // Each weight
            for (int k = 0; k < layers[i].neurons[j].numOfInput; k++)
            {
                layers[i].neurons[j].weights[k] = weights[weightCount++];
            }
        }
    }
}

int NeuralNetwork::GetNumberOfWeights() const
{
    int weights = 0;
    
    // Each layer
    for (int i = 0; i < layers.size(); i++)
    {
        // Eeach neuron
        for (int j = 0; j < layers[i].numOfNeurons; j++)
        {
            // Each weight
            for (int k = 0; k < layers[i].neurons[j].numOfInput; k++)
            {
                weights++;
            }
        }
    }
    
    return weights;
}

const std::vector<int>& NeuralNetwork::GetSplitPoints() const
{
    return splitPoints;
}

void NeuralNetwork::Mutate(float mutationRate, bool clamp)
{    
    const int minOffset = -1;
    const int maxOffset = 1;
    
    std::vector<float> weights = GetWeights();
    
    for (int i = 0; i < weights.size(); ++i)
    {
        float mutation = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX));
        if (mutation < mutationRate)
        {
            float weightOffset = minOffset + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxOffset - minOffset)));
            weights[i] += weightOffset;
            
            if(clamp)
            {
                weights[i] = (weights[i] <= -1.f) ? -1.f : (weights[i] >= 1.f) ? 1.f : weights[i]; // Clamp weight between -1 and 1.
            }
        }
    }
    
    SetWeights(weights);
}

void NeuralNetwork::MutateExchange()
{
    int numOfWeights = GetNumberOfWeights();
    
    int numOfSwaps = (rand() % static_cast<int>(numOfWeights / 2));
    
    std::vector<float> weights = GetWeights();
    
    for (int i = 0; i < numOfSwaps; i++)
    {
        int pos1 = (rand() % static_cast<int>(numOfWeights));
        int pos2 = pos1;
        
        while(pos1 == pos2)
        {
            pos2 = (rand() % static_cast<int>(numOfWeights));
        }
        
        std::swap(weights[pos1], weights[pos2]);
    }
    
    SetWeights(weights);
}

float NeuralNetwork::FastSigmoid(float input)
{
    //return erf(sqrt(M_PI) * input / 2);
    return input / (1 + fabs(input));
    
    //return 1 / (1 + exp(-input));
}

void NeuralNetwork::CalculateSplitPoints()
{
    splitPoints.clear();
    
    int weightCounter = 0;
    
    // Each layer
    for (int i = 0; i < layers.size(); i++)
    {
        // Eeach neuron
        for (int j = 0; j < layers[i].numOfNeurons; j++)
        {
            // Each weight
            for (int k = 0; k < layers[i].neurons[j].numOfInput; k++)
            {
                weightCounter++;
            }
            
            splitPoints.push_back(weightCounter - 1);
        }
    }
}
