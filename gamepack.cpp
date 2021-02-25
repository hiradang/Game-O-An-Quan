#include "const_list.h"
#include "gamepack.h"
#include <vector>
#include <time.h>

void waitUntilSpacePressed()
{
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYUP || e.type == SDL_QUIT))
        {
            if (e.type == SDL_QUIT)
            {
                cout << "Exit by SDL_QUIT\n";
                exit(0);
            }
            if (e.key.keysym.sym == SDLK_SPACE) return;
            if (e.key.keysym.sym == SDLK_ESCAPE) return;
        }
    }
}

movement getNext(const movement currentMove){
    movement nextMove;
    nextMove.position = (currentMove.position + 12 + currentMove.direction) % 12;
    nextMove.direction = currentMove.direction;
    return nextMove;
}

void GamePack::initGame(int id)
{
    modeID = id;
    vSDL.initAll(OBJECT_PATH, FONT_PATH, 32, 20);
    mBoard.initBoard();
    vSDL.drawAll(mBoard);
}

void GamePack::pickFromTile(int i, int val, int delay, int id)
{
    mBoard.pickPawn(i, val);
    switch (id)
    {
        case 1: //collect to hand
        {
            mBoard.collectToHand(val);
            break;
        }
        case 2: //collect to points
        {
            mBoard.scoreUpdate(val);
            break;
        }
        default: break;
    }
    SDL_Delay(delay);
    vSDL.drawAll(mBoard);
}

bool GamePack::endgameCondition()
{
    return !(mBoard.getKingTile(0) || mBoard.getPawnTile(0) || mBoard.getKingTile(1) || mBoard.getPawnTile(6));
}

void GamePack::scatterNew()
{
    int a = mBoard.getTurn() * 6;
    for (int i = 1; i <= 5; ++ i)
    {
        if (mBoard.getPawnTile(i + a) > 0) return;
    }
    cout << "Scattered some\n";
    for (int i = 1; i <= 5; ++ i)
    {
        pickFromTile(i + a, -1, SCATTERNEW_DELAY, 2);
    }
}

