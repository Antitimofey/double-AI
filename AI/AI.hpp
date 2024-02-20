#pragma once

#include <cmath> // std::ceil in 'AI::MoveWithEating'

#include <MainHead.hpp>
#include "Window.hpp" // need for 'Window::WhatCheckerEated'

namespace AI {
    //Tree MoveWithEating (int* level, int Startpos);

struct Tnode {
    int key = 0;
    struct Tnode *son = nullptr;
    struct Tnode *brother = nullptr;

    Tnode (int val) : key {val} {};
}; 

    STR::BestMove BestMove(int* desk, int color); // may be recursionLevel
    
    std::pair<int, int> MoveWithoutEating (int* level, int startPos);
    std::vector<int> MoveQueenWithoutEating(int* level, int startPos); 
    //std::vector<int> MoveWithEating (int* level, Tnode* mainNode, int counterEatenCheckers);
    std::vector<int> MoveWithEating(int* level, int startPos, int& maxEatenCheckers);
    std::vector<int> MoveQueenWithEating (int* level, int startPos, int& maxEatencheck);

} // end of namespase "AI"
