#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp

#include "ObjectCollection.hpp"
#include "C_GeneticAgent.hpp"
#include "C_ScreenWrapAround.hpp"
#include "WorkingDirectory.hpp"

class GeneticAlgorithm
{
public:
    GeneticAlgorithm(ObjectCollection& agents, ResourceAllocator<sf::Texture>& textureAllocator, WorkingDirectory& workingDir);
    
    void Update(float deltaTime);
    
private:
    static bool PoolSort(std::shared_ptr<C_GeneticAgent> a, std::shared_ptr<C_GeneticAgent> b);
    
    std::shared_ptr<C_GeneticAgent> AddAgentToSimulation();
    void AddToPool(std::shared_ptr<C_GeneticAgent> agent);
    void CalculateTotalFitness();
    const std::shared_ptr<C_GeneticAgent> FitnessProportionateSelection();
    std::shared_ptr<C_GeneticAgent> CreateAgentFromCrossOver(const std::shared_ptr<C_GeneticAgent> parentOne, const std::shared_ptr<C_GeneticAgent> parentTwo);
    NeuralNetwork CreateNeuralNetworkFromCrossOver(const NeuralNetwork& networkOne, const NeuralNetwork& networkTwo);
    
    ObjectCollection& agents;
    ResourceAllocator<sf::Texture>& textureAllocator;
    WorkingDirectory& workingDir;
    std::vector<std::shared_ptr<C_GeneticAgent>> pool;
    float totalFitnessScore;
    float crossOverPoint;
    float mutationChance;
    int maxPoolSize;
};

#endif /* GeneticAlgorithm_hpp */
