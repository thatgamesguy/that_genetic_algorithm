#include "GeneticAlgorithm.hpp"

GeneticAlgorithm::GeneticAlgorithm(ObjectCollection& agents, ResourceAllocator<sf::Texture>& textureAllocator, WorkingDirectory& workingDir) : agents(agents), textureAllocator(textureAllocator), workingDir(workingDir), totalFitnessScore(0.f), crossOverPoint(0.5f), mutationChance(0.3f), maxPoolSize(20)
{
    const int initialAgentCount = 100;
    
    for (int i = 0; i < initialAgentCount; i++)
    {
        AddAgentToSimulation();
    }
}

std::shared_ptr<C_GeneticAgent> GeneticAlgorithm::AddAgentToSimulation()
{
    const int minX = 50;
    const int minY = 50;
    
    const int maxX = 750;
    const int maxY = 550;
    
    const std::string spritePreName = "ufo";
    const std::string spritePostName = ".png";
    
    std::shared_ptr<Object> ufo = std::make_shared<Object>();
    
    ufo->AddComponent<C_Velocity>();
    auto agent = ufo->AddComponent<C_GeneticAgent>();
    agent->SetObjectCollection(&agents);

    auto sprite = ufo->AddComponent<C_Sprite>();
    sprite->SetTextureAllocator(&textureAllocator);
    const std::string ufoCount = std::to_string(1 + (std::rand() % (4 - 1 + 1))); // Selects a random ufo sprite.
    sprite->Load(workingDir.Get() + spritePreName + ufoCount + spritePostName);
    sprite->SetCenter(23.f, 23.f);
    
    const int randX = minX + (std::rand() % (maxX - minX + 1));
    const int randY = minY + (std::rand() % (maxY - minY + 1));
    ufo->transform->SetPosition(randX, randY);
    
    auto screenWrapAround = ufo->AddComponent<C_ScreenWrapAround>();
    screenWrapAround->SetSpriteHalfSize({23.5f, 23.5f});
    
    agents.Add(ufo);
    
    return agent;
}

bool GeneticAlgorithm::PoolSort(std::shared_ptr<C_GeneticAgent> a, std::shared_ptr<C_GeneticAgent> b)
{
    return a->GetTimeAlive() > b->GetTimeAlive();
}

void GeneticAlgorithm::Update(float deltaTime)
{
    const int maxNewInAFrame = 1;
    int currentNew = 0;
    
    std::vector<std::shared_ptr<Object>>& objects = agents.GetObjects();

    for(auto& o : objects)
    {
        if(currentNew >= maxNewInAFrame)
        {
            return;
        }
        
        auto geneticAgent = o->GetComponent<C_GeneticAgent>();
        
        if(geneticAgent->GetEnergy() <= 0)
        {
            currentNew++;
            
            AddToPool(geneticAgent);
            
            CalculateTotalFitness();
            
            if(pool.size() > 1)
            {
                const std::shared_ptr<C_GeneticAgent> parentOne = FitnessProportionateSelection();
                const std::shared_ptr<C_GeneticAgent> parentTwo = FitnessProportionateSelection();
                
                CreateAgentFromCrossOver(parentOne, parentTwo);
            }
            
            o->QueueForRemoval();
        }
    }
}

void GeneticAlgorithm::AddToPool(std::shared_ptr<C_GeneticAgent> agent)
{
    pool.push_back(agent);
    
    std::sort(pool.begin(), pool.end(), PoolSort);
    
    if(pool.size() > maxPoolSize)
    {
        pool.erase(pool.end() - 1);
    }
}

void GeneticAlgorithm::CalculateTotalFitness()
{
    totalFitnessScore = 0;
    
    for (int i = 0; i < pool.size(); i++)
    {
        totalFitnessScore += pool[i]->GetTimeAlive();
    }
}

const std::shared_ptr<C_GeneticAgent> GeneticAlgorithm::FitnessProportionateSelection()
{
    float randomSlice = static_cast <float>(rand()) / (static_cast <float>(RAND_MAX / totalFitnessScore));
    
    std::shared_ptr<C_GeneticAgent> choosenAgent = nullptr;
    
    float fitnessTotal = 0;
    
    for (int i = 0; i < pool.size(); i++)
    {
        fitnessTotal += pool[i]->GetTimeAlive();
        
        if (fitnessTotal > randomSlice)
        {
            choosenAgent = pool[i];
            break;
        }
    }
    
    return choosenAgent;
}

std::shared_ptr<C_GeneticAgent> GeneticAlgorithm::CreateAgentFromCrossOver(const std::shared_ptr<C_GeneticAgent> parentOne, const std::shared_ptr<C_GeneticAgent> parentTwo)
{
    NeuralNetwork neuralNetwork = CreateNeuralNetworkFromCrossOver(parentOne->GetNeuralNetwork(), parentTwo->GetNeuralNetwork());
    
    auto agent = AddAgentToSimulation();
    agent->SetNeuralNetwork(neuralNetwork);
    
    float mutation = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX));
    
    if (mutation < mutationChance)
    {
        agent->MutateNeuralNetwork();
    }
    
    return agent;
}

NeuralNetwork GeneticAlgorithm::CreateNeuralNetworkFromCrossOver(const NeuralNetwork& networkOne, const NeuralNetwork& networkTwo)
{
    int numOfInput = networkOne.numOfInput;
    int numOfHiddenLayers = networkOne.numOfHiddenLayers;
    int numOfNeuronsInHiddenLayer = networkOne.numOfNeuronsInHiddenLayers;
    int numOfOutput = networkOne.numOfOutput;
    
    NeuralNetwork neuralNet(numOfInput, numOfHiddenLayers, numOfNeuronsInHiddenLayer, numOfOutput);
    
    std::vector<float> parentOneWeights = networkOne.GetWeights();
    std::vector<float> parentTwoWeights = networkTwo.GetWeights();
    
    int crossOver = parentOneWeights.size() * crossOverPoint;
    
    std::vector<float> newWeights;
    
    for (int i = 0; i < crossOver; i++)
    {
        newWeights.push_back(parentOneWeights[i]);
    }
    
    for (int i = crossOver; i < parentOneWeights.size(); i++)
    {
        newWeights.push_back(parentTwoWeights[i]);
    }
    
    neuralNet.SetWeights(newWeights);
    
    return neuralNet;
}

