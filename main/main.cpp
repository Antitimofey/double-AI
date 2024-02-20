#include "MainHead.hpp"
#include "Window.hpp"
#include "AI.hpp"
#include <cmath> // delete it

void PrintTnode(AI::Tnode mainNode) {
        AI::Tnode nowNode = mainNode;
        while (nowNode.brother != nullptr){
            nowNode = *nowNode.brother;
            std::cout << nowNode.key << std::endl;
            if (nowNode.son != nullptr) {
                PrintTnode(*nowNode.son);
            }
    }
}

void PrintVector (std::vector<int> vec) {
    for (auto elem : vec) {
        std::cout << elem << "\t";
    }
    std::cout << std::endl;
}

// returning local offset of checker: 0: -9 -> -7 -> 7 -> 9 :3
int offset(int x) { 
    return std::pow(-1, (1 + x / 2)) *  (2 * (abs(x - 1.5) / 1) + 7);
}

int main()
{
        int level[] =
    {
        0, 3, 0, 3, 0, 3, 0, 3,
        3, 0, 3, 0, 3, 0, 3, 0,
        0, 3, 0, 3, 0, 3, 0, 3,
        1, 0, 1, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 0, 1, 0, 1,
        2, 0, 2, 0, 2, 0, 2, 0,
        0, 2, 0, 2, 0, 2, 0, 2,
        2, 0, 2, 0, 2, 0, 2, 0,
    };

    int level1[] =
    {
        0, 1, 0, 1, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 0, 1, 0,
        0, 1, 0, 3, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 0, 1, 0,
        0, 2, 0, 1, 0, 1, 0, 1,
        1, 0, 2, 0, 1, 0, 1, 0,
        0, 2, 0, 1, 0, 2, 0, 2,
        2, 0, 2, 0, 1, 0, 2, 0,
    };

    int level2[] =
    {
        0, 8, 0, 1, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 0, 1, 0,
        0, 2, 0, 1, 0, 1, 0, 1,
        2, 0, 2, 0, 2, 0, 1, 0,
        0, 2, 0, 2, 0, 2, 0, 2,
        1, 0, 1, 0, 2, 0, 2, 0,
    }; 

    int level3[] = {
        0, 1, 0, 1, 0, 3, 0, 3,
        3, 0, 3, 0, 3, 0, 3, 0,
        0, 3, 0, 1, 0, 3, 0, 2,
        3, 0, 1, 0, 3, 0, 1, 0,
        0, 1, 0, 1, 0, 9, 0, 2,
        2, 0, 2, 0, 1, 0, 2, 0,
        0, 2, 0, 1, 0, 2, 0, 2,
        2, 0, 1, 0, 2, 0, 2, 0,
    };

    int* usingLevel = level;

    sf::Texture tex;
    tex.loadFromFile("../final_texture.png");
    Window::Checkers checkers (usingLevel, &tex);

    Move move;

    bool IsWindowStopped = false;

    STR::BestMove bestMove = AI::BestMove(usingLevel, 3);
    //std::vector<int> a = AI::MoveQueenWithEating(usingLevel, 46); // 21 // 35
    //std::vector<int> a = AI::MoveQueenWithoutEating(usingLevel, 46);
    //std::vector<int> a = AI::MoveWithEating(usingLevel, 46);
    //PrintVector(a);
    std::cout << bestMove.profit << "\n"; PrintVector(bestMove.path);
    //std::cout << "no eating: " << AI::MoveWithoutEating(usingLevel, 12) << std::endl;

    //return 0;




    std::thread t1([&]() { // rendering board
        IsWindowStopped = Window::PlayingWindow(usingLevel, checkers, move);
        //IsWindowStopped = true;
    });

    std::thread t2([&]() {
        while (!IsWindowStopped) {
            if (move.IsItMovingNow) {
                bool IsSucsessfull = false;
                IsSucsessfull = checkers.moveChecker (usingLevel, move);
                if (!IsSucsessfull) {
                    move.IsItMovingNow = false;
                }
                if (move.wasPastMoveEating) {
                    std::vector<int> a;
                    int counter = 0; // does nothung
                    if (usingLevel[move.endPos] == 2) {
                        a = AI::MoveWithEating(usingLevel, move.endPos, counter);
                    }
                    if (usingLevel[move.endPos] == 8) {
                        std::clog << "start of recurtion call" << std::endl;
                        a = AI::MoveQueenWithEating(usingLevel, move.endPos, counter);
                        std::clog << "start of recurtion call" << std::endl;
                    }
                    if (a.size() > 1) {
                        move.startPos = move.endPos;
                        move.IsItMovingNow = false;
                    }
                }

            if (move.IsItMovingNow) {
                STR::BestMove bestMove = AI::BestMove(usingLevel, 3);
                std::vector<int> a = bestMove.path;
                //std::cout << "profit: " << bestMove.profit << std::endl;
                //PrintVector(a);
                for (int i = a.size() - 1; i > 0; --i) {
                    move.startPos = a[i];
                    move.endPos = a[i - 1];
                    bool IsSucsessfull = false;
                    IsSucsessfull = checkers.moveChecker(usingLevel, move);

                }
                move.IsItMovingNow = false;

            }
            }
        }


    });

    t1.join();
    t2.join();


    /*
    */
    return 0;
}