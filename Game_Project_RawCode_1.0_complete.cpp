#include <bits/stdc++.h>
using namespace std;

int a[2][7];
int scorePlayer[2] = {};
int player = 0; // the first player will play first

struct coordinate{
    int row;
    int column;
    string direction;
};

coordinate getNextRight(coordinate chosenCell);
coordinate getNextLeft(coordinate chosenCell);
void initiate();
void update(); // print the current board
coordinate select();
void exchange();
void scatter_new();
coordinate sum(coordinate chosenCell);
coordinate scatter(coordinate chosenCell);
string check(coordinate chosenCell);
string checkSum(coordinate chosenCell);
void move(coordinate chosenCell);

int main(){
    initiate();
    update();
    bool check = true;
    // when 2 king cells are not empty at the same time
    while( (a[0][0] + a[1][0]) * (a[0][0] + a[1][0]) + (a[0][6] + a[1][6]) *  (a[0][6] + a[1][6]) ){
            // check if all of the cells are empty
        for (int i = 1; i<=5; i++){
            if (a[player][1]) {
                check = false;
                break;
            }
        }

        if (check) scatter_new();

        coordinate chosenCell = select();
        move(chosenCell);
        system("cls");
        update();
        exchange();
    }

    cout << "Game over!" << endl;
    if (scorePlayer[0] > scorePlayer[1]) cout << "Player 1 wins";
    else if (scorePlayer[0] < scorePlayer[1]) cout << "Player 2 wins";
    else cout << "It's a draw!";
    return 0;
}

coordinate getNextRight(coordinate chosenCell){
    coordinate nextPosition;
    nextPosition.direction = "right";

    if (chosenCell.row == 0 && (chosenCell.column>=1 && chosenCell.column <=6) ){
        nextPosition.row = 0;
        nextPosition.column = chosenCell.column-1;
    }

    if (chosenCell.row == 1 && (chosenCell.column >=1 && chosenCell.column <=4)){
        nextPosition.row = 1;
        nextPosition.column = chosenCell.column+1;
    }

    if (chosenCell.row == 1 && chosenCell.column == 5) {
        nextPosition.row = 0;
        nextPosition.column = 6;
    }

    if (chosenCell.row == 0 && chosenCell.column == 0){
        nextPosition.row = 1;
        nextPosition.column = 1;
    }

    return nextPosition;
}

coordinate getNextLeft(coordinate chosenCell){
    coordinate nextPosition;
    nextPosition.direction = "left";

    if (chosenCell.row == 0 && (chosenCell.column>=0 && chosenCell.column <=5)){
        nextPosition.row = 0;
        nextPosition.column = chosenCell.column+1;
    }

    if (chosenCell.row == 1 && (chosenCell.column >=2 && chosenCell.column <=5)){
        nextPosition.row = 1;
        nextPosition.column = chosenCell.column-1;
    }

    if (chosenCell.row == 0 && chosenCell.column == 6) {
        nextPosition.row = 1;
        nextPosition.column = 5;
    }

    if (chosenCell.row == 1 && chosenCell.column == 1){
        nextPosition.row = 0;
        nextPosition.column = 0;
    }

    return nextPosition;
}

void initiate(){
    for (int i = 1; i<=5; i++){
        a[0][i] = 5;
        a[1][i] = 5;
    }
    a[0][0] = 0; a[1][0] = 1; // 1 represents for king
    a[0][6] = 0; a[1][6] = 1; // 1 represents for king
}

void update(){
    for (int i = 0; i<2; i++){
        for (int j = 0; j<7; j++)
        cout << a[i][j] << " ";
    cout << endl;
    }
    cout << endl;
    cout << "Player1's point: " << scorePlayer[0] << endl;
    cout << "Player2's point: " << scorePlayer[1];
    cout << endl;
}

coordinate select(){
    coordinate wantedPosition;

    cout <<  endl << "Choose the cell you want! " << endl << "Input the row: " ;
    cin >> wantedPosition.row;

    cout << "Input the column: " ;
    cin >> wantedPosition.column;

    cout << "Do you want to go the the left or the right? " ;
    cin >> wantedPosition.direction;

    cout << endl;
    return wantedPosition;
}

void scatter_new(){ // if player's point is lower than 5, then it will be a negative number
    for (int i = 1; i<=5; i++)
        a[player][i] = 1;
    scorePlayer[player] -=5;
}

void exchange(){
    if (player == 1) player = 0;
    else player = 1;
    cout << endl << "Player " << player +1 << "'s turn"  << endl;
}

coordinate sum(coordinate chosenCell){
    if (chosenCell.row == 0 && chosenCell.column == 0){
        if (a[1][0]) scorePlayer[player] = scorePlayer[player] + a[0][0] + 10;
        else scorePlayer[player]+= a[0][0];
        a[1][0] = 0;
        a[0][0] = 0;
        return chosenCell;
    }

    if (chosenCell.row == 0 && chosenCell.column == 6){
        if (a[1][6]) scorePlayer[player] = scorePlayer[player] + a[0][6] + 10;
        else scorePlayer[player]+= a[0][6];
        a[0][6] = 0;
        a[1][6] = 0;
        return chosenCell;
    }

    scorePlayer[player] += a[chosenCell.row][chosenCell.column];
    a[chosenCell.row][chosenCell.column] = 0;
    return chosenCell;
}

