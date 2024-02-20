#include "Window.hpp"
#include <vector>
#include <string>
//#include <pair>

bool Window::PlayingWindow(const int* level, Window::Checkers& checkers, Move& move) {
    // create the window
    sf::RenderWindow window(sf::VideoMode(800 , 800), "with");

    Window::Board map;
    if (!map.load("../final_texture.png", sf::Vector2u(100, 100), level))
        std::cerr << "error!\n";

    sf::Texture tex;
    tex.loadFromFile("../final_texture.png");
    sf::Sprite sp (tex);

    Window::CellFraim fraim (0, 10);
    fraim.ChangeCell(11);

    //Window::Checkers checkers (level, &tex);


    // run the main loop
    while (window.isOpen())
    {
        int upperChecker = 0; // this chek always will cover other in case of collision
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int pressedCell = event.mouseButton.y / 100 * 8 +
                                      event.mouseButton.x / 100;
                    std::cout << "pressedCell is: " << pressedCell << std::endl;

                    Window::MouseClick(event.mouseButton.x, event.mouseButton.y,
                                       level, checkers, move);
                    fraim.ChangeCell(pressedCell);

                    int lengthMainDiagonal = 8 - abs(pressedCell % 8 - pressedCell / 8);
                    int lengthBackDiagonal = 8 - abs(pressedCell % 8 + pressedCell / 8 + 1 - 8);

                    /*
                    static bool chooseStartPos = true;
                    //if (move.IsItPlayersMove) {

                    if (!move.IsItMovingNow) {
                        if (chooseStartPos) {
                            if (level[pressedCell] < 2) {
                                std::cerr << "no checker in chosen cell\n";
                                break;
                            }
                            chooseStartPos = false;
                            move.startPos = pressedCell;
                        } else {
                            chooseStartPos = true;
                            move.endPos = pressedCell;
                            move.IsItMovingNow = true;
                            move.IsItPlayersMove = false;
                            checkers.upperChecker = move.startPos;
                            //если ход в цепочке последний, то move.IsItPiayesMove = false;
                        }
                    } else {std::cout << "Wait for end of moving operation" << std::endl;}
                    //} else {std::cout << "wait for end of computer's move" << std::endl;}
                    //checkers.moveChecker (40, 33);
                    */


                }
            }
        }

        // draw the map
        window.clear();
        window.draw(map);
        window.draw(checkers);
        window.draw(fraim);
        window.display();
    }
    return true;
}