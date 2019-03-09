#ifndef C_ScreenWrapAround_hpp
#define C_ScreenWrapAround_hpp

#include "Component.hpp"

class C_ScreenWrapAround : public Component
{
public:
    C_ScreenWrapAround(Object* owner);
    
    void SetSpriteHalfSize(const sf::Vector2f& spriteSize);
    
    void SetScreenSize(const sf::Vector2u& screenSize);
    
    void LateUpdate(float deltaTime) override;
    
private:
    sf::Vector2u screenSize;
    sf::Vector2f spriteHalfSize;
};

#endif /* C_ScreenWrapAround.hpp */