movement GamePack::humanSelect()
{
    movement nextMove;
    nextMove.direction = 1;
    if (mBoard.getTurn() == 0) nextMove.position = 1;
    else nextMove.position = 11;
    SDL_Event mEvent;
    bool quit = false, qpos = false, qdir = false;
    while (!quit)
    {
        while (!qpos) // choose position
        {
            while (SDL_PollEvent(&mEvent) != 0)
            {
                if (mEvent.type == SDL_QUIT)
                {
                    cout << "Exit by SDL_QUIT\n";
                    exit(0);
                }
                if (mEvent.type == SDL_KEYUP)
                    switch (mEvent.key.keysym.sym)
                    {
                        case (SDLK_RETURN): case (SDLK_q):
                        {
                            nextMove.position = mBoard.getCursor();
                            if (mBoard.getPawnTile(nextMove.position) != 0)
                            {
                                mBoard.lockCursor(); qpos = true;
                            }
                            else cout << "This tile has nothing to pick: " << nextMove.position << "\n";
                            break;
                        }
                        case (SDLK_RIGHT): case (SDLK_d):
                        {
                            mBoard.moveCursor(1);
                            vSDL.drawAll(mBoard);
                            break;
                        }
                        case (SDLK_LEFT): case (SDLK_a):
                        {
                            mBoard.moveCursor(-1);
                            vSDL.drawAll(mBoard);
                            break;
                        }
                        case (SDLK_ESCAPE):
                        {
                            escGameLoop = true;
                            return nextMove;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
            }
        }
        vSDL.drawAll(mBoard);
        while (!qdir) // choose direction
        {
            while (SDL_PollEvent(&mEvent) != 0)
            {
                if(mEvent.type == SDL_QUIT)
                {
                    cout << "Exit by SDL_QUIT\n";
                    exit(0);
                }
                if (mEvent.type == SDL_KEYUP)
                    switch (mEvent.key.keysym.sym)
                    {
                        case (SDLK_RETURN): case (SDLK_q):
                        {
                            mBoard.unlockCursor(); qpos = false;
                            vSDL.drawAll(mBoard);
                            cout << "Reselecting movement ...\n";
                            break;
                        }
                        case (SDLK_RIGHT): case (SDLK_d):
                        {
                            if (mBoard.getTurn() == 0) nextMove.direction = 1; else nextMove.direction = -1;
                            mBoard.unlockCursor(); qdir = true;
                            break;
                        }
                        case (SDLK_LEFT): case (SDLK_a):
                        {
                            if (mBoard.getTurn() == 0) nextMove.direction = -1; else nextMove.direction = 1;
                            mBoard.unlockCursor(); qdir = true;
                            break;
                        }
                        case (SDLK_ESCAPE):
                        {
                            escGameLoop = true;
                            return nextMove;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
            }
            if (!qpos) break;
        }
        if (qpos && qdir) quit = true;
    }
    return nextMove;
}

movement GamePack::AISelect()
{
    movement nextMove = {0, 0};
    SDL_Delay(1000);

    vector <int> potentialPawnTile;
    for (int i = 1; i<=5; i++){
        if (mBoard.getPawnTile(i + 6*mBoard.getTurn()))
        potentialPawnTile.push_back(i + 6*mBoard.getTurn());
    }
    srand(time(0));
    nextMove.position = potentialPawnTile[rand() % potentialPawnTile.size()];

    // update cursorPos
    if (!mBoard.getTurn()) mBoard.moveCursor(nextMove.position - mBoard.getCursor());
        else mBoard.moveCursor(mBoard.getCursor() - nextMove.position);
    srand(time(0));
    nextMove.direction = (rand() % 2)? 1: -1;
    return nextMove;
}

movement GamePack::selectMovement()
{
    movement nextMove = {0, 0};
    if ((mBoard.getTurn() == 0 && (modeID == 0 || modeID == 1)) || (mBoard.getTurn() == 1 && (modeID == 0 || modeID == 2))) nextMove = humanSelect();
    else
    {
        nextMove = AISelect();
        //cout << "This is where AI code goes\n";
    }
    if ((mBoard.getTurn() == 0 && (1 > nextMove.position || nextMove.position > 5)) ||
        (mBoard.getTurn() == 1 && (7 > nextMove.position || nextMove.position > 11)))
    {
        cout << "Illegal movement. Please check your code (probably AI fault)\n";
        exit(1);
    }
    if (mBoard.getPawnTile(nextMove.position) == 0)
    {
        cout << "Illegal movement. Please check your code (probably AI fault)\n";
        exit(1);
    }
    return nextMove;
}

movement GamePack::scatterMove(movement &currentMove)
{
    movement nextMove = currentMove;
    int avai = mBoard.getPawnTile(currentMove.position);
    while (avai != 0)
    {
        if (nextMove.position == 0 || nextMove.position == 6) pickFromTile(nextMove.position, 1, SCATTER_DELAY, 1);
        else pickFromTile(nextMove.position, avai, COLLECT_DELAY, 1);
        while (mBoard.getHand() > 0)
        {
            nextMove = getNext(nextMove);
            pickFromTile(nextMove.position, -1, SCATTER_DELAY, 1);
        }
        nextMove = getNext(nextMove);
        avai = mBoard.getPawnTile(nextMove.position);
    }
    return nextMove;
}

int GamePack::sumEarned(movement &currentMove)
{
    int earned = 0;
    movement nextMove = currentMove;
    while (true)
    {
        if (mBoard.getPawnTile(nextMove.position) > 0) break;
        if (nextMove.position == 0 && mBoard.getKingTile(0) == 1) break;
        if (nextMove.position == 6 && mBoard.getKingTile(1) == 1) break;

        nextMove = getNext(nextMove);
        int p = mBoard.getPawnTile(nextMove.position);
        if (nextMove.position == 0 && p == 0 && mBoard.getKingTile(0) == 0) break;
        if (nextMove.position == 6 && p == 0 && mBoard.getKingTile(1) == 0) break;
        if (nextMove.position != 0 && nextMove.position != 6 && p == 0) break;
        for (int i = 0; i < 2; ++ i)
        {
            if (nextMove.position == i * 6)
            {
                earned += mBoard.captureKing(i);
            }
        }
        pickFromTile(nextMove.position, p, SCORE_DELAY, 2);
        earned += p;
        nextMove = getNext(nextMove);
    }
    return earned;
}

void GamePack::turnChange(int earned)
{
    mBoard.endTurnUpdate(earned);
    vSDL.drawAll(mBoard);
}

void GamePack::endResult()
{
    mBoard.endgameUpdate();
    vSDL.drawEndgame(mBoard);
    waitUntilSpacePressed();
}

void GamePack::runGame(int id)
{
    initGame(id);
    while (!endgameCondition())
    {
        scatterNew();
        movement nextMove = selectMovement();
        if (escGameLoop) return;
        nextMove = scatterMove(nextMove);
        int earned = sumEarned(nextMove);
        turnChange(earned);
    }
    endResult(); // hidden: you can use the esc to exit the "Game Over" screen
    SDL_Delay(1000);
}

void GamePack::tutorial()
{
    vSDL.initTutor();
    bool quit = false;
    int page = 0;
    SDL_Event mEvent;
    while (!quit)
    {
        vSDL.drawTutor(page);
        //cout << page << "\n";
        while (SDL_PollEvent(&mEvent) != 0)
        {
            if (mEvent.type == SDL_QUIT)
            {
                cout << "Exit by SDL_QUIT\n";
                exit(0);
            }
            if (mEvent.type == SDL_KEYUP)
            {
                switch (mEvent.key.keysym.sym)
                {
                    case SDLK_RIGHT:
                    case SDLK_d:
                    {
                        if (page + 1 < NUM_OF_TUTOR_PAGE) ++ page;
                        break;
                    }
                    case SDLK_LEFT:
                    case SDLK_a:
                    {
                        if (page - 1 >= 0) -- page;
                        break;
                    }
                    case SDLK_ESCAPE:
                    case SDLK_RETURN:
                    case SDLK_q:
                    case SDLK_SPACE:
                    {
                        quit = true;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
    }
}

int GamePack::chooseGameMode(string menuPath)
{
    vSDL.drawMenu(menuPath);
    SDL_Event mEvent;
    bool quit = false;
    int chosen = -1;
    while (!quit)
    {
        while (SDL_PollEvent(&mEvent) != 0)
        {
            if (mEvent.type == SDL_QUIT)
            {
                cout << "Exit by SDL_QUIT\n";
                exit(0);
            }
            if (mEvent.type == SDL_KEYUP)
            {
                switch (mEvent.key.keysym.sym)
                {
                    case SDLK_0:
                    {
                        chosen = 0;
                        quit = true;
                        break;
                    }
                    case SDLK_1:
                    {
                        chosen = 1;
                        quit = true;
                        break;
                    }
                    case SDLK_2:
                    {
                        chosen = 2;
                        quit = true;
                        break;
                    }
                    case SDLK_3:
                    {
                        chosen = 3;
                        quit = true;
                        break;
                    }
                    case SDLK_8:
                    {
                        chosen = 8;
                        quit = true;
                        break;
                    }
                    case SDLK_ESCAPE:
                    {
                        cout << "Exit by ESCAPE from main MENU\n";
                        exit(0);
                        break;
                    }
                    default:
                    {
                        //return -1;
                        break;
                    }
                }
            }
        }
    }
    cout << chosen;
    return chosen;
}


