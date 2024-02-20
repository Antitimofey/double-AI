#include "AI.hpp"
#include <stdlib.h>

bool circleChange(std::vector<int> &vec, int num) {
    for (int i = 0; i < vec.size() - 1; ++i) {
        vec[i] = vec[i+1];
    }
    vec[vec.size() - 1] = num;
    return true;
}


STR::BestMove AI::BestMove(int* desk, int color) {
    //if (color == 3)
    //std::clog << "START OF AI::BESTMOVE, color = " << color << std::endl;
    //const int color = 2; //color of checker: 2 (white) or 3 (black)
    //std::vector<int> bestMoving;
    //int totalCheckersEaten = 0;
    STR::BestMove bestMove;
    for (int i = 0; i < 64; ++i) {
        if ((desk[i] - color) % 6 == 0) {
            int counterEatenChechers = 0;
            std::vector<int> a;
            if (desk[i] > 7) { // queen case
            //std::clog << "START OF AI::QUEEN EATING" << std::endl; // delete it!!!!
            std::clog << "before main call QueenWithEating !!!!!!!!!!!!!" << std::endl;
                a = AI::MoveQueenWithEating(desk, i, counterEatenChechers);
            std::clog << "after main call QueenWithEating !!!!!!!!!!!!!" << std::endl;
            //std::clog << "END OF AI::QUEEN EATING" << std::endl; // delete it!!!!
            } else {
                a = AI::MoveWithEating(desk, i, counterEatenChechers);

            }
            if (counterEatenChechers >= bestMove.profit) {
                bestMove.path = a;
                circleChange(bestMove.path, i);
                bestMove.profit = counterEatenChechers;
            }
        }
    }

    if (bestMove.path.size() == 1) {
        bestMove.path.push_back(-1);
        bestMove.profit = 0;

        int bestPose = -1;
        int startBestPose = -1;
        int bestProfit = -100;
        for (int i = 0; i < 64; ++i) {
            if (desk[i] == color) {
                int move = MoveWithoutEating(desk, i).first;
                int profit = MoveWithoutEating(desk, i).second;
                if (move != -1 && profit > bestProfit) {
                    bestPose = move;
                    bestProfit = profit;
                    startBestPose = i;
                    /*
                    bestMove.path[1] = move;
                    circleChange(bestMove.path, i);
                    return bestMove;
                    */
                }
            }

            if (desk[i] == color + 6) {
                //std::clog << "START OF AI::QUEEN  NOT EATING" << std::endl; // delete it!!!!
                std::vector<int> a = AI::MoveQueenWithoutEating(desk, i);
                //std::clog << "START OF AI::QUEEN NOT EATING" << std::endl; // delete it!!!!
                if (a.size() > 0) {
                    bestMove.path[1] = a[0];
                    circleChange(bestMove.path, i);
                        if (color == 3)
    //std::clog << "END OF AI::BESTMOVE, out 1" << std::endl;
                    return bestMove;
                }
            }
        }
        if (startBestPose != -1) {
            bestMove.path[1] = bestPose;
            circleChange(bestMove.path, startBestPose);
                if (color == 3)
    std::clog << "END OF AI::BESTMOVE, out 2" << std::endl;
            return bestMove;
        }
    }
    if (bestMove.path.size() == 0) {
        std::cerr << "EXEPTION: no checkers to move\n";
        exit(-1);
        // win event
    }
    if (bestMove.path[1] == -1) {
    std::cerr << "EXEPTION: no possible moves\n";
        // win event
    }
    //if (color == 3)
    //std::clog << "END OF AI::BESTMOVE, out 3" << std::endl;
    return bestMove;
}

// bullshit
std::vector<int> AI::MoveQueenWithoutEating(int* level, int startPos) {
    //level[StartPos] can be only a queen: 8 (white) or 9 (black)
    std::vector<int> allMoves;
    if (level[startPos] < 8) {
        std::cerr << "INCORRECT CALL OF 'AI::MoveQueenWithEating', "
                  <<"startPos IS:  " << startPos << "\n";
        //return -1;
    }

    int lengthMainDiagonal = 8 - abs(startPos % 8 - startPos / 8);
    int lengthBackDiagonal = 8 - abs(startPos % 8 + startPos / 8 - 7);

    for (int k = 0; k < 4; ++k) {
        int deltaVector = offset(k);
        for (int i = 1; i < std::max(lengthBackDiagonal, lengthMainDiagonal); ++i) {
            int possibleCell = (startPos + deltaVector * i);


            if (abs(startPos / 8 - (startPos + deltaVector) / 8) != 1 ||
                abs(startPos % 8 - (startPos +  deltaVector) % 8) != 1 ||
                startPos + deltaVector > 64 || startPos + deltaVector < 0) {
                // case out of desk
                break;
            }

            if (level[possibleCell] != 1) { // case over checker
                break;
            }

            allMoves.push_back(possibleCell);
        }
    }
    //return allMoves;
    return allMoves;
}

