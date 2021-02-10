#include "GUI.h"

GUI::GUI() {
    window.create(sf::VideoMode(windowLength, windowHeight), "Warships ",sf::Style::Default);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);


    if(!font.loadFromFile("FontFile.ttf"))
    {
        MessageBox(NULL,"Font not found!","ERROR",NULL);
        return;
    }

    //Set so that every time, the computer places ships in different spots
    srand (time(NULL));
    (std::mt19937(rand() % 100));  // random-number engine used (Mersenne-Twister in this case)

    state=GAME_MENU;
}


void GUI::drawWindow() {
    if (window.isOpen())
    {
        playerRectangle.clear();
        int x,y;
        for (int i = 0; i < 10; ++i)
        {
            for (int j = 0; j < 10; ++j)
            {
                y = 60 + 55 * i;
                x = 800 + 55 * j;
                playerRectangle.push_back(sf::RectangleShape(sf::Vector2f(50, 50)));
                playerRectangle.back().setPosition(x,y);
                playerRectangle.back().setFillColor(sf::Color::Green);

                y = 60 + 55 * i;
                x = 160 + 55 * j;
                enemyRectangle.push_back(sf::RectangleShape(sf::Vector2f(50, 50)));
                enemyRectangle.back().setPosition(x,y);
                enemyRectangle.back().setFillColor(sf::Color::Green);
            }
        }

        window.clear(sf::Color::Black);
        for (int i =0 ; i<playerModel->getBoardSize();i++) {
            // draw player rectangles
            if (playerModel->getBoard(i)==0){
                playerRectangle.at(i).setFillColor(sf::Color::Green);// hidden
            }
            else if (playerModel->getBoard(i)==1){
                playerRectangle.at(i).setFillColor(sf::Color::Yellow);// mishit
            }
            else if(playerModel->getBoard(i)==2){
                playerRectangle.at(i).setFillColor(sf::Color::Green);// ship during setting, change if you want to see where the computer places ships
            }
            else if(playerModel->getBoard(i)==3){
                playerRectangle.at(i).setFillColor(sf::Color::Cyan); // ship hit
            }

            window.draw(playerRectangle.at(i));
            // draw enemy rectangles
            if (enemyModel->getBoard(i)==0){
                enemyRectangle.at(i).setFillColor(sf::Color::Green); // hidden
            }
            else if (enemyModel->getBoard(i)==1){
                enemyRectangle.at(i).setFillColor(sf::Color::Yellow); // mishit
            }
            else if(enemyModel->getBoard(i)==2){
                enemyRectangle.at(i).setFillColor(sf::Color::Blue); // ship during setting
            }
            else if(enemyModel->getBoard(i)==3){
                enemyRectangle.at(i).setFillColor(sf::Color::Cyan); // ship hit
            }
            window.draw(enemyRectangle.at(i));
        }
        window.display();
    }
}

void GUI::checkEventGameplay() {

    for (int i = 0; i < playerRectangle.size(); i++) {

        if (playerRectangle.at(i).getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) &&
            event.type == sf::Event::MouseButtonPressed && event.mouseButton.button==0&&(playerModel->getBoard(i)==0 || playerModel->getBoard(i)==2)) {
            // If player makes a legal move, the enemy shoots
            if(playerModel->Shoot(i)) {
                enemyModel->enemyShoot();
            }
        }
    }
    //Determines if any player has reached the point threshold
    if(playerModel->getPoints() == points or enemyModel->getPoints() == points) state = GAME_OVER;
}

void GUI::checkPlayerSetting() {
    //Player setting ships
    for (int i = 0; i <  playerRectangle.size(); ++i) {
        //if condition that checks if the player has pressed a rectangle
        if (enemyRectangle.at(i).getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) &&
            event.type == sf::Event::MouseButtonPressed&&event.mouseButton.button==0&&enemyModel->getBoard(i)==0) {
            enemyModel->placeShip(i);
        }
    }
    if(enemyModel->getShips()==0) state = GAME_ENEMY_SETUP;
}

void GUI::checkComputerSetting() {
    std::uniform_int_distribution<int> uni(0,100);
    playerModel->placeShip(uni(rng));
    if(playerModel->getShips()==0) state = GAME_GAMEPLAY;
}

void GUI::runGame() {
    //Main game loop
    while(state != GAME_END)
    {
        switch (state)
        {
            case GAME_MENU:
                menu();
                break;
            case GAME_PLAYER_SETUP:
                checkPlayerSetting();
                break;
            case GAME_ENEMY_SETUP:
                checkComputerSetting();
                break;
            case GAME_GAMEPLAY:
                checkEventGameplay();
                break;
            case GAME_OVER:
                finishGame();
                break;
        }
        if (window.pollEvent(event) && window.isOpen()) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            drawWindow();
        }
    }
}

void GUI::menu() {
    sf::Text title("Warships",font,120);
    title.setStyle(sf::Text::Bold);
    title.setColor(sf::Color::Black);

    title.setPosition(windowLength/2-title.getGlobalBounds().width/2,20);

    int tmp = 2;

    sf::Text text[tmp];

    std::string str[] = {"Play","Exit"};
    for(int i=0; i < tmp; i++)
    {
        text[i].setFont(font);
        text[i].setCharacterSize(65);

        text[i].setString(str[i]);
        text[i].setPosition(windowLength / 2 - text[i].getGlobalBounds().width / 2, 250 + i * 120);
    }

    while(state == GAME_MENU)
    {
        sf::Vector2f mouse(sf::Mouse::getPosition(window));

        while(window.pollEvent(event))
        {
            // ESC or X
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                state = GAME_END;

            // MENU
            else if(text[0].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
            {
                state = GAME_PLAYER_SETUP;
            }

            // EXIT
            else if(text[1].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
            {
                state = GAME_END;
            }
        }
        //Clever animation letting user know if mouse is on the text
        for(int i=0; i < tmp; i++)
            if(text[i].getGlobalBounds().contains(mouse))
                text[i].setColor(sf::Color::Cyan);
            else text[i].setColor(sf::Color::Black);

        window.clear( sf::Color::White);

        window.draw(title);
        for(int i=0; i < tmp; i++)
            window.draw(text[i]);

        window.display();
    }
}

void GUI::finishGame() {
    // setting text options
    text.setFont(font);
    text.setCharacterSize(300);
    text.setColor(sf::Color::Black);
    text.setPosition(windowLength/2 - 230,0 - info.getLocalBounds().width /2);
    text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2.0f, text.getLocalBounds().top + text.getLocalBounds().height / 2.0f);
    text.setFillColor( sf::Color::Red );
    text.setStyle( sf::Text::Style::Bold | sf::Text::Style::Underlined );
    text.setOutlineColor( sf::Color::Yellow );
    text.setOutlineThickness( -2 );

    //setting information options
    info.setFont(font);
    info.setColor(sf::Color::Black);
    info.setCharacterSize(35);
    info.setString("Press ESC");
    info.setPosition(windowLength / 2 - info.getGlobalBounds().width / 2, windowHeight / 2 - info.getGlobalBounds().height / 2 + 150);

    if (playerModel->getPoints() > enemyModel->getPoints()) {
        text.setString("You won!!!");
    }
    else {
        text.setString("You lost!!!");
    }

    //SFML loop
    while(state==GAME_OVER) {
        while (window.pollEvent(event)) {
            window.clear( sf::Color::White);
            window.draw(text);
            window.draw(info);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                event.type = sf::Event::Closed;
            }
            if (event.type == sf::Event::Closed) window.close();

            window.display();
        }
    }
}
