#ifndef WARSHIPS_MODEL_H
#define WARSHIPS_MODEL_H
#include <vector>
#include <iostream>
#include <ctime>
#include <windows.h>
#include <random>

class Model {
public:
    Model();

    bool Shoot(int i);
    void enemyShoot();
    bool canSet(int);
    void placeShip(int);

    //Getters & setters
    void setBoard(int,int);
    int getBoard(int);
    int getBoardSize();
    int getShips();
    int getPoints();

private:
    std::vector<int> board;
    std::vector<int> excluded;
    int points=0;
    int ships=5;
};

#endif //WARSHIPS_MODEL_H