coordinate scatter(coordinate chosenCell){
    coordinate nextPosition = chosenCell;

    // scatter for king cell in 2 directions
    if (chosenCell.row == 0 && chosenCell.column == 6 && chosenCell.direction == "right"){
        a[0][6]--;
        a[0][5]++;
        nextPosition.row = 0; nextPosition.column = 5; nextPosition.direction = "right";
        return nextPosition;
    }

    if  (chosenCell.row == 0 && chosenCell.column == 0 && chosenCell.direction == "right" ){
        a[0][0]--;
        a[1][1]++;
        nextPosition.row = 1; nextPosition.column = 1; nextPosition.direction  = "right";
        return nextPosition;
    }

    if  (chosenCell.row == 0 && chosenCell.column == 0 && chosenCell.direction == "left" ){
        a[0][0]--;
        a[0][1]++;
        nextPosition.row = 0; nextPosition.column = 1; nextPosition.direction  = "left";
        return nextPosition;
    }

    if  (chosenCell.row == 0 && chosenCell.column == 6 && chosenCell.direction == "left" ){
        a[0][0]--;
        a[1][5]++;
        nextPosition.row = 1; nextPosition.column = 5; nextPosition.direction  = "left";
        return nextPosition;
    }

    while (a[chosenCell.row][chosenCell.column]){
        if (chosenCell.direction == "right")
            nextPosition = getNextRight(nextPosition);
        else nextPosition = getNextLeft(nextPosition);

        (a[nextPosition.row][nextPosition.column])++;
        (a[chosenCell.row][chosenCell.column])--;
    }

    return nextPosition; // coordinate of last cell

}

string check(coordinate chosenCell){
    coordinate nextCell, secondNextCell;

    if (chosenCell.direction == "right"){
        nextCell = getNextRight(chosenCell);
        secondNextCell = getNextRight(nextCell);
    }else{
        nextCell = getNextLeft(chosenCell);
        secondNextCell = getNextLeft(nextCell);
    }

    if (chosenCell.row == 1 && chosenCell.column == 5 && chosenCell.direction == "right"){
        if (!a[0][6]){
            if (!a[1][6] ){
                if (a[0][5]) return "sum"; else return "exchange";
            }
            else return "exchange";
        }
        else return "scatter";
    }

    if (chosenCell.row == 0 && chosenCell.column == 1 && chosenCell.direction == "right"){
        if (!a[0][0]){
            if (!a[1][0]){
                if (a[1][1]) return "sum"; else return "exchange";
                }
            else return "exchange";
        }
        else return "scatter";
    }

    if (chosenCell.row == 1 && chosenCell.column == 1 && chosenCell.direction == "left"){
        if (!a[0][0]){
            if (!a[1][0])
                if (a[0][1]) return "sum"; else return "exchange";
            else return "exchange";
        }
        else return "scatter";
    }

    if (chosenCell.row == 0 && chosenCell.column == 5 && chosenCell.direction == "left"){
        if (!a[0][6]){
            if (!a[1][6])
                if (a[1][5]) return "sum"; else return "exchange";
            else return "exchange";
        }
        else return "scatter";
    }

    if (a[nextCell.row][nextCell.column] == 0){
            if (a[secondNextCell.row][secondNextCell.column] == 0)
                return "exchange";
            else return "sum";
    }
    else return "scatter";
}

string checkSum(coordinate chosenCell){
    coordinate nextCell, secondNextCell;
    if (chosenCell.direction == "right"){
        nextCell = getNextRight(chosenCell);
        secondNextCell = getNextRight(nextCell);
    }
    else{
        nextCell = getNextLeft(chosenCell);
        secondNextCell = getNextLeft(nextCell);
    }

    if ( a[nextCell.row][nextCell.column] == 0 && a[secondNextCell.row][secondNextCell.column] !=0 )
        return "sum"; else return "exchange";
}

void move(coordinate chosenCell){
    coordinate lastCell = scatter(chosenCell);
    string state = check(lastCell);

    while(state!= "exchange"){  // play until cant make any moves
        if (check(lastCell) == "sum"){
            if (chosenCell.direction == "right" ){
                lastCell = sum(getNextRight(getNextRight(lastCell)));
                state = checkSum(lastCell);
            }
            else {
                lastCell = sum(getNextLeft(getNextLeft(lastCell)));
                state = checkSum(lastCell);
            }
        }
        else {
            if (chosenCell.direction == "right"){
                lastCell = scatter(getNextRight(lastCell));
                state = check(lastCell);
            } else {
                lastCell = scatter(getNextLeft(lastCell));
                state = check(lastCell);
            }
        }
    }
}


