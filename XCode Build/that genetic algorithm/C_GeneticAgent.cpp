#include "C_GeneticAgent.hpp"
#include "Object.hpp"

float C_GeneticAgent::sightRadius(120.f);
float C_GeneticAgent::agentRadius(20.f);

C_GeneticAgent::C_GeneticAgent(Object* owner) : Component(owner), maxMoveForce(80.f), energy(100.f), energyReductionMultiplier(0.2f), neuralNetwork(neuralNumOfInput, neuralNumOfHiddenLayers, neuralNumOfNeuronsInHiddenLayer, neuralNumOfOutput), energyReducedOnContact(100.f),
mutationRate(0.5f)
{
    
}

void C_GeneticAgent::Awake()
{
    velocity = owner->GetComponent<C_Velocity>();
}

void C_GeneticAgent::Update(float deltaTime)
{
    if(energy <= 0)
    {
        return;
    }
    
    timeAlive += deltaTime;
    
    std::vector<AgentPositionData> agentsInSight = GetAgentsInSight();
    
    ApplyDamageOnCollision(agentsInSight, deltaTime);
    
    //std::vector<float> neuralNetInput = CalculateNetworkInput(agentsInSight);
    
    sf::Vector2f closestAgentPos = GetClosestAgentPosition(agentsInSight);
    std::vector<float> neuralNetInput;
    neuralNetInput.push_back(closestAgentPos.x);
    neuralNetInput.push_back(closestAgentPos.y);
    
    // Get output from neural network
    std::vector<float> output = neuralNetwork.GetOutput(neuralNetInput);

    float x = output[0];
    float y = output[1];

    //use velocity normal as input?
    
    /*
    float moveUp = output[0];
    float moveDown = output[1];
    float moveLeft = output[2];
    float moveRight = output[3];
    
    //float x = moveLeft > moveRight ? -moveLeft : moveRight;
    //float y = moveUp > moveDown ? -moveUp : moveDown;
    
    // If move left force is greater than the move right force then we want to move left.
    float x = moveLeft > moveRight ? -(moveLeft - moveRight) : moveRight - moveLeft;
    float y = moveUp > moveDown ? -(moveUp - moveDown) : moveDown - moveUp;
    */
    
    const sf::Vector2f move = sf::Vector2f(x, y) * maxMoveForce * deltaTime;
    
    velocity->Add(move);
    
   // const sf::Vector2f vel = velocity->Get();
    
    // We sum all forces when calculating energy reduction.
    float energyReduction = (fabs(x) + fabs(y)) * energyReductionMultiplier * deltaTime;
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

std::vector<AgentPositionData> C_GeneticAgent::GetAgentsInSight()
{
    std::vector<AgentPositionData> positionData;
    
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
                AgentPositionData data;
                data.position = otherPos;
                data.heading = heading;
                data.distance = distance;
                
                positionData.push_back(data);
            }
        }
    }
    
    return positionData;
}

std::vector<float> C_GeneticAgent::CalculateNetworkInput(std::vector<AgentPositionData> positionData)
{
    std::vector<float> input;
    
    // We fill our array with 0 value floats to start with.
    // An input will only be higher than 0 if there is an another agent within our sight radius.
    for (int i = 0; i < neuralNumOfInput; i++)
    {
        input.push_back(0.f);
    }
    
    for (int i = 0; i < positionData.size(); i++)
    {
        AgentPositionData& data = positionData[i];
        
        sf::Vector2f dir = data.heading / data.distance;
        float angle = (atan2(dir.y, dir.x)) * 180 / M_PI;                 // angle can be between 0 and 180 and 0 and -180
        
        // First we calculate how detailed we want our indexing. We split it into half as the angle can be positive or negative.
        int halfSplit = 180 / (neuralNumOfInput / 2);
        
        int index = ceil(fabs(angle) / halfSplit) - 1;
        
        if(angle < 0)
        {
            index += (neuralNumOfInput / 2); // This converts it to an index representing a minus angle.
        }
        
        // We need to convert the distance to a number between 0 and 1 to be used as input.
        float normalisedDistance = data.distance / sightRadius;
        
        // We want a higher number to represent a closer agent so we invert the number.
        float inverseDistance = 1 - normalisedDistance;
        
        // Now we have our index and input, we first check if this input is greater than what we already have stored, if so we overwrite it.
        if(inverseDistance > input[index])
        {
            input[index] = inverseDistance;
        }
    }
    
    return input;
}

float C_GeneticAgent::Magnitude(const sf::Vector2f& a)
{
    return sqrt((a.x * a.x) + (a.y * a.y));
}

void C_GeneticAgent::ApplyDamageOnCollision(std::vector<AgentPositionData> agentsInSight, float deltaTime)
{
    for (int i = 0; i < agentsInSight.size(); i++)
    {
        AgentPositionData& data = agentsInSight[i];
        
        if(data.distance <= agentRadius)
        {
            energy -= energyReducedOnContact * deltaTime;
        }
    }
}

sf::Vector2f C_GeneticAgent::GetClosestAgentPosition(std::vector<AgentPositionData> agentsInSight)
{
    float distance = MAXFLOAT;
    AgentPositionData* found = nullptr;
    
    for (int i = 0; i < agentsInSight.size(); i++)
    {
        AgentPositionData& data = agentsInSight[i];
        
        if(data.distance < distance)
        {
            found = &data;
            distance = data.distance;
        }
    }
    
    if(found != nullptr)
    {
        sf::Vector2f dir = found->heading / found->distance;
        return dir;
    }
    
    return sf::Vector2f(0.f, 0.f);
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

void C_GeneticAgent::MutateNeuralNetwork()
{
    std::vector<float> weights(neuralNetwork.GetWeights());
    
    const int minOffset = -1;
    const int maxOffset = 1;
    
    for (int i = 0; i < weights.size(); ++i)
    {
        float mutation = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX));
        if (mutation < mutationRate)
        {
            float weightOffset = minOffset + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxOffset - minOffset)));
            weights[i] += weightOffset;
           
            weights[i] = (weights[i] <= -1.f) ? -1.f : (weights[i] >= 1.f) ? 1.f : weights[i]; // Clamp weight between -1 and 1.
        }
    }
    
    neuralNetwork.SetWeights(weights);
}

