#include <bits/stdc++.h>
using namespace std;

int a[2][7];
int sumPlayer[2] = {};

struct coordinate{
    int row;
    int column;
    string direction;
};

coordinate getNextRight(int i, int j);
coordinate getNextLeft(int i, int j);
void initiate();
void update(); // print the current board
coordinate select();
string check(int i, int j, string direction);
void sum(int i, int j, int &currentSum);
coordinate scatter(const int i,const int j, string direction);
void scatter_new(int player);

int main(){
    coordinate chosenCell;
    coordinate lastCell;

    initiate();
    update();
    // when 2 king cells are not empty at the same time
    //while( (a[0][0] + a[1][0]) * (a[0][0] + a[1][0]) + (a[0][6] + a[1][6]) *  (a[0][6] + a[1][6]) ){
        chosenCell = select();
        lastCell = scatter(chosenCell.row, chosenCell.column, chosenCell.direction); // scatter and then return the postion of last cell
       cout << check(lastCell.row, lastCell.column, lastCell.direction);
    //}

    update();
    return 0;
}

coordinate getNextRight(int i, int j){
    coordinate nextPosition;
    if (i == 0 && (j>=1 && j <=6)){
        nextPosition.row = 0;
        nextPosition.column = j-1; // bug1: = j-- T.T
    }

    if (i == 1 && (j >=1 && j <=4)){
        nextPosition.row = 1;
        nextPosition.column = j+1;
    }

    if (i == 1 && j == 5) {
        nextPosition.row = 0;
        nextPosition.column = 6;
    }

    if (i == 0 && j == 0){
        nextPosition.row = 1;
        nextPosition.column = 1;
    }

    return nextPosition;
}

coordinate getNextLeft(int i, int j){
    coordinate nextPosition;

    if (i == 0 && (j>=0 && j <=5)){
        nextPosition.row = 0;
        nextPosition.column = j+1;
    }

    if (i == 1 && (j >=2 && j <=5)){
        nextPosition.row = 1;
        nextPosition.column = j-1;
    }

    if (i == 0 && j == 6) {
        nextPosition.row = 1;
        nextPosition.column = 5;
    }

    if (i == 1 && j == 1){
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
    cout << "Player1's point: " << sumPlayer[0] << endl;
    cout << "Player2's point: " << sumPlayer[1];
    cout << endl;
}

coordinate select(){
    coordinate wantedPosition;

    cout << "Choose the cell you want! " << endl << "Input the row: " ;
    cin >> wantedPosition.row;

    cout << "Input the column: " ;
    cin >> wantedPosition.column;

    cout << "Do you want to go the the left or the right? " ;
    cin >> wantedPosition.direction;

    return wantedPosition;
}

string check(int i, int j, string direction){ // return "sum", "stop", "scatter"
    coordinate nextPostion, secondNextPosition;

    //4 occasion when next to the king cell

    if (i == 1 && j == 5 && direction == "right"){
        if (!a[0][6]){
            if (!a[1][6]) return "sum 0, 5";
            else return "stop";
        }
        else return "scatter";
    }

    if (i == 0 && j == 1 && direction == "right"){
        if (!a[0][0]){
            if (!a[1][0]) return "sum 1, 1";
            else return "stop";
        }
        else return "scatter";
    }

    if (i == 1 && j == 1 && direction == "left"){
        if (!a[0][0]){
            if (!a[1][0]) return "sum 1, 1";
            else return "stop";
        }
        else return "scatter";
    }

    if (i == 0 && j == 5 && direction == "left"){
        if (!a[0][6]){
            if (!a[1][6]) return "sum 1, 5";
            else return "stop";
        }
        else return "scatter";
    }

    //other occasions, use the getNext function

    if (direction == "right"){

    nextPostion = getNextRight(i, j);
    secondNextPosition = getNextRight(nextPostion.row, nextPostion.column);

        if (a[nextPostion.row][nextPostion.column] == 0){
            if (a[secondNextPosition.row][secondNextPosition.column] == 0)
                return "stop";
            else return "sum";
        }
        else return "scatter";
    }


    if (direction == "left"){
        nextPostion = getNextLeft(i, j);
        secondNextPosition = getNextLeft(nextPostion.row, nextPostion.column);

        if (a[nextPostion.row][nextPostion.column] == 0){
            if (a[secondNextPosition.row][secondNextPosition.column] == 0)
             return "stop";
            else return "sum";
        }
        else return "scatter";
    }
}

void sum(int i, int j, int &currentSum){
    // sum for 2 king cells

    if (i == 0 && j == 0){
        if (a[1][0]) currentSum = currentSum + a[0][0] + 10;
        else currentSum += a[0][0];
        a[1][0] = 0;
        a[0][0] = 0;
    }

    if (i == 0 && j == 6){
        if (a[1][6]) currentSum == currentSum + a[0][6] + 10;
        else currentSum += a[0][6];
        a[0][6] = 0;
        a[1][6] = 0;
    }

    currentSum += a[i][j];
    a[i][j] = 0;
}

coordinate scatter(const int i, const int j, string direction){
    coordinate nextPosition;

    int row = i, column = j;
    while (a[i][j]){
        if (direction == "right")
            nextPosition = getNextRight(row, column);
        else nextPosition = getNextLeft(row, column);

        (a[nextPosition.row][nextPosition.column])++;
        row = nextPosition.row;
        column = nextPosition.column;
        (a[i][j])--;
    }

    return nextPosition; // coordinate of last cell
}

void scatter_new(int player){ // if player's point is lower than 5, then it will be a negative number
    for (int i = 1; i<=5; i++)
        a[player][i] = 1;
    sumPlayer[player] -=5;
}


void exchange(int player){ //dùng static =)))
    if (player) player = 0;
}
