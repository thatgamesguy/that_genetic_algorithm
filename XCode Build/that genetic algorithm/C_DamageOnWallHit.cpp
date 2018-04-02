#include "C_DamageOnWallHit.hpp"
#include "C_GeneticAgent.hpp"

C_DamageOnWallHit::C_DamageOnWallHit(Object* owner) : Component(owner), windowWidth(800), windowHeight(600) {}

void C_DamageOnWallHit::Awake()
{
    agent = owner->GetComponent<C_GeneticAgent>();
}

void C_DamageOnWallHit::Update(float deltaTime)
{
    const sf::Vector2f& pos = owner->transform->GetPosition();
    const float radius = agent->GetAgentRadius();
    const float halfRadius = radius * 0.5f;
    
    if(pos.x <= radius || pos.x >= windowWidth - halfRadius || pos.y <= radius || pos.y >= windowHeight - halfRadius)
    {
        agent->SetEnergy(0.f);
    }
}

void C_DamageOnWallHit::SetWindowSize(unsigned int width, unsigned int height)
{
    windowWidth = width;
    windowHeight = height;
}
