#include <iostream>
#include "engine.h"

using namespace std;

void Boardgame::initBoard()
{
    for (int i = 1; i <= 5; ++ i) pawnTile[i] = 5;
    //pawnTile[5] = 0;
    for (int i = 7; i <= 11; ++ i) pawnTile[i] = 5;
    pawnTile[0] = 0; pawnTile[6] = 0;
    kingTile[0] = 1; kingTile[1] = 1;
    playerScore[0] = 0; playerScore[1] = 0;
    currentTurn = 0;
    lastScored = -10;
    cursorPos = 1;
    cursorLocker = false;
    hand = 0;
}

int Boardgame::getPawnTile(int i)
{
    if (pawnTile[i] < 0)
    {
        cout << "Error: Negative particle (Pawn) detected. Calculation checks are needed.\n";
        exit(1);
        return 0;
    }
    return pawnTile[i];
}

int Boardgame::getKingTile(int i)
{
    if (kingTile[i] != 0 && kingTile[i] != 1)
    {
        cout << "Error: Invalid particle (King) detected. Calculation checks are needed.\n";
        exit(1);
        return 0;
    }
    return kingTile[i];
}

int Boardgame::getScore(int i)
{
    return playerScore[i];
}

int Boardgame::getTurn()
{
    if (currentTurn != 0 && currentTurn != 1)
    {
        cout << "Error: Invalid (currentTurn) detected. Calculation checks are needed.\n";
        exit(1);
        return 0;
    }
    return currentTurn;
}

int Boardgame::getLastScored()
{
    return lastScored;
}

int Boardgame::getCursor()
{
    if ((currentTurn == 0 && (1 > cursorPos || cursorPos > 5)) || (currentTurn == 1 && (7 > cursorPos || cursorPos > 11)))
    {
        cout << "Error: Invalid (Cursor position) detected. Calculation checks are needed, especially initialization.\n";
        cout << "Current turn: " << currentTurn << "\nCurrent cursor position: " << cursorPos << "\n";
    }
    return cursorPos;
}

bool Boardgame::getLocker()
{
    return cursorLocker;
}

int Boardgame::getHand()
{
    return hand;
}

void Boardgame::pickPawn(int i, int val)
{
    if (pawnTile[i] - val >= 0) pawnTile[i] -= val;
    else
    {
        cout << "Something went wrong (pick pawn). Please check your calculations.\n";
        exit(1);
    }
}

void Boardgame::scoreUpdate(int val)
{
    playerScore[currentTurn] += val;
}

void Boardgame::moveCursor(int val)
{
    if (!cursorLocker)
    {
        if (currentTurn == 0)
        {
            if (1 <= cursorPos + val && cursorPos + val <= 5) cursorPos += val;
        }
        else
        {
            if (7 <= cursorPos - val && cursorPos - val <= 11) cursorPos -= val;
        }
    }
}

void Boardgame::lockCursor()
{
    cursorLocker = true;
}

void Boardgame::unlockCursor()
{
    cursorLocker = false;
}

void Boardgame::collectToHand(int val)
{
    hand += val;
}

int Boardgame::captureKing(int i)
{
    if (kingTile[i] != 0)
    {
        playerScore[currentTurn] += 10;
        kingTile[i] = 0;
        return 10;
    }
    return 0;
}

void Boardgame::endTurnUpdate(int val)
{
    lastScored = val;
    currentTurn = 1 - currentTurn;
    cursorPos = 1 + currentTurn * 10;
}

void Boardgame::endgameUpdate()
{
    for (int i = 1; i <= 5; ++ i)
    {
        playerScore[0] += pawnTile[i];
        pawnTile[i] = 0;
        playerScore[1] += pawnTile[i + 6];
        pawnTile[i + 6] = 0;
    }
}
