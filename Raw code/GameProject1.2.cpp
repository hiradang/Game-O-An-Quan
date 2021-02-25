#include <bits/stdc++.h>
using namespace std;

struct movement{
    int position;
    int direction;
    // 1 = clockwise; -1 = counter-clockwise;
};

void initiate(int BOARDGAME[], int KING[]); // initiate the board game
void printGameBoard(const int BOARDGAME[],const int KING[]);    // print the board game
void printScore(const int SCORE[]); // print the score of players
void printWhileScattering(const int BOARDGAME[],const int KING[], const int stones); // print the board game after each step
movement getNext(const movement chosenTile); // get the position of the next tile
void renderFinalResult(const int BOARDGAME[], int SCORE[]);

movement select(int BOARDGAME[], const int PLAYER);  // get the player's choice
int collect(int BOARDGAME[], const movement chosenTile); // get the stones form the chosen tile to scatter and minus the stones in this tile
movement scatter(int BOARDGAME[],const int KING[], const int PLAYER, const movement chosenTile);
    // scatter until unable to scatter, get the lastTile for sum();
int sum(int BOARDGAME[], int KING[], movement lastTile); // get the sum if possible and returns 0 if impossible
void swapTurn(int &PLAYER);
bool scatterNewCondition(const int BOARDGAME[], const int PLAYER);
void scatterNew(int BOARDGAME[], const int KING[], const int PLAYER, int SCORE[]);
bool endGameCondition(const int BOARDGAME[], const int KING[]);
void gameLoop();

int main(){

    gameLoop();
    return 0;
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

void printGameBoard(const int BOARDGAME[], const int KING[]){
    system("cls");
    for (int i = 0; i<=6; i++)
        cout << setw(2) << BOARDGAME[i] << " ";
    cout << endl;
    cout << setw(2) << KING[0] << " ";
    for (int i = 11; i>=7; i--)
        cout << setw(2) << BOARDGAME[i] << " ";
    cout << setw(2) << KING[1] << endl << endl;

}

void printScore(const int SCORE[]){
    cout << "Player 1's score: " << SCORE[0] << endl;
    cout << "Player 2's score: " << SCORE[1] << endl << endl;
}

void printWhileScattering(const int BOARDGAME[], const int KING[], const int stones){
    printGameBoard(BOARDGAME, KING);
    cout << endl << "Number of stones you are scattering: " << stones <<"!" << endl;
    system("pause");
}

movement getNext(const movement chosenTile){
    movement nextTile;
    nextTile.position = (chosenTile.position + 12 + chosenTile.direction) % 12;
    nextTile.direction = chosenTile.direction;
    return nextTile;
}

void renderFinalResult(const int BOARDGAME[], int SCORE[]){
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

movement select(int BOARDGAME[], const int PLAYER){
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

int collect(int BOARDGAME[], const movement chosenTile){
    if (chosenTile.position == 0 || chosenTile.position == 6) { // if collecting from king tile, only get 1 stone
        BOARDGAME[chosenTile.position]--;
        return 1;
    }else{
        int stones = BOARDGAME[chosenTile.position];
        BOARDGAME[chosenTile.position] = 0;
        return stones;
    }
}

movement scatter(int BOARDGAME[], const int KING[], const int PLAYER, const movement chosenTile){
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

void swapTurn(int &PLAYER){
    PLAYER = 3 -PLAYER;
}

bool scatterNewCondition(const int BOARDGAME[], const int PLAYER){
    for (int i = 1; i<=5; i++){                     // PLAYER 1 : 1   2   3   4   5
        if ( BOARDGAME[i + (PLAYER-1) *6] )         // PLAYER 2 : 11  10  9   8   7
            return false;
    }
    return true;
}

void scatterNew(int BOARDGAME[], const int KING[], const int PLAYER, int SCORE[]){
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

bool endGameCondition(const int BOARDGAME[], const int KING[]){
    return ( !( BOARDGAME[0] + KING[0]) && !(BOARDGAME[6] + KING[1]) );
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
