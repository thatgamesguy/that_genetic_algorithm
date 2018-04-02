#ifndef C_Label_hpp
#define C_Label_hpp

#include "Component.hpp"
#include "ResourceAllocator.hpp"
#include "Window.hpp"

class C_Label : public Component
{
public:
    C_Label(Object* owner);
    
    void SetFontAllocator(ResourceAllocator<sf::Font>* fontAllocator);
    
    void SetFont(const std::string& fontPath);
    
    void SetText(const std::string& text);
    
    void SetPosition(const sf::Vector2f& pos);
    
    void Draw(Window& window) override;
    
private:
    ResourceAllocator<sf::Font>* fontAllocator;
    sf::Text label;
};

#endif /* C_Label_hpp */
