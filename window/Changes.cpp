#include "Window.hpp"

//delete it!!!
void printSfVector (sf::Vector2f vec) {
    std::cout << vec.x << "  " << vec.y << std::endl;
}



void Window::ShowMove(int*level, int startPos, int endPos) {
    if (startPos < 2 || endPos != 1) {
        std::cerr << "INCORRECT CALL OF 'Window::ShowMove': startPos is " << startPos
                  <<", endPos is " << endPos << "\n";
    }
    ;
}

bool IsCellInBoard(int cell) {
    return false;
}

void Window::ChangeDeskWhenChekerMove (int* level, int startPos, int endPos) {
    level[endPos] = level[startPos];
    level[startPos] = 1;
    //std::cout << startPos << endPos << std::endl;

    if (level[endPos] == 2 && endPos < 8) { // case turning in white queen
        level[endPos] = 8;
    }
    //std::cout << std::boolalpha << (level[startPos]) << std::endl;
    if (level[endPos] == 3 && endPos >= 56) { // case turning in black queen
        level[endPos] = 9;
        std::cout << "sucsessfull turning in black checker\n";
    }
}