std::pair<int, int> AI::MoveWithoutEating (int* level, int startPos) {
    std::vector<int> possibleMoves;
    if (level[startPos] == 2) {
        if (startPos > 7 && (startPos / 8 != 0) && level[startPos - 9] == 1) {
            possibleMoves.push_back(startPos - 9);
        }
        if (startPos > 7 && (startPos / 8 != 7) && level[startPos - 7] == 1) {
            possibleMoves.push_back(startPos - 7);
        }
        if (possibleMoves.size() > 0) {
            return std::pair<int, int> (possibleMoves[0], 0);
        }
    }

    int leftMoveProfit = 0; // how many chechers white can eat
    int rightMoveProfit = 0; // if we are here, profit can`t be more, then 0
    if (level[startPos] == 3) {
        
        if (startPos < 56 && (startPos / 8 != 7) && level[startPos + 7] == 1) {
            possibleMoves.push_back(startPos + 7);

            level[startPos + 7] = level[startPos];
            level[startPos] = 1;
            STR::BestMove bestMove = AI::BestMove(level, 2);
            level[startPos] = level[startPos + 7];
            level[startPos + 7] = 1;
            leftMoveProfit = -1 * bestMove.profit;
        }
            if (startPos < 56 && (startPos / 8 != 0) && level[startPos + 9] == 1) {
            possibleMoves.push_back(startPos + 9);

            level[startPos + 9] = level[startPos];
            level[startPos] = 1;
            STR::BestMove bestMove = AI::BestMove(level, 2);
            level[startPos] = level[startPos + 9];
            level[startPos + 9] = 1;
            rightMoveProfit = -1 * bestMove.profit;
        }
    }
    if (possibleMoves.size() == 2) {
        return leftMoveProfit >= rightMoveProfit ? std::pair<int, int>(possibleMoves[0],
        leftMoveProfit) : std::pair<int, int>(possibleMoves[1], rightMoveProfit);

    } else if (possibleMoves.size() == 1) {
        return std::pair<int, int>(possibleMoves[0], std::min(leftMoveProfit, rightMoveProfit));

    } else {return std::pair<int, int>(-1, 0);}
}

int count = 0;


std::vector<int> AI::MoveQueenWithEating (int* level, int startPos, int& maxEatenCheckers) {
    int criticalPoint = 0;
    //level[StartPos] can be only a queen: 8 (white) or 9 (black)
    std::vector<int> bestPathToMove;
    ++maxEatenCheckers;
    int bestWayToEat = -1; // pos to move to eat MORE!!!
    bool isItLeaf = true;
    if (level[startPos] < 8) {
        std::cerr << "INCORRECT CALL OF 'AI::MoveQueenWithEating', "
                  << "startPos IS:  " << startPos << "\n";
        //return -1;
    }


    int lengthMainDiagonal = 8 - abs(startPos % 8 - startPos / 8);
    int lengthBackDiagonal = 8 - abs(startPos % 8 + startPos / 8 - 7);

    for (int k = 0; k < 4; ++k) {
        int deltaVector = offset(k);
        for (int i = 0; i < std::max(lengthBackDiagonal, lengthMainDiagonal); ++i) {
            int possibleCell = (startPos + deltaVector * i);

            if (abs(possibleCell / 8 - (possibleCell + 2 * deltaVector) / 8) != 2 ||
                abs(possibleCell % 8 - (possibleCell +  2 * deltaVector) % 8) != 2 ||
                possibleCell + deltaVector * 2 > 64 || possibleCell + deltaVector * 2 < 0) {
                // case out of desk
                break;
            }

            if (level[possibleCell + deltaVector] == level[startPos] ||
                level[possibleCell + deltaVector] == level[startPos] - 6) {
                // case jump throw queen or checker of same color
                break;
            }

            if (level[possibleCell + deltaVector] == 1) {
                continue;
            }

            //if we are here, there is enimy's checker/queen in pos + deltaVector
            if (level[possibleCell + 2 * deltaVector] != 1) {
                break;
            }
            // if we are here, we definately can eat checker in pos + deltaVector
            // let's find, where we have to stop
            for (int j = 0; j < 6; ++j) {
                std::clog << "START OF FOR" << std::endl;
                int stopCell = possibleCell + (2 + j) * deltaVector;

                if (stopCell < 0) {
                    break;
                }

                if (level[stopCell] != 1) {
                    break;
                }
                int copyLevel[64];
                for (int i = 0;  i < 64; ++i) {
                    copyLevel[i] = level[i];
                }
                copyLevel[stopCell] = copyLevel[startPos];
                copyLevel[startPos] = 1;
                copyLevel[possibleCell + deltaVector] = 1;
                int counterEatencheckers = maxEatenCheckers;
                std::cout << "count " << ++count << std::endl;
                std::vector<int> pathToMove = AI::MoveQueenWithEating(copyLevel, stopCell,
                                                                      counterEatencheckers);
                if (pathToMove.size() >= bestPathToMove.size()) {
                    maxEatenCheckers = counterEatencheckers;
                    //bestPathToMove = pathToMove;
                    bestPathToMove.swap(pathToMove);
                    bestPathToMove.push_back(stopCell);
                }
                isItLeaf = false;
                std::clog << "END OF FOR" << std::endl;
            //break;
            }
            std::clog << "OUT OF ALL FOR OTERATIONS" << std::endl;
        }
    }
    if (isItLeaf) {
        bestPathToMove.push_back(-1);
        --maxEatenCheckers;
        // call of "BestMove"
    }
PrintVector(bestPathToMove);
std::clog << "END OF QUEEN EAT, startPos: " << startPos << std::endl;
    return bestPathToMove;
}


