#ifndef C_AgentSight_hpp
#define C_AgentSight_hpp

#include <math.h>

#include "Component.hpp"
#include "ObjectCollection.hpp"

struct AgentData
{
    sf::Vector2f position;
    sf::Vector2f heading;
    float distance;
};

class C_AgentSight : public Component
{
public:
    C_AgentSight(Object* owner);
    
    void SetObjectCollection(ObjectCollection* collection);
    
    void LateUpdate(float deltaTime) override;
    
    const std::vector<AgentData>& GetAgentsInSight();
    
    std::shared_ptr<AgentData> GetClosestAgent();
    
    float GetRadius() const;
    
private:
    inline float Magnitude(const sf::Vector2f& v)
    {
        return sqrt((v.x * v.x) + (v.y * v.y));
    }
    
    inline sf::Vector2f Normalise(const sf::Vector2f& v)
    {
        float l = Magnitude(v);
        
        if (l != 0)
        {
            return v / l;
        }
        
        return v;
    }
    
    void CalculateAgentsInSight();
    
    float sightRadius;
    ObjectCollection* objectCollection;
    bool calculated;
    std::vector<AgentData> agentsInSight;
    std::shared_ptr<AgentData> closestAgent;
};

#endif /* C_AgentSight_hpp */
