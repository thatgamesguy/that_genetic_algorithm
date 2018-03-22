#ifndef C_Velocity_hpp
#define C_Velocity_hpp

#include <math.h>

#include "Component.hpp"
#include "C_Sprite.hpp"

class C_Velocity : public Component
{
public:
    C_Velocity(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;
    
    void Add(const sf::Vector2f& vel);
    
    void Set(const sf::Vector2f& vel);
    
private:
    void UpdateAngle();
    void Clamp();
    void UpdatePosition(float deltaTime);
    
    sf::Vector2f velocity;
    float damping;
    sf::Vector2f maxVelocity;
    std::shared_ptr<C_Sprite> sprite;
};

#endif /* C_Velocity_hpp */

