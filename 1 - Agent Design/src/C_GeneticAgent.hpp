#ifndef C_GeneticAgent_hpp
#define C_GeneticAgent_hpp

#include "Component.hpp"
#include "C_Velocity.hpp"

class C_GeneticAgent : public Component
{
public:
    C_GeneticAgent(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;
    
    void SetEnergy(float amount);
    float GetEnergy();
    
    static float GetSightRadius();
    
private:
    static float sightRadius;
    float maxMoveForce;
    std::shared_ptr<C_Velocity> velocity;
    float energy;
    float energyReductionMultiplier;
};

#endif /* C_GeneticAgent_hpp */
