#include "C_AgentCollision.hpp"
#include "Object.hpp"

C_AgentCollision::C_AgentCollision(Object* owner) : Component(owner), energyReducedOnContact(20000.f){}

void C_AgentCollision::Awake()
{
    agent = owner->GetComponent<C_GeneticAgent>();
    sight = owner->GetComponent<C_AgentSight>();
}

void C_AgentCollision::Update(float deltaTime)
{
    const std::vector<AgentData>& agentsInSight = sight->GetAgentsInSight();
    const float agentRadius = agent->GetAgentRadius();
    
    for (int i = 0; i < agentsInSight.size(); i++)
    {
        const AgentData& data = agentsInSight[i];
        
        if(data.distance <= agentRadius * 0.5f)
        {
            agent->SetEnergy(agent->GetEnergy() - energyReducedOnContact * deltaTime);
        }
    }
}
