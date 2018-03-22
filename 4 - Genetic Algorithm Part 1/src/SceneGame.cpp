#include "SceneGame.hpp"

SceneGame::SceneGame(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator)
: workingDir(workingDir), textureAllocator(textureAllocator), geneticAlgorithm(objects, textureAllocator, workingDir) { }

void SceneGame::OnCreate()
{
    const int minX = 50;
    const int minY = 50;
    
    const int maxX = 750;
    const int maxY = 550;
    
    // Inlitialise Player
    player = std::make_shared<Object>();
    
    player->AddComponent<C_Velocity>();
    
    auto movement = player->AddComponent<C_KeyboardMovement>();
    movement->SetInput(&input);
    
    auto sprite = player->AddComponent<C_Sprite>();
    sprite->SetTextureAllocator(&textureAllocator);
    sprite->Load(workingDir.Get() + "playerShip.png");
    sprite->SetCenter(26.5f, 34.5f);
    
    const int randX = minX + (std::rand() % (maxX - minX + 1));
    const int randY = minY + (std::rand() % (maxY - minY + 1));
    player->transform->SetPosition(randX, randY);
    
    auto screenWrapAround = player->AddComponent<C_ScreenWrapAround>();
    screenWrapAround->SetSpriteHalfSize({26.5f, 35.5f});
    
    // As we are not adding player to our object collection we need to manually call the initilisation methods.
    player->Awake();
    player->Start();
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
    
    player->Update(deltaTime);
    objects.Update(deltaTime);
    geneticAlgorithm.Update(deltaTime);
}

void SceneGame::LateUpdate(float deltaTime)
{
    player->LateUpdate(deltaTime);
    objects.LateUpdate(deltaTime);
}

void SceneGame::Draw(Window& window)
{
    player->Draw(window);
    objects.Draw(window);
}
