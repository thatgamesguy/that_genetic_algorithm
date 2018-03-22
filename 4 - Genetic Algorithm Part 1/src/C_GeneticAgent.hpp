#ifndef C_GeneticAgent_hpp
#define C_GeneticAgent_hpp

#include "Component.hpp"
#include "C_Velocity.hpp"
#include "NeuralNetwork.hpp"
#include "ObjectCollection.hpp"

struct AgentPositionData
{
    sf::Vector2f heading;
    float distance;
};

class C_GeneticAgent : public Component
{
public:
    C_GeneticAgent(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;
    
    void SetEnergy(float amount);
    float GetEnergy();
    
    void SetObjectCollection(ObjectCollection* collection);
    
    float GetTimeAlive() const;
    
    void SetNeuralNetwork(NeuralNetwork& network);
    const NeuralNetwork& GetNeuralNetwork() const;
    
    void MutateNeuralNetwork();
    
    static float GetSightRadius();
    
private:
    std::vector<AgentPositionData> GetAgentsInSight();
    std::vector<float> CalculateNetworkInput(std::vector<AgentPositionData> positionData);
    float Magnitude(const sf::Vector2f& a);
    void ApplyDamageOnCollision(std::vector<AgentPositionData> agentsInSight, float deltaTime);
    
    static float sightRadius;
    static float agentRadius;
    
    const int neuralNumOfInput = 8;
    const int neuralNumOfHiddenLayers = 1;
    const int neuralNumOfNeuronsInHiddenLayer = 10;
    const int neuralNumOfOutput = 4;

    float maxMoveForce;
    std::shared_ptr<C_Velocity> velocity;
    float energy;
    float energyReductionMultiplier;
    float energyReducedOnContact;
    float timeAlive;
    float mutationRate;
    NeuralNetwork neuralNetwork;
    
    ObjectCollection* collection;
};

#endif /* C_GeneticAgent_hpp */
