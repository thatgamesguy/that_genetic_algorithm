#include "C_GeneticAgent.hpp"
#include "Object.hpp"

float C_GeneticAgent::sightRadius(80.f);

C_GeneticAgent::C_GeneticAgent(Object* owner) : Component(owner), maxMoveForce(100.f), energy(60.f), energyReductionMultiplier(2.f), neuralNetwork(neuralNumOfInput, neuralNumOfHiddenLayers, neuralNumOfNeuronsInHiddenLayer, neuralNumOfOutput)
{
    
}

void C_GeneticAgent::Awake()
{
    velocity = owner->GetComponent<C_Velocity>();
}

void C_GeneticAgent::Update(float deltaTime)
{
    std::vector<float> neuralNetInput = CalculateNetworkInput();
    
    // Get output from neural network
    std::vector<float> output = neuralNetwork.GetOutput(neuralNetInput);

    float moveUp = output[0];
    float moveDown = output[1];
    float moveLeft = output[2];
    float moveRight = output[3];
    
    // If move left force is greater than the move right force then we want to move left.
    float x = moveLeft > moveRight ? -(moveLeft - moveRight) : moveRight - moveLeft;
    float y = moveUp > moveDown ? -(moveUp - moveDown) : moveDown - moveUp;

    const sf::Vector2f move = sf::Vector2f(x, y) * maxMoveForce * deltaTime;
    
    velocity->Add(move);
    
    // We sum all forces when calculating energy reduction.
    float energyReduction = (moveUp + moveDown + moveLeft + moveRight) * energyReductionMultiplier * deltaTime;
    energy -= energyReduction;
}

void C_GeneticAgent::SetEnergy(float amount)
{
    energy = amount;
}

float C_GeneticAgent::GetEnergy()
{
    return energy;
}

void C_GeneticAgent::SetObjectCollection(ObjectCollection* collection)
{
    this->collection = collection;
}

float C_GeneticAgent::GetSightRadius()
{
    return sightRadius;
}

std::vector<float> C_GeneticAgent::CalculateNetworkInput()
{
    std::vector<float> input;
    // We fill our array with 0 value floats to start with.
    // An input will only be higher than 0 if there is an another agent within our sight radius.
    for (int i = 0; i < neuralNumOfInput; i++)
    {
        input.push_back(0.f);
    }
    
    std::vector<std::shared_ptr<Object>>& objects = collection->GetObjects();
    
    const int thisID = owner->instanceID->Get();
    const sf::Vector2f& thisPos = owner->transform->GetPosition();
    
    for (int i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<Object>& other = objects[i];
        
        if(thisID != other->instanceID->Get())
        {
            const sf::Vector2f otherPos = other->transform->GetPosition();
            
            sf::Vector2f heading = otherPos - thisPos;
            float distance = Magnitude(heading);
            
            if(distance < sightRadius)
            {
                sf::Vector2f dir = heading / distance;
                float angle = (atan2(dir.y, dir.x)) * 180 / M_PI;                 // angle can be between 0 and 180 and 0 and -180
                
                // First we calculate how detailed we want our indexing. We split it into half as the angle can be positive or negative.
                int halfSplit = 180 / (neuralNumOfInput / 2);
                
                int index = ceil(fabs(angle) / halfSplit) - 1;
                
                if(angle < 0)
                {
                    index += 4; // This converts it to an index representing a minus angle.
                }
                
                // We need to convert the distance to a number between 0 and 1 to be used as input.
                float normalisedDistance = distance / sightRadius;
                
                // We want a higher number to represent a closer agent so we invert the number.
                float inverseDistance = 1 - normalisedDistance;
                
                // Now we have our index and input, we first check if this input is greater than what we already have stored, if so we overwrite it.
                if(inverseDistance > input[index])
                {
                    input[index] = inverseDistance;
                }
            }
        }
    }
    
    return input;
}

float C_GeneticAgent::Magnitude(const sf::Vector2f& a)
{
    return sqrt((a.x * a.x) + (a.y * a.y));
}

