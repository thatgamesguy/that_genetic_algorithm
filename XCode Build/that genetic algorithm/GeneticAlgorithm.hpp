#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp

#include <iostream>
#include <fstream>

#include "ObjectCollection.hpp"
#include "C_GeneticAgent.hpp"
#include "C_ScreenWrapAround.hpp"
#include "WorkingDirectory.hpp"
#include "C_DamageOnWallHit.hpp"
#include "C_AgentSight.hpp"
#include "C_AgentCollision.hpp"

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
    int FitnessProportionateSelection(int exclusion = -1);
    std::shared_ptr<C_GeneticAgent> CreateAgentFromCrossOver(const std::shared_ptr<C_GeneticAgent> parentOne, const std::shared_ptr<C_GeneticAgent> parentTwo);
    NeuralNetwork CreateNeuralNetworkFromCrossOver(const NeuralNetwork& networkOne, const NeuralNetwork& networkTwo);
    
    void LogToFile();
    void SavePoolToFile();
    void LoadPoolFromFile();
    
    ObjectCollection& agents;
    ResourceAllocator<sf::Texture>& textureAllocator;
    WorkingDirectory& workingDir;
    std::vector<std::shared_ptr<C_GeneticAgent>> pool;
    float totalFitnessScore;
    float mutationChance;
    int maxPoolSize;
    int addedToSimulation;
    int genNumber;
    
    std::ofstream log;
};

#endif /* GeneticAlgorithm_hpp */
