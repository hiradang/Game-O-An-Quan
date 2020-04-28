#include <bits/stdc++.h>
using namespace std;
int a[14];
// 0 1 2 3 4 5 6
// 13 12 11 .....
int player = 0; // player 1 will play first. This variable indicates the player's turn.
int scorePlayer[2] = {};

struct coordinate{
    int position;
    int direction;
    // right = 1 = clockwise
    // left = -1 = counter-clockwise
};

void initiate();
coordinate select();
void update();
coordinate getNextPosition(coordinate currentPosition);
void exchange();
int collectFromChosenPosition(coordinate chosenPosition);
void sumEndGame();
bool endGameCondition();
void scatterNew();
bool checkScatterNew();
void scatter(coordinate chosenPosition);
void sum(coordinate chosenPosition);
void gameLoop();
void renderFinalResult();
void printWhileScattering(int numberOfStones);

int main(){
    initiate();
    update();
    gameLoop();
    sumEndGame();
    renderFinalResult();
    return 0;
}

void initiate(){
    for (int i = 1; i<=12; i++){
        if (i!= 6 && i !=7) a[i] = 5;
    }
    a[0] = 0; a[6] = 0;
    a[13] = 1; a[7] = 1; // 1 presents for king
}

coordinate select(){
    coordinate chosenPosition;
    int n;
    cout << endl;
    cout << "Player " << player+1 << "'s turn!" << endl;
    cout << "Please choose a cell in the row " << player +1 << endl;
    cout << "Enter the cell: 1<=n <=5 " << endl;
    cin >> n;
    if (player ==0) chosenPosition.position = n;
    else chosenPosition.position = 13-n;

    while (n <1 || n >5 || a[chosenPosition.position] == 0){ // examine the validity of position
        if (n <1 || n >5)
            cout << "Your position is invalid! Please re-enter! " << endl;
        else cout << "This cell is empty! Please re-enter! " << endl;
        cin >> n;
        if (player ==0) chosenPosition.position = n;
        else chosenPosition.position = 13-n;
    }
    cout << "Do you want to go clockwise or counter-clockwise?" << endl;
    cout << "1 = clockwise, -1 = counter-clockwise" << endl;
    cin >> chosenPosition.direction;
    while (abs(chosenPosition.direction) != 1){
        cout << "Your direction is invalid! Please re-enter! " << endl;
        cin >> chosenPosition.direction;
    }
    return chosenPosition;
}

void update(){
    system("cls");
    for (int i = 0; i<=6; i++){
        cout << setw(2) << a[i] << " ";
    }
    cout << endl;
    for (int i = 13; i>=7; i--){
        cout << setw(2) << a[i] << " ";
    }
    cout << endl << endl;

    cout << "Player 1's score: " << scorePlayer[0] << endl;
    cout << "Player 2's score: " << scorePlayer[1] << endl;
}

coordinate getNextPosition(coordinate currentPosition){
    coordinate nextPosition;
    nextPosition.direction = currentPosition.direction;
    nextPosition.position = (currentPosition.position + 14 + currentPosition.direction)%14;

    if (nextPosition.position == 7 || nextPosition.position == 13){
        nextPosition.position = (nextPosition.position + 14 + currentPosition.direction)%14;
        // avoid the position of king =)) get the next position;
    }
    return nextPosition;
}

void exchange(){
    player = 1 - player;
}

int collectFromChosenPosition(coordinate chosenPosition){
    if (chosenPosition.position == 0 || chosenPosition.position == 6){
        a[chosenPosition.position]--;
        return 1;
    }
    else{
        int n = a[chosenPosition.position];
        a[chosenPosition.position] = 0;
        return n;
    }
}

void sumEndGame(){
    for (int i = 1; i<=5; i++) scorePlayer[0] +=a[i];
    for (int i = 8; i<=12; i++) scorePlayer[1] += a[i];
}

bool endGameCondition(){
    return ( (a[0] + a[13]) * (a[0] + a[13]) +  (a[6] + a[7]) * (a[6] + a[7]) ==0);
}

void scatterNew(){
    //player 1: 1  2  3  4  5
    //player 2: 8  9 10 11 12
    for (int i = 1; i<=5; i++){
        a[player*7 + i] = 1; // player = 0 || player = 1
    }
    scorePlayer[player] -=5;

    // if player's score is lower than 5, it will be a negative =))
}

bool checkScatterNew(){
    for (int i = 1; i<=5; i++){
        if (a[player*7 + i]) return false;
    }
    return true;
}

