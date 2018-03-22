#ifndef C_GeneticAgent_hpp
#define C_GeneticAgent_hpp

#include "Component.hpp"
#include "C_Velocity.hpp"
#include "NeuralNetwork.hpp"
#include "ObjectCollection.hpp"

class C_GeneticAgent : public Component
{
public:
    C_GeneticAgent(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;
    
    void SetEnergy(float amount);
    float GetEnergy();
    
    void SetObjectCollection(ObjectCollection* collection);
    
    static float GetSightRadius();
    
private:
    std::vector<float> CalculateNetworkInput();
    float Magnitude(const sf::Vector2f& a);
    
    const int neuralNumOfInput = 8;
    const int neuralNumOfHiddenLayers = 1;
    const int neuralNumOfNeuronsInHiddenLayer = 10;
    const int neuralNumOfOutput = 4;
    
    static float sightRadius;
    float maxMoveForce;
    std::shared_ptr<C_Velocity> velocity;
    float energy;
    float energyReductionMultiplier;
    NeuralNetwork neuralNetwork;
    
    ObjectCollection* collection;
};

#endif /* C_GeneticAgent_hpp */
