#include "SceneGame.hpp"

SceneGame::SceneGame(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator)
: workingDir(workingDir), textureAllocator(textureAllocator) { }

void SceneGame::OnCreate()
{
    const int minX = 50;
    const int minY = 50;
    
    const int maxX = 750;
    const int maxY = 550;
    
    // Initialise UFOs
    const std::string spritePreName = "ufo";
    const std::string spritePostName = ".png";
    
    for (int i = 0; i < 30; i++)
    {
        std::shared_ptr<Object> ufo = std::make_shared<Object>();
        
        auto sprite = ufo->AddComponent<C_Sprite>();
        sprite->SetTextureAllocator(&textureAllocator);
        const std::string ufoCount = std::to_string(1 + (std::rand() % (4 - 1 + 1)));
        sprite->Load(workingDir.Get() + spritePreName + ufoCount + spritePostName);
        
        const int randX = minX + (std::rand() % (maxX - minX + 1));
        const int randY = minY + (std::rand() % (maxY - minY + 1));
        ufo->transform->SetPosition(randX, randY);
        
        objects.Add(ufo);
    }
    
    // Inlitialise Player
    std::shared_ptr<Object> player = std::make_shared<Object>();
    
    auto movement = player->AddComponent<C_KeyboardMovement>();
    movement->SetInput(&input);
    
    auto sprite = player->AddComponent<C_Sprite>();
    sprite->SetTextureAllocator(&textureAllocator);
    sprite->Load(workingDir.Get() + "playerShip.png");
    sprite->SetCenter(34.5f, 26.5f);
    
    const int randX = minX + (std::rand() % (maxX - minX + 1));
    const int randY = minY + (std::rand() % (maxY - minY + 1));
    player->transform->SetPosition(randX, randY);
    
    objects.Add(player);
}

void SceneGame::OnDestroy()
{
    
}

void SceneGame::ProcessInput()
{
    input.Update();
}

void SceneGame::Update(float deltaTime)
{
    objects.ProcessRemovals();
    objects.ProcessNewObjects();
    
    objects.Update(deltaTime);
}

void SceneGame::LateUpdate(float deltaTime)
{
    objects.LateUpdate(deltaTime);
}

void SceneGame::Draw(Window& window)
{
    objects.Draw(window);
}
