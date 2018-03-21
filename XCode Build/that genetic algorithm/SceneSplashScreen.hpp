#ifndef SceneSplashScreen_hpp
#define SceneSplashScreen_hpp

#include <SFML/Graphics.hpp>

#include "SceneStateMachine.hpp"
#include "WorkingDirectory.hpp"
#include "ResourceAllocator.hpp"

class SceneSplashScreen : public Scene
{
public:
    SceneSplashScreen(WorkingDirectory& workingDir, SceneStateMachine& sceneStateMachine, Window& window, ResourceAllocator<sf::Texture>& textureAllocator);
    
    void OnCreate() override;
    void OnDestroy() override;
    
    void OnActivate() override;
    
    void SetSwitchToScene(unsigned int id);
    
    void Update(float deltaTime) override;
    void Draw(Window& window) override;
    
private:
    sf::Texture splashTexture;
    sf::Sprite splashSprite;
    
    WorkingDirectory& workingDir;
    SceneStateMachine& sceneStateMachine;
    Window& window;
    
    float currentSeconds;
    float showForSeconds;
    
    unsigned int switchToState;
    
    ResourceAllocator<sf::Texture>& textureAllocator;
};

#endif /* SceneSplashScreen_hpp */
