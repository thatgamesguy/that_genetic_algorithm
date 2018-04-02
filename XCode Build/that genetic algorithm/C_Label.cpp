#include "C_Label.hpp"


C_Label::C_Label(Object* owner) : Component(owner)
{
    
}

void C_Label::SetFontAllocator(ResourceAllocator<sf::Font>* fontAllocator)
{
    this->fontAllocator = fontAllocator;
}

void C_Label::SetFont(const std::string& fontPath)
{
    if(fontAllocator)
    {
        int fontID = fontAllocator->Add(fontPath);
        
        if(fontID >= 0)
        {
            std::shared_ptr<sf::Font> font = fontAllocator->Get(fontID);
            label.setFont(*font);
            label.setFillColor(sf::Color::Black);
            label.setCharacterSize(22);
        }
    }
}

void C_Label::SetText(const std::string& text)
{
    label.setString(text);
}

void C_Label::SetPosition(const sf::Vector2f& pos)
{
    label.setPosition(pos);
}

void C_Label::Draw(Window& window)
{
    window.Draw(label);
}
