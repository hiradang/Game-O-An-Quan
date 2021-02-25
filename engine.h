#ifndef ENGINE__H
#define ENGINE__H

class Boardgame
{
public:
    void initBoard();

    int getPawnTile(int i);
    int getKingTile(int i);
    int getScore(int i);
    int getTurn();
    int getLastScored();
    int getCursor();
    bool getLocker();
    int getHand();

    void pickPawn(int i, int val);
    void scoreUpdate(int val);
    void moveCursor(int val);
    void lockCursor();
    void unlockCursor();
    void collectToHand(int val);
    int captureKing(int i);
    void endTurnUpdate(int val);
    void endgameUpdate();
private:
    int pawnTile[12];
    int kingTile[2];
    int playerScore[2];
    int currentTurn;
    int lastScored;
    int cursorPos;
    bool cursorLocker;
    int hand;
};

#endif // ENGINE__H
