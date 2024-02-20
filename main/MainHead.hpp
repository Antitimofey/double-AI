#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath> // need for std::pow


struct Move
{
    bool IsItMovingNow = false;
    //bool IsItPlayersMove = true;
    int startPos = -1;
    int endPos = -2;
    bool chooseStartPos = true;
    bool wasPastMoveEating = false;
};

namespace STR {

struct BestMove
{
    std::vector<int> path;
    int profit = 0;

};

};
// returning local offset of checker: 0: -9 -> -7 -> 7 -> 9 :3
int offset(int x);
void PrintVector (std::vector<int> vec);
