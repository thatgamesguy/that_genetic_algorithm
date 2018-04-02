#include "C_KeyboardMovement.hpp"
#include "Object.hpp"

C_KeyboardMovement::C_KeyboardMovement(Object* owner) : Component(owner), moveForce(800.f) {}

void C_KeyboardMovement::Awake()
{
    velocity = owner->GetComponent<C_Velocity>();
}

void C_KeyboardMovement::SetInput(Input* input)
{
    this->input = input;
}

void C_KeyboardMovement::Update(float deltaTime)
{
    sf::Vector2f move(0.f, 0.f);
    
    if(input->IsKeyPressed(Input::Key::Left))
    {
        move.x -= moveForce * deltaTime;
    }
    else if(input->IsKeyPressed(Input::Key::Right))
    {
        move.x += moveForce * deltaTime;
    }
    
    if(input->IsKeyPressed(Input::Key::Up))
    {
        move.y -= moveForce * deltaTime;
    }
    else if(input->IsKeyPressed(Input::Key::Down))
    {
        move.y += moveForce * deltaTime;
    }

    velocity->Add(move);

}

