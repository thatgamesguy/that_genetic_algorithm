#include "C_DamageOnWallHit.hpp"
#include "C_GeneticAgent.hpp"

C_DamageOnWallHit::C_DamageOnWallHit(Object* owner) : Component(owner), windowSize(1920, 1080) {}

void C_DamageOnWallHit::Awake()
{
    agent = owner->GetComponent<C_GeneticAgent>();
}

void C_DamageOnWallHit::Update(float deltaTime)
{
    const sf::Vector2f& pos = owner->transform->GetPosition();
    const float radius = agent->GetAgentRadius();
    const float halfRadius = radius * 0.5f;
    
    if(pos.x <= radius || pos.x >= windowSize.x - halfRadius || pos.y <= radius || pos.y >= windowSize.y - halfRadius)
    {
        agent->SetEnergy(0.f);
    }
}

void C_DamageOnWallHit::SetWindowSize(const sf::Vector2u& windowSize)
{
    this->windowSize = windowSize;
}
