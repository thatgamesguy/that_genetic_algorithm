#include "C_Velocity.hpp"
#include "Object.hpp"

C_Velocity::C_Velocity(Object* owner) : Component(owner), velocity(0.f, 0.f), damping(0.69f), maxVelocity(170.f, 170.f) {}

void C_Velocity::Awake()
{
    sprite = owner->GetComponent<C_Sprite>();
}

void C_Velocity::Update(float deltaTime)
{
    velocity *= powf(damping, deltaTime);
    
    UpdateAngle();
    
    UpdatePosition(deltaTime);
}

void C_Velocity::Add(const sf::Vector2f& vel)
{
    velocity += vel;
    
    Clamp();
}

void C_Velocity::UpdateAngle()
{
    float angle = (atan2(velocity.y, velocity.x)) * 180 / M_PI;
    sprite->SetRotation(angle);
}

void C_Velocity::Clamp()
{
    if (fabs(velocity.x) > maxVelocity.x)
    {
        velocity.x = velocity.x > 0.f ? maxVelocity.x : -maxVelocity.x;
    }
    
    if (fabs(velocity.y) > maxVelocity.y)
    {
        velocity.y = velocity.y > 0.f ? maxVelocity.y : -maxVelocity.y;
    }
}

void C_Velocity::UpdatePosition(float deltaTime)
{
    sf::Vector2f frameMove = velocity * deltaTime;
    owner->transform->AddPosition(frameMove);
}


