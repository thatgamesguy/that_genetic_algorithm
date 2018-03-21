#include "C_KeyboardMovement.hpp"
#include "Object.hpp"

C_KeyboardMovement::C_KeyboardMovement(Object* owner) : Component(owner), moveSpeed(100) {}

void C_KeyboardMovement::Awake()
{
    sprite = owner->GetComponent<C_Sprite>();
}

void C_KeyboardMovement::SetInput(Input* input)
{
    this->input = input;
}

void C_KeyboardMovement::SetMovementSpeed(int moveSpeed)
{
    this->moveSpeed = moveSpeed;
}

void C_KeyboardMovement::Update(float deltaTime)
{
    
    if(input == nullptr)
    {
        return;
    }
    

    float angle = 0;
    int xMove = 0;
    if(input->IsKeyPressed(Input::Key::Left))
    {
        xMove = -moveSpeed;
        angle = 270;
    }
    else if(input->IsKeyPressed(Input::Key::Right))
    {
        xMove = moveSpeed;
        angle = 90;
    }
    
    int yMove = 0;
    if(input->IsKeyPressed(Input::Key::Up))
    {
        yMove = -moveSpeed;
        
        if(xMove > 0)
        {
            angle -= 45;
        }
        else if (xMove < 0)
        {
            angle += 45;
        }
    }
    else if(input->IsKeyPressed(Input::Key::Down))
    {
        yMove = moveSpeed;
        
        if(xMove > 0)
        {
            angle += 45;
        }
        else if (xMove < 0)
        {
            angle -= 45;
        }
        else
        {
            angle = 180;
        }
    }
    
    float xFrameMove = xMove * deltaTime;
    float yFrameMove = yMove * deltaTime;
    
    owner->transform->AddPosition(xFrameMove, yFrameMove);
    
    if(xMove != 0 || yMove != 0)
    {
        sprite->SetRotation(angle);
    }
}
