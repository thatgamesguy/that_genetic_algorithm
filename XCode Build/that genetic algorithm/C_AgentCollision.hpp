#ifndef C_AgentCollision_hpp
#define C_AgentCollision_hpp

#include "Component.hpp"
#include "C_GeneticAgent.hpp"

class C_AgentCollision : public Component
{
public:
    C_AgentCollision(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;

private:
    std::shared_ptr<C_GeneticAgent> agent;
    std::shared_ptr<C_AgentSight> sight;
    float energyReducedOnContact;
};

#endif /* C_AgentCollision_hpp */
