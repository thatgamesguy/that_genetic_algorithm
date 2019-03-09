#include "C_GeneticAgent.hpp"
#include "Object.hpp"

C_GeneticAgent::C_GeneticAgent(Object* owner) : Component(owner), maxMoveForce(1400.f), energy(200.f), neuralNetwork(neuralNumOfInput, neuralNumOfHiddenLayers, neuralNumOfNeuronsInHiddenLayer, neuralNumOfOutput), timeAlive(0.f), agentRadius(28.f), windowSize(1920, 1080)
{
    
}

void C_GeneticAgent::Awake()
{
    velocity = owner->GetComponent<C_Velocity>();
    sight = owner->GetComponent<C_AgentSight>();
}

void C_GeneticAgent::Update(float deltaTime)
{
    if(energy <= 0)
    {
        return;
    }
    
    timeAlive += deltaTime;
    
    std::vector<float> neuralNetInput = BuildNetworkInput();
    
    // Get output from neural network
    std::vector<float> neuralNetOutput = neuralNetwork.GetOutput(neuralNetInput);

    float x = neuralNetOutput[0];
    float y = neuralNetOutput[1];

    const sf::Vector2f move = sf::Vector2f(x, y) * maxMoveForce * deltaTime;
    
    velocity->Set(move);
}

void C_GeneticAgent::SetWindowSize(const sf::Vector2u& windowSize)
{
    this->windowSize = windowSize;
}

void C_GeneticAgent::SetEnergy(float amount)
{
    energy = amount;
}

float C_GeneticAgent::GetEnergy() const
{
    return energy;
}

void C_GeneticAgent::SetObjectCollection(ObjectCollection* collection)
{
    this->collection = collection;
}

float C_GeneticAgent::GetAgentRadius() const
{
    return agentRadius;
}

float C_GeneticAgent::GetTimeAlive() const
{
    return timeAlive;
}

void C_GeneticAgent::SetNeuralNetwork(NeuralNetwork& network)
{
    neuralNetwork = network;
}

const NeuralNetwork& C_GeneticAgent::GetNeuralNetwork() const
{
    return neuralNetwork;
}

std::vector<float> C_GeneticAgent::BuildNetworkInput()
{
    std::vector<float> networkInput;
    
    for (int i = 0; i < neuralNumOfInput; i++)
    {
        networkInput.push_back(0.f);
    }

    std::shared_ptr<AgentData> closest = sight->GetClosestAgent();
    
    if(closest)
    {
        sf::Vector2f to = closest->heading / closest->distance;
        
        networkInput[0] = to.x;
        networkInput[1] = to.y;

        // We need to convert the distance to a number between 0 and 1 to be used as input.
        float normalisedDistance = closest->distance / sight->GetRadius();
        
        // We want a higher number to represent a closer agent so we invert the number.
        networkInput[2] = 1 - normalisedDistance;
        
        const sf::Vector2f& pos = owner->transform->GetPosition();
        
        float leftDistance = pos.x;
        float topDistance =  pos.y;
        float rightDistance = fabs(windowSize.x - pos.x);
        float bottomDistance = fabs(windowSize.y - pos.y);
        
        networkInput[3] = 1 - (leftDistance / windowSize.x);
        networkInput[4] = 1 - (rightDistance / windowSize.x);
        networkInput[5] = 1 - (topDistance / windowSize.y);
        networkInput[6] = 1 - (bottomDistance / windowSize.y);
        
        // Could use below but found agents evolve quicker when using discrete values for each wall.
        //networkInput[3] = pos.x / screenWidth;
        //networkInput[4] = pos.y / screenHeight;
    }
    
    return networkInput;
}