std::vector<int> AI::MoveWithEating(int level[64], int startPos, int& maxEatenCheckers) {
    std::vector<int> bestPathToMove;
    ++maxEatenCheckers;
    const int alreadyEatenCheckers = maxEatenCheckers; // how many checkers we already eat
    //int bestWayToEat = -1; // pos to move to eat MORE!!!
    bool isItLeaf = true;
    if (level[startPos] < 2 || level[startPos] > 3) {
        std::cerr << "INCORRECT CALL OF 'AI::MoveWithoutEating' IN:  "
                  <<"  WHITCH IS:  " << level[startPos] << "\n";
        //return -1;
    }

    for (int i = 0; i < 4; ++i) {
        int deltaVecor = offset(i);
        if (abs(startPos / 8 - (startPos + 2 * deltaVecor) / 8) != 2 ||
        abs(startPos % 8 - (startPos +  2 * deltaVecor) % 8) != 2 ||
        startPos + deltaVecor * 2 > 64 || startPos + deltaVecor * 2 < 0) {
            continue;
        }
        if ((level[startPos + deltaVecor] + level[startPos]) % 6 != 5 ||
        (level[startPos + 2 * deltaVecor] != 1)) {
            continue;
        }

        int copyLevel[64];
        for (int i = 0;  i < 64; ++i) {
            copyLevel[i] = level[i];
        }
        copyLevel[startPos + 2 * deltaVecor] = copyLevel[startPos];
        copyLevel[startPos] = 1;
        copyLevel[startPos + deltaVecor] = 1;


        std::vector<int> pathToMove;
        int counterEatenCheckers = alreadyEatenCheckers;
        if (level[startPos + deltaVecor] >= 8) {
            counterEatenCheckers += 2;
        }
        if ((level[startPos] == 2 && startPos + 2 * deltaVecor < 8) ||
            level[startPos] == 3 && startPos + 2 * deltaVecor >= 56) {
            // case turning in queen
            copyLevel[startPos + 2 * deltaVecor] += 6;
            pathToMove = AI::MoveQueenWithEating(copyLevel, startPos + 2 * deltaVecor,
                                                 counterEatenCheckers);
        } else {
            pathToMove = AI::MoveWithEating(copyLevel, startPos + 2 * deltaVecor,
                                            counterEatenCheckers);
        }

        //if (pathToMove.size() >= bestPathToMove.size()) {
        if (counterEatenCheckers >= maxEatenCheckers) {
            maxEatenCheckers = counterEatenCheckers;
            bestPathToMove = pathToMove;
            bestPathToMove.push_back(startPos + 2 * deltaVecor);
        }
        isItLeaf = false;
    }
    if (isItLeaf) {
        bestPathToMove.push_back(-1);
        --maxEatenCheckers;
        if (level[startPos] % 6 == 3) { // black checker or queen
            // --maxEatenCheckers;
        }
        // call of "BestMove"
    }
    return bestPathToMove;
}
