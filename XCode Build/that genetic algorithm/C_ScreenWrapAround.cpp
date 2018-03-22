#include "C_ScreenWrapAround.hpp"
#include "Object.hpp"

C_ScreenWrapAround::C_ScreenWrapAround(Object* owner) : Component(owner), screenSize(800.f, 600.f), spriteHalfSize(0.f, 0.f) {}

void C_ScreenWrapAround::SetSpriteHalfSize(const sf::Vector2f& spriteHalfSize)
{
    this->spriteHalfSize = spriteHalfSize;
}

void C_ScreenWrapAround::SetScreenSize(const sf::Vector2f& screenSize)
{
    this->screenSize = screenSize;
}

void C_ScreenWrapAround::LateUpdate(float deltaTime)
{
    const sf::Vector2f& pos = owner->transform->GetPosition();
    
    sf::Vector2f newPos = pos;
    
    if(pos.x < -spriteHalfSize.x)
    {
        newPos.x = screenSize.x + spriteHalfSize.x;
    }
    else if(pos.x > screenSize.x + spriteHalfSize.x)
    {
        newPos.x = -spriteHalfSize.x;
    }
    
    if(pos.y < -spriteHalfSize.y)
    {
        newPos.y = screenSize.y + spriteHalfSize.y;
    }
    else if (newPos.y > screenSize.y + spriteHalfSize.y)
    {
        newPos.y = -spriteHalfSize.y;
    }
    
    owner->transform->SetPosition(newPos);
}
