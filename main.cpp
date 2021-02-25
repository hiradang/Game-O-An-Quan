/*
Description:
-   Now included "Tutorial"
*/

#include "const_list.h"
#include "draw.h"
#include "engine.h"
#include "gamepack.h"

using namespace std;

int main(int argc, char* argv[])
{
    GamePack mGame;
    bool quit = false;
    while (!quit)
    {
        int mode = mGame.chooseGameMode(MENU_PATH);
        switch (mode)
        {
            case -1:
            {
                quit = true;
                break;
            }
            case 0:
            case 1:
            case 2:
            case 3:
            {
                mGame.runGame(mode);
                break;
            }
            case 8:
            {
                mGame.tutorial();
                break;
            }
            default:
            {
                cout << "Invalid (Selection) detected.\n";
                break;
            }
        }
        // 0: PvP - 1: PvAI, Player goes first - 2: PvAI, AI goes first - 3: AIvAI (maybe we won't use this, but it might be fun)
    }
    return 0;
}
