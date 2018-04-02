#include "GeneticAlgorithm.hpp"

GeneticAlgorithm::GeneticAlgorithm(ObjectCollection& agents, ResourceAllocator<sf::Texture>& textureAllocator, WorkingDirectory& workingDir) : agents(agents), textureAllocator(textureAllocator), workingDir(workingDir), totalFitnessScore(0.f), mutationChance(0.1f), maxPoolSize(40), addedToSimulation(0), genNumber(0)
{
    const int initialAgentCount = 52;
    
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
    
    ufo->AddComponent<C_DamageOnWallHit>();
    
    auto sight = ufo->AddComponent<C_AgentSight>();
    sight->SetObjectCollection(&agents);
    
    ufo->AddComponent<C_AgentCollision>();
    
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
    screenWrapAround->SetSpriteHalfSize({23.f, 23.f});
    
    agents.Add(ufo);
    
    return agent;
}

bool GeneticAlgorithm::PoolSort(std::shared_ptr<C_GeneticAgent> a, std::shared_ptr<C_GeneticAgent> b)
{
    return a->GetTimeAlive() > b->GetTimeAlive();
}

void GeneticAlgorithm::Update(float deltaTime)
{
    std::vector<std::shared_ptr<Object>>& objects = agents.GetObjects();
    
    for(auto& o : objects)
    {
        auto geneticAgent = o->GetComponent<C_GeneticAgent>();
        
        if(geneticAgent != nullptr && geneticAgent->GetEnergy() <= 0)
        {
            AddToPool(geneticAgent);
            
            CalculateTotalFitness();
            
            if(pool.size() > 1)
            {
                int parentOne = FitnessProportionateSelection();
                int parentTwo = FitnessProportionateSelection();
                
                std::shared_ptr<C_GeneticAgent> agent = CreateAgentFromCrossOver(pool[parentOne], pool[parentTwo]);
                
                float mutation = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX));
                if (mutation < mutationChance)
                {
                    NeuralNetwork network = agent->GetNeuralNetwork();
                    network.MutateExchange();
                    agent->SetNeuralNetwork(network);
                }
                
                if(++addedToSimulation % maxPoolSize == 0)
                {
                    LogToFile();
                    addedToSimulation = 0;
                }
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

int GeneticAlgorithm::FitnessProportionateSelection(int exclusion)
{
    float randomSlice = static_cast <float>(rand()) / (static_cast <float>(RAND_MAX / totalFitnessScore));

    std::shared_ptr<C_GeneticAgent> choosenAgent = nullptr;
    
    float fitnessTotal = 0;
    
    for (int i = 0; i < pool.size(); i++)
    {
        if(i == exclusion)
        {
            continue;
        }
        
        fitnessTotal += pool[i]->GetTimeAlive();
        
        if (fitnessTotal >= randomSlice)
        {
            return i;
        }
    }
    
    return -1;
}

std::shared_ptr<C_GeneticAgent> GeneticAlgorithm::CreateAgentFromCrossOver(const std::shared_ptr<C_GeneticAgent> parentOne, const std::shared_ptr<C_GeneticAgent> parentTwo)
{
    NeuralNetwork neuralNetwork = parentOne->GetNeuralNetwork();
    
    if(parentOne != parentTwo)
    {
        neuralNetwork = CreateNeuralNetworkFromCrossOver(parentOne->GetNeuralNetwork(), parentTwo->GetNeuralNetwork());
    }

    auto agent = AddAgentToSimulation();
    agent->SetNeuralNetwork(neuralNetwork);
    
    return agent;
}

NeuralNetwork GeneticAlgorithm::CreateNeuralNetworkFromCrossOver(const NeuralNetwork& networkOne, const NeuralNetwork& networkTwo)
{
    std::vector<float> parentOneWeights = networkOne.GetWeights();
    std::vector<float> parentTwoWeights = networkTwo.GetWeights();
    
    std::vector<int> crossoverPoints = networkOne.GetSplitPoints();
    
    int crossOverIndex = (rand() % static_cast<int>(crossoverPoints.size()));
    int crossOverPoint = crossoverPoints[crossOverIndex];
    
    std::vector<float> newWeights;
    
    for (int i = 0; i < crossOverPoint; i++)
    {
        newWeights.push_back(parentOneWeights[i]);
    }
    
    for (int i = crossOverPoint; i < parentOneWeights.size(); i++)
    {
        newWeights.push_back(parentTwoWeights[i]);
    }
    
    int numOfInput = networkOne.numOfInput;
    int numOfHiddenLayers = networkOne.numOfHiddenLayers;
    int numOfNeuronsInHiddenLayer = networkOne.numOfNeuronsInHiddenLayers;
    int numOfOutput = networkOne.numOfOutput;
    
    NeuralNetwork neuralNet(numOfInput, numOfHiddenLayers, numOfNeuronsInHiddenLayer, numOfOutput);
    neuralNet.SetWeights(newWeights);
    
    return neuralNet;
}

void GeneticAlgorithm::LogToFile()
{
    log.open("ga_log.txt", std::ios_base::app | std::ios_base::out);
 
    log << std::to_string(++genNumber) << ": " << std::to_string(totalFitnessScore / maxPoolSize) <<  std::endl;
    
    log.close();
}

void GeneticAlgorithm::SavePoolToFile()
{
    log.open("pool_data.txt");
    
    for(const auto& p : pool)
    {
        std::vector<float> weights = p->GetNeuralNetwork().GetWeights();
        
        for(const auto& w : weights)
        {
            log << w << std::endl;
        }
        
        log << std::endl;
    }
    
    log.close();
}
