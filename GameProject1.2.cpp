#include <bits/stdc++.h>
using namespace std;

struct movement{
    int position;
    int direction;
    // 1 = clockwise; -1 = counter-clockwise;
};

void initiate(int BOARDGAME[], int KING[]); // initiate the board game
void printGameBoard(int BOARDGAME[],int KING[]);    // print the board game
void printScore(int SCORE[]);
void printWhileScattering(int BOARDGAME[],int KING[], int stones);
movement getNext(movement chosenTile); // get the position of the next tile
void renderFinalResult(int BOARDGAME[], int SCORE[]);

movement select(int BOARDGAME[], int PLAYER);  // get the player's choice
int collect(int BOARDGAME[], movement chosenTile); // get the stones form the chosen tile to scatter and minus the stones in this tile
movement scatter(int BOARDGAME[],int KING[], int PLAYER, movement chosenTile);
int sum(int BOARDGAME[], int KING[], movement lastTile);
void swapTurn(int &PLAYER);
bool scatterNewCondition(int BOARDGAME[], int PLAYER);
void scatterNew(int BOARDGAME[], int KING[], int PLAYER, int SCORE[]);
bool endGameCondition(int BOARDGAME[], int KING[]);
void gameLoop();

int main(){

    gameLoop();
    return 0;
}

void gameLoop(){
    int BOARDGAME[12];  // 12 ordinary tiles
    int KING[2]; // 2 tile of King
    int SCORE[2] = {};
    int PLAYER = 1; // player 1 will play first

    initiate(BOARDGAME, KING);
    printGameBoard(BOARDGAME, KING);
    printScore(SCORE);

    while(!endGameCondition(BOARDGAME, KING)){
        scatterNew(BOARDGAME, KING, PLAYER, SCORE);
        movement chosenTile = select(BOARDGAME, PLAYER);
        movement nextTile = scatter(BOARDGAME, KING, PLAYER, chosenTile);
        SCORE[PLAYER-1] += sum(BOARDGAME, KING, nextTile);
        swapTurn(PLAYER);
        printGameBoard(BOARDGAME, KING); printScore(SCORE);
    }
    renderFinalResult(BOARDGAME, SCORE);
}

void initiate(int BOARDGAME[], int KING[]){
    for (int i = 0; i<12; i++) BOARDGAME[i] = 5;
    BOARDGAME[0] = 0;
    BOARDGAME[6] = 0;
    KING[0] = 1; KING[1] = 1; // the presence of king
    // board game will look like this
    // 0   1   2   3   4   5   6             0  1  1  1  1  1  0
    // K  11  10   9   8   7   K             1  1  1  1  1  1  1
}

void printGameBoard(int BOARDGAME[], int KING[]){
    system("cls");
    for (int i = 0; i<=6; i++)
        cout << setw(2) << BOARDGAME[i] << " ";
    cout << endl;
    cout << setw(2) << KING[0] << " ";
    for (int i = 11; i>=7; i--)
        cout << setw(2) << BOARDGAME[i] << " ";
    cout << setw(2) << KING[1] << endl << endl;

}

void printScore(int SCORE[]){
    cout << "Player 1's score: " << SCORE[0] << endl;
    cout << "Player 2's score: " << SCORE[1] << endl << endl;
}

movement getNext(movement chosenTile){
    movement nextTile;
    nextTile.position = (chosenTile.position + 12 + chosenTile.direction) % 12;
    nextTile.direction = chosenTile.direction;
    return nextTile;
}

int collect(int BOARDGAME[], movement chosenTile){
    if (chosenTile.position == 0 || chosenTile.position == 6) { // if collecting from king tile, only get 1 stone
        BOARDGAME[chosenTile.position]--;
        return 1;
    }else{
        int stones = BOARDGAME[chosenTile.position];
        BOARDGAME[chosenTile.position] = 0;
        return stones;
    }
}

