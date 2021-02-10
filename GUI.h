#ifndef WARSHIPS_GUI_H
#define WARSHIPS_GUI_H

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Model.h"
#include <Windows.h>
#include <string>
#include <random>
#include <ctime>
#include <cstdlib>

class GUI {
public:
    GUI();

    void drawWindow();
    void checkEventGameplay();
    void checkPlayerSetting();
    void checkComputerSetting();
    void runGame();
    void finishGame();
private:
    sf::RenderWindow window;
    std::vector<sf::RectangleShape> playerRectangle;
    std::vector<sf::RectangleShape> enemyRectangle;

    Model *playerModel= new Model;
    Model *enemyModel= new Model;

    sf::Event event;
    sf::Font font;
    sf::Text text;
    sf::Text info;

    int windowLength=1500;
    int windowHeight=720;

    enum GameState {GAME_MENU,GAME_PLAYER_SETUP,GAME_ENEMY_SETUP,GAME_GAMEPLAY,GAME_OVER,GAME_END};
    GameState state;

    void menu();

    int const points =5;
    std::mt19937 rng;

};

#endif //WARSHIPS_GUI_H
