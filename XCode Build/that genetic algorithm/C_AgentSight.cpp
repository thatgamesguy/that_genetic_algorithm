#include "C_AgentSight.hpp"

C_AgentSight::C_AgentSight(Object* owner) : Component(owner), calculated(false), sightRadius(100.f){}

void C_AgentSight::SetObjectCollection(ObjectCollection* collection)
{
    objectCollection = collection;
}

void C_AgentSight::LateUpdate(float deltaTime)
{
    agentsInSight.clear();
    closestAgent = nullptr;
    calculated = false;
}

const std::vector<AgentData>& C_AgentSight::GetAgentsInSight()
{
    if(!calculated)
    {
        CalculateAgentsInSight();
        calculated = true;
    }
    
    return agentsInSight;
}

std::shared_ptr<AgentData> C_AgentSight::GetClosestAgent()
{
    if(!calculated)
    {
        CalculateAgentsInSight();
        calculated = true;
    }
    
    return closestAgent;
}

float C_AgentSight::GetRadius() const
{
    return sightRadius;
}

void C_AgentSight::CalculateAgentsInSight()
{
    std::vector<std::shared_ptr<Object>>& objects = objectCollection->GetObjects();
 
    float closestDist = MAXFLOAT;
    const int thisID = owner->instanceID->Get();
    const sf::Vector2f& thisPos = owner->transform->GetPosition();
    
    for (int i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<Object> other = objects[i];
        
        if(thisID != other->instanceID->Get())
        {
            const sf::Vector2f otherPos = other->transform->GetPosition();
            
            sf::Vector2f heading = otherPos - thisPos;
            float distance = Magnitude(heading);
            
            if(distance < sightRadius)
            {
                AgentData data;
                data.position = otherPos;
                data.heading = heading;
                data.distance = distance;
                
                agentsInSight.push_back(data);
                
                if(distance < closestDist)
                {
                    closestAgent = std::make_shared<AgentData>(data);
                    closestDist = distance;
                }
            }
        }
    }
}
