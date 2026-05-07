#include "Core/Game.h"

int main()
{
    Game* pGame = new Game();
    pGame->letsgo();
    delete pGame;
    return 0;
}