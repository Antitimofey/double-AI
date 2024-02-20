#include "Window.hpp"

int Window::MouseClick(int x, int y, const int* level, Window::Checkers& checkers, Move& move) {

    static bool chooseStartPos = true; // = &move.chooseStartPos;
    const int color = 2; //color of checker: 2 (white) or 3 (black)

    int pressedCell = y / 100 * 8 + x / 100;

    if (move.IsItMovingNow) {
        std::cerr << "please wait for end of moving operation\n";
        return -1;
    }

    if (move.startPos == move.endPos && move.wasPastMoveEating) { // double check
    // if we are here, we can eat checker once more
        chooseStartPos = false;
        if (Window::WhatCheckerEated(level, move.startPos, pressedCell) == -1) {
            std::cerr << "checker in pos " << move.startPos << " already eated this move,"
                      << " so you have to eat this check once more\n";
            return -1;
        }
    }

    if (chooseStartPos) {
        std::cout << "in choosing statrPos " << level[pressedCell] << std::endl;
        //if (abs(level[pressedCell] - color) % 6 != 0) {
        if (!(level[pressedCell] == color || level[pressedCell] == color + 6)) {
            std::cerr << "no checker of your color in chosen cell\n";
            return -1;
        }
        chooseStartPos = false;
        move.startPos = pressedCell;

    } else {
        if (pressedCell == move.startPos) {
            std::cout << "why do you click here twice?\n";
            return -1;
        }
        
        
        if (abs(move.startPos / 8 - pressedCell / 8) !=
            abs(move.startPos % 8 - pressedCell % 8)) {
            std::cerr << "checkers can move only by diagonal\n";
            std::cerr << "please, choose startPos once again\n";
            chooseStartPos = true;
            return -1;
        }

        if (level[move.startPos] == 8 &&
            Window::WhatCheckerEated(level, move.startPos, pressedCell) == -2) {
                return -1; // queen eated more then one checker per time
            }

        if (level[move.startPos] == 8 &&
            Window::WhatCheckerEated(level, move.startPos, pressedCell) == -3) {
                return -1; // queen coudn`t jump throw checker of the same color
            }

        if ((abs(move.startPos / 8 - pressedCell / 8) > 2) && level[move.startPos] == 2) {
            std::cerr << "checker in pos " << move.startPos << " is not a queen\n";
            return -1;
        }

        if (abs(move.startPos / 8 - pressedCell / 8) == 2 && level[move.startPos] == 2
            && (level[(pressedCell + move.startPos) / 2] + level[move.startPos]) % 6 != 5) {
            std::cerr << "checker in pos " << move.startPos << " is not a queen\n";
            return -1;
            }

        if (move.startPos < pressedCell && ((level[(pressedCell + move.startPos) / 2]
            + level[move.startPos]) % 6 != 5) && level[move.startPos] == 2) {
            std::cerr << "checker in pos " << move.startPos << " is not a qween, so" <<
                      " it cann't go back\n";
            return -1;
        }

        chooseStartPos = true;
        move.endPos = pressedCell;
        checkers.upperChecker = move.startPos;
        move.IsItMovingNow = true; // must be the last sentense in function
    }
    return 0;
}




int Window::WhatCheckerEated(const int* level, int startPos, int endPos) {
    int eatedChecker = -1;
            for (int i = 1; i < abs(startPos % 8 - endPos % 8); ++i) {
                int middleCell = startPos + (endPos - startPos) /
                                 (abs(startPos % 8 - endPos % 8)) * i;
                //std::cout << "middleCell: " << middleCell << std::endl;
                if (level[middleCell] % 6 == level[startPos] % 6) {
                    std::cerr << "ERROR in 'Window::Checkers::moveChecker': "
                              << "imposible to jump throw check with same color \n";
                    std::cerr << "startPos " << startPos << " endPos " << endPos << '\n';
                    return -3;
                }
                if ((level[middleCell] + level[startPos]) % 6 == 5) {
                    if (eatedChecker < 0) {
                        eatedChecker = middleCell;
                    }
                    else {
                        std::cerr << "ERROR in 'Window::Checkers::moveColor': "
                                  << "impossible to eat more than one check per move\n";
                        std::cerr << "startPos " << startPos << " endPos " << endPos << '\n';
                        return -2;
                    }
                }
            }
            return eatedChecker;
}
