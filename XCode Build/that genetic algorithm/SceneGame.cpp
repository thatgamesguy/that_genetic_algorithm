#include "SceneGame.hpp"

SceneGame::SceneGame(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window)
: workingDir(workingDir), textureAllocator(textureAllocator), geneticAlgorithm(objects, textureAllocator, workingDir, window) { }

void SceneGame::OnCreate()
{

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
    objects.Update(deltaTime);
    geneticAlgorithm.Update(deltaTime);
}

void SceneGame::LateUpdate(float deltaTime)
{
    objects.LateUpdate(deltaTime);
    
    objects.ProcessRemovals();
    objects.ProcessNewObjects();
}

void SceneGame::Draw(Window& window)
{
    objects.Draw(window);
}