movement select(int BOARDGAME[], int PLAYER){
    movement chosenTile;
    cout << "Player " << PLAYER << "'s turn!" << endl;
    cout << "Please choose a tile in the row " << PLAYER << ": (1 <= N <= 5)" << endl;
    while (true){
        cin >> chosenTile.position;
        if (chosenTile.position <1 || chosenTile.position >5){
            cout << "Your position is invalid! Please re-enter!: " << endl;
        }
        else{
            if (PLAYER == 2) chosenTile.position = 12 - chosenTile.position;

            if(!BOARDGAME[chosenTile.position])
                cout << "This position is empty! Unable to scatter! Please re-enter!";
            else break;
        }
    }

    cout << "Do you want to go clockwise or counter-clockwise?" << endl;
    cout << "1 = clockwise, -1 = counter-clockwise" << endl;
    cin >> chosenTile.direction;
    while (abs(chosenTile.direction) != 1){
        cout << "Your direction is invalid! Please re-enter! " << endl;
        cin >> chosenTile.direction;
    }
    return chosenTile;
}

void swapTurn(int &PLAYER){
    PLAYER = 3 -PLAYER;
}

bool scatterNewCondition(int BOARDGAME[], int PLAYER){
    for (int i = 1; i<=5; i++){                     // PLAYER 1 : 1   2   3   4   5
        if ( BOARDGAME[i + (PLAYER-1) *6] )         // PLAYER 2 : 11  10  9   8   7
            return false;
    }
    return true;
}

void scatterNew(int BOARDGAME[], int KING[], int PLAYER, int SCORE[]){
    if (scatterNewCondition(BOARDGAME, PLAYER)){
        for (int i = 1; i<=5; i++)
            BOARDGAME[i + (PLAYER-1) *6]++;
        SCORE[PLAYER-1]-=5;
        printGameBoard(BOARDGAME, KING);
        cout << "You have just scattered because all of your tiles are empty!" << endl;
        system("pause");
    }
    else return;
}

bool endGameCondition(int BOARDGAME[], int KING[]){
    return ( !( BOARDGAME[0] + KING[0]) && !(BOARDGAME[6] + KING[1]) );
}

void renderFinalResult(int BOARDGAME[], int SCORE[]){
    // sum up the remaining stones on the board
    for (int i = 1; i<=5; i++){
        SCORE[0] += BOARDGAME[i];
        SCORE[1] += BOARDGAME[i + 6];
    }

    // render the result
    printScore(SCORE);

    if (SCORE[0] > SCORE[1]) cout << "Player 1 wins! Congratulations!" << endl;
    else if (SCORE[0] < SCORE[1]) cout << "Player 2 wins! Congratulations!" << endl;
    else cout << "It's a draw!" << endl;
}

movement scatter(int BOARDGAME[], int KING[], int PLAYER, movement chosenTile){
    movement nextTile = chosenTile;
    do{
        int stones = collect(BOARDGAME, nextTile);
        printWhileScattering(BOARDGAME, KING, stones);
        while(stones){
            nextTile = getNext(nextTile);
            BOARDGAME[nextTile.position]++;
            stones--;
        printWhileScattering(BOARDGAME, KING, stones);
        }
        nextTile = getNext(nextTile);
    } while(BOARDGAME[nextTile.position]);
    return nextTile; // the tile after the last tile of scattering
}

int sum(int BOARDGAME[], int KING[], movement lastTile){ // second next Tile after the last tile of scattering
    int point = 0;
    movement nextTile = lastTile;
    while (true){
        nextTile = getNext(nextTile);
        if (nextTile.position == 0 && !KING[0] && BOARDGAME[0] == 0) break;
		if (nextTile.position == 6 && !KING[1] && BOARDGAME[6] == 0) break;
		if (nextTile.position != 0 && nextTile.position != 6 && BOARDGAME[nextTile.position] == 0) break;
		if (nextTile.position == 0 && KING[0])
		{
			point += 10;
			KING[0] = 0;
		}
		if (nextTile.position == 6 && KING[1])
		{
			point += 10;
			KING[1] = 0;
		}
		point += BOARDGAME[nextTile.position];
		BOARDGAME[nextTile.position] = 0;
		nextTile = getNext(nextTile);
		if (BOARDGAME[nextTile.position] != 0) break;
	}
    if (point) cout << "You've earned " << point <<"!" << endl;
    system("pause");
    return point;
}


void printWhileScattering(int BOARDGAME[], int KING[], int stones){
    printGameBoard(BOARDGAME, KING);
    cout << endl << "Number of stones you are scattering: " << stones <<"!" << endl;
    system("pause");
}
