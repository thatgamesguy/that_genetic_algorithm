#include "Neuron.hpp"

Neuron::Neuron(int numOfInput)
{
    this->numOfInput = numOfInput + 1;

    const int min = -1;
    const int max = 1;
    
    //Initialise random weights for each input
    for (int i = 0; i < this->numOfInput; i++)
    {
        float weight = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
        weights.push_back(weight);
    }
}
