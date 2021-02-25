#ifndef GAMEPACK__H
#define GAMEPACK__H

#include "engine.h"
#include "draw.h"

struct movement
{
    int position = 0;
    int direction = 0;
};

class GamePack
{
public:
    void runGame(int id);
    void tutorial();
    int chooseGameMode(string menuPath);
private:
    Boardgame mBoard;
    VisualMaker vSDL;
    int modeID;
    bool escGameLoop = false;

    void initGame(int id);
    void pickFromTile(int i, int val, int delay, int id);
    bool endgameCondition();
    void scatterNew();
    movement humanSelect();
    movement AISelect();
    movement selectMovement();
    movement scatterMove(movement &currentMove);
    int sumEarned(movement &currentMove);
    void turnChange(int earned);
    void endResult();
};

#endif // GAMEPACK__H
