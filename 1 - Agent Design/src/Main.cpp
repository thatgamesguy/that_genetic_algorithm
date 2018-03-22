#include "Game.hpp"

int main()
{
    srand (static_cast <unsigned> (time(0)));
    
    Game game;
    
    while (game.IsRunning())
    {
        game.CaptureInput();
        game.Update();
        game.LateUpdate();
        game.Draw();
        game.CalculateDeltaTime();
    }
    
    return 0;
}
