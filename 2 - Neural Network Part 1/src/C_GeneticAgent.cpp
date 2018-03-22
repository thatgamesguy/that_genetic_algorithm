#include "C_GeneticAgent.hpp"
#include "Object.hpp"

float C_GeneticAgent::sightRadius(30.f);

C_GeneticAgent::C_GeneticAgent(Object* owner) : Component(owner), maxMoveForce(100.f), energy(60.f), energyReductionMultiplier(2.f)
{
    
}

void C_GeneticAgent::Awake()
{
    velocity = owner->GetComponent<C_Velocity>();
}

void C_GeneticAgent::Update(float deltaTime)
{
    const int minX = -1;
    const int maxX = 1;
    
    float moveX = (minX + (std::rand() % (maxX - minX + 1)));
    float moveY = (minX + (std::rand() % (maxX - minX + 1)));
    
    const sf::Vector2f move = sf::Vector2f(moveX, moveY) * maxMoveForce * deltaTime;
    
    velocity->Add(move);
    
    float energyReduction = (fabs(moveX) + fabs(moveY)) * energyReductionMultiplier * deltaTime;
    energy -= energyReduction;
}

void C_GeneticAgent::SetEnergy(float amount)
{
    energy = amount;
}

float C_GeneticAgent::GetEnergy()
{
    return energy;
}

float C_GeneticAgent::GetSightRadius()
{
    return sightRadius;
}