void scatter(coordinate chosenPosition){
    int number = collectFromChosenPosition(chosenPosition); // collect from chosen position and this position becomes empty
    printWhileScattering(number); system("pause");
    coordinate nextPosition = chosenPosition, lastPosition, secondNextPosition;
    while(number){
        nextPosition = getNextPosition(nextPosition);
        (a[nextPosition.position])++;
        number--;
        printWhileScattering(number);
        system("pause");
    }

    lastPosition = nextPosition; // last position when scattering finishes
    nextPosition = getNextPosition(nextPosition);
    secondNextPosition = getNextPosition(nextPosition);

    // check the next move
    if ( (lastPosition.position == 5 && lastPosition.direction == 1) || // 4 special positions
        (lastPosition.position == 1 && lastPosition.direction == -1) ||
        (lastPosition.position == 12 && lastPosition.direction == 1) ||
        (lastPosition.position == 8 && lastPosition.direction == -1) ){

        if(a[nextPosition.position]) scatter(nextPosition);
        else{
            if (nextPosition.position == 6){
                if (a[7] ==1) exchange();
                else{
                    if(a[secondNextPosition.position]) sum(secondNextPosition);
                    else exchange();
                }
            }
            else {
                if( a[13] == 1) exchange();
                else{
                    if (a[secondNextPosition.position]) sum(secondNextPosition);
                    else exchange();
                }
            }
        }
    }
    else{ // ordinary positions
        if (a[nextPosition.position]) scatter(nextPosition);
        else {
            if (a[secondNextPosition.position]) sum(secondNextPosition);
            else exchange();
        }
    }
}

void sum(coordinate chosenPosition){
    // cases when the king can be summed up
    int earnedStone;
    if ( (chosenPosition.position == 0 && a[13] != 0 ) || (chosenPosition.position == 6 && a[7] != 0) ){
        scorePlayer[player] = scorePlayer[player] + 10 + a[chosenPosition.position];
        earnedStone = 10 +a[chosenPosition.position];
        a[chosenPosition.position] = 0;
        if (chosenPosition.position == 0) a[13] = 0; else a[7] =0; // the value of king cell becomes 0
     }
     else{// ordinary cases
        scorePlayer[player] += a[chosenPosition.position];
        earnedStone = a[chosenPosition.position];
        a[chosenPosition.position] = 0;
     }
     cout << "You have just earned " << earnedStone << "!" << endl;
     system("pause");

     // check next move
    coordinate nextPosition = getNextPosition(chosenPosition);
    coordinate secondNextPosition = getNextPosition(nextPosition);

    if ( (chosenPosition.position == 5 && chosenPosition.direction == 1) || // 4 special positions
        (chosenPosition.position == 1 && chosenPosition.direction == -1) ||
        (chosenPosition.position == 12 && chosenPosition.direction == 1) ||
        (chosenPosition.position == 8 && chosenPosition.direction == -1) ){
        if (a[nextPosition.position] == 0 && a[13- nextPosition.position] == 0)
            // position 0 or 6 and the corresponding king position 13 && 7
            sum(secondNextPosition); else exchange();
    }
    else{
        if (a[nextPosition.position]) exchange(); // next position is not empty
        else {
            if (a[secondNextPosition.position]) sum(secondNextPosition);
            else exchange();
        }
    }

}

void gameLoop(){
    while(!endGameCondition()){
        if (checkScatterNew()) {
            scatterNew();
            update();
            cout << "You have just scattered to your cells because all of them are empty!!!";
        }
        coordinate chosenPosition = select();
        scatter(chosenPosition);
        update();
    }
}

void renderFinalResult(){
    cout << endl << "Final Result:";
    cout << "Player 1's score: " << scorePlayer[0] << endl;
    cout << "Player 2's score: " << scorePlayer[1] << endl;
    if (scorePlayer[0] > scorePlayer[1]) cout << endl << "Player 1 wins!";
    else if (scorePlayer[0] < scorePlayer[1]) cout << endl << "!Player 2 wins";
    else cout << endl << "It's a draw!";
}

void printWhileScattering(int numberOfStones){
    system("cls");
    for (int i = 0; i<=6; i++){
        cout << setw(2) << a[i] << " ";
    }
    cout << endl;
    for (int i = 13; i>=7; i--){
        cout << setw(2) << a[i] << " ";
    }
    cout << endl << endl;
    cout << endl << "Number of Stones you're scattering: " << numberOfStones << endl;
}
