#ifndef C_DamageOnWallHit_hpp
#define C_DamageOnWallHit_hpp

#include "Component.hpp"
#include "C_GeneticAgent.hpp"

class C_DamageOnWallHit : public Component
{
public:
    C_DamageOnWallHit(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;
    
    void SetWindowSize(unsigned int width, unsigned int height);
    
private:
    std::shared_ptr<C_GeneticAgent> agent;
    
    unsigned int windowWidth;
    unsigned int windowHeight;
};

#endif /* C_DamageOnWallHit_hpp */
