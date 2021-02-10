#include "Model.h"
Model::Model() {
    //Initialize board
    for (int i = 0; i < 100; ++i) {
        board.push_back(0);
        excluded.push_back(i);
    }
}

void Model::setBoard(int a,int b) {
    board.at(a) = b;
}

int Model::getBoard(int a) {
    return board.at(a);
}

int Model::getBoardSize() {
    return board.size();
}

bool Model::canSet(int x) {
    //Function determines if a ship collides or is near any other ship or if it is out of bounds
    bool canSet = true;

    if (board.size()>x && x>=0&&board.at(x)!=0) canSet = false;

    if(x%10!=9) {
        if (board.size() > x + 1 && x + 1 >= 0 && board.at(x + 1) != 0) canSet = false;

        if (board.size() > x - 9 && x - 9 >= 0 && board.at(x - 9) != 0) canSet = false;

        if (board.size() > x + 11 && x + 11 >= 0 && board.at(x + 11) != 0) canSet = false;
    }

    if(x%10!=0) {
        if (board.size() > x - 1 && x - 1 >= 0 && board.at(x - 1) != 0) canSet = false;

        if (board.size() > x + 9 && x + 9 >= 0 && board.at(x + 9) != 0) canSet = false;

        if (board.size() > x - 11 && x - 11 >= 0 && board.at(x - 11) != 0) canSet = false;
    }

    if (board.size()>x+10 && x+10>=0 && board.at(x+10)!=0) canSet = false;

    if (board.size()>x-10 && x-10>=0 && board.at(x-10)!=0) canSet = false;

    return canSet;
}

int Model::getPoints() {
    return points;
}

int Model::getShips() {
    return ships;
}

bool Model::Shoot(int i) {
    bool shot = false;

    if (getBoard(i)==0)
    {
        setBoard(i, 1);
        shot = true;
    }
    else if (getBoard(i)==2){
        setBoard(i,3);
        points++;
        shot = false;
    }

    return shot;
}

void Model::enemyShoot() {
    int tmp= rand() % excluded.size();


    if (getBoard(excluded.at(tmp))==2){
        setBoard(excluded.at(tmp),3);
        points++;
        excluded.erase(excluded.begin()+tmp);
        enemyShoot();
    }
    else {
        setBoard(excluded.at(tmp), 1);
        excluded.erase(excluded.begin()+tmp);
    }
}

void Model::placeShip(int i) {
    if(getBoard(i)==0 && canSet(i)) {
        setBoard(i,2);
        ships--;
    }
    else if(getBoard(i)==2){
        setBoard(i,0);
        ships++;
    }
}

