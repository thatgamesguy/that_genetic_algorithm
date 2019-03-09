#ifndef C_GeneticAgent_hpp
#define C_GeneticAgent_hpp

#include "Component.hpp"
#include "C_Velocity.hpp"
#include "NeuralNetwork.hpp"
#include "ObjectCollection.hpp"
#include "C_AgentSight.hpp"

class C_GeneticAgent : public Component
{
public:
    C_GeneticAgent(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;
    
    void SetWindowSize(const sf::Vector2u& windowSize);
    
    void SetEnergy(float amount);
    float GetEnergy() const;
    
    void SetObjectCollection(ObjectCollection* collection);
    
    float GetTimeAlive() const;
    
    void SetNeuralNetwork(NeuralNetwork& network);
    const NeuralNetwork& GetNeuralNetwork() const;
    
    float GetAgentRadius() const;
    
private:
    std::vector<float> BuildNetworkInput();
    
    const int neuralNumOfInput = 7;
    const int neuralNumOfHiddenLayers = 1;
    const int neuralNumOfNeuronsInHiddenLayer = 10;
    const int neuralNumOfOutput = 2;

    sf::Vector2u windowSize;
    
    float agentRadius;
    float maxMoveForce;
    float energy;
    float timeAlive;
    std::shared_ptr<C_Velocity> velocity;
    std::shared_ptr<C_AgentSight> sight;
    NeuralNetwork neuralNetwork;
    
    ObjectCollection* collection;
};



#endif /* C_GeneticAgent_hpp */
