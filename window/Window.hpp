#pragma once

#include "MainHead.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono> // to make a delay in Window::Checkers::moveCheck
#include <thread> // to make a delay in Window::Checkers::moveCheck

//delete it!!!
void printSfVector (sf::Vector2f vec);

namespace Window {
    void ChangeDeskWhenChekerMove (int* level, int startPos, int endPos);
    void ShowMove(int*level, int startPos, int endPos);
    int WhatCheckerEated(const int* level, int startPos, int endPos);
    //int* ShowPossibleWaysToMove(int* level, int start_cell);
    

    class Board : public sf::Drawable, public sf::Transformable
    {
    public:

        bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles)
        {
            // load the tileset texture
            if (!m_tileset.loadFromFile(tileset))
                return false;

            // resize the vertex array to fit the level size
            m_vertices.setPrimitiveType(sf::Triangles);
            m_vertices.resize(width * height * 6);

            // populate the vertex array, with two triangles per tile
            for (unsigned int i = 0; i < 8; ++i) { // i is Y-coord
                for (unsigned int j = 0; j < 8; ++j) { // j is X-coord
                    int cell_ind = i * 8 + j;
                    int renderingCell = 0;
                    if (tiles[cell_ind]) {
                        renderingCell = 1;
                    }
                    m_vertices[cell_ind*6].position = sf::Vector2f(j*100, i*100);
                    m_vertices[cell_ind*6 + 1].position = sf::Vector2f((j+1)*100, i*100);
                    m_vertices[cell_ind*6 + 2].position = sf::Vector2f(j*100, (i+1)*100);
                    m_vertices[cell_ind*6 + 3].position = sf::Vector2f(j*100, (i+1)*100);
                    m_vertices[cell_ind*6 + 4].position = sf::Vector2f((j+1)*100, i*100);
                    m_vertices[cell_ind*6 + 5].position = sf::Vector2f((j+1)*100, (i+1)*100);

                    m_vertices[cell_ind*6].texCoords = sf::Vector2f(renderingCell*100, 0);
                    m_vertices[cell_ind*6 + 1].texCoords = sf::Vector2f((renderingCell+1)*100, 0);
                    m_vertices[cell_ind*6 + 2].texCoords = sf::Vector2f(renderingCell*100, 100);
                    m_vertices[cell_ind*6 + 3].texCoords = sf::Vector2f(renderingCell*100, 100);
                    m_vertices[cell_ind*6 + 4].texCoords = sf::Vector2f((renderingCell+1)*100, 0);
                    m_vertices[cell_ind*6 + 5].texCoords = sf::Vector2f((renderingCell+1)*100, 100);
                }
            }

            return true;
        }

    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            // apply the transform
            states.transform *= getTransform();

            // apply the tileset texture
            states.texture = &m_tileset;

            // draw the vertex array
            target.draw(m_vertices, states);
        }

        sf::VertexArray m_vertices;
        sf::Texture m_tileset;
        const unsigned int width = 8;
        const unsigned int height = 8;
    };


    class CellFraim : public sf::Drawable, public sf::Transformable
    { //draw a frame around the aimed cell
    public:
        CellFraim (int einCell, int einWidth) : cell {einCell}, width {einWidth} {
            for (int i = 0; i < 2; ++i){
                rectangles[i*2 + 0].setSize(sf::Vector2f(width, 100));
                rectangles[i*2 + 0].setPosition(X - width/2 + 100*i, Y);
                rectangles[i*2 + 1].setSize(sf::Vector2f(100 + width, width));
                rectangles[i*2 + 1].setPosition(X - width/2, Y + 100*i - width/2);
            }

            for (auto& rectangle : rectangles) {
            rectangle.setFillColor(sf::Color::Red);
            }
        }

        void ChangeCell (int newCell) {
            cell = newCell;
            X = (cell % 8) * 100; // X-coord of left top corner of cell
            Y = (cell / 8) * 100; // Y-coord of left top corner of cell
            for (int i = 0; i < 2; ++i){
            rectangles[i*2 + 0].setPosition(X - width/2 + 100*i, Y);
            rectangles[i*2 + 1].setPosition(X - width/2, Y + 100*i - width/2);
            }
        }

    private:
        int cell;
        sf::VertexArray vertices;
        int X = (cell % 8) * 100; // X-coord of left top corner of cell
        int Y = (cell / 8) * 100; // Y-coord of left top corner of cell
        int width; // width of the frame
        sf::RectangleShape rectangles[4]; // 0 = left -> bottom -> right -> up = 3

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            states.transform *= getTransform();
            states.texture = NULL;

           for (auto elem : rectangles) {
            target.draw(elem, states);
           }

        }

    };


    class Checkers : public sf::Drawable, public sf::Transformable
    {
    public:
        int upperChecker = 0;
        Checkers (int* desk, sf::Texture* texture) {
            for (int i = 0; i < 64; ++i) {
                if (desk[i] > 1) {
                    sf::CircleShape circle (47, 100);
                    circle.setTexture(texture);
                    circle.setTextureRect(sf::IntRect(sf::Vector2i(209 + (desk[i] - 2) * 100, 3),
                                          sf::Vector2i(85, 85)));
                    circle.setPosition(sf::Vector2f(((i % 8) * 100 + 3), ((i / 8) * 100 + 3)));
                    arrCircles[i] = circle;
                }
            }
        }

        bool moveChecker (int* level, Move& move) {
            int eatedChecker = -1;
            static sf::Texture tex;
            tex.loadFromFile("../final_texture.png");
            int startPos = move.startPos;
            int endPos = move.endPos;

            if (abs(startPos % 8 - endPos % 8) == 0) {
                std::cout << "ERROR in 'Window::Checkers::moveChecker': " <<
                        "division by zero: \n";
                std::cout << "startPos " << startPos << " endPos " << endPos << std::endl;
                return false;
            }

           eatedChecker = Window::WhatCheckerEated(level, startPos, endPos);

            if (eatedChecker > 0) {
                move.wasPastMoveEating = true;
            } else {move.wasPastMoveEating = false;}

            int shadowingPos = 100 * (startPos % 8 - eatedChecker % 8) /
                                     (startPos % 8 - endPos % 8);
            if (arrCircles[startPos].getRadius() != 47 || arrCircles[endPos].getRadius() != 0
                || abs(startPos / 8 - endPos / 8) != abs(startPos % 8 - endPos % 8)) {
                std::cerr << "INCORRECT CALL OF 'Window::Chechers::moveChecker', "
                          << "startPos is: " << startPos << ", endPos is: " << endPos << "\n";
                std::cerr <<arrCircles[startPos].getRadius() << arrCircles[endPos].getRadius() << "\n";
                return false;
            }
            sf::Vector2f deltaVector (sf::Vector2f((endPos % 8 - startPos % 8) * 100,
                                                   (endPos / 8 - startPos / 8) * 100));
            sf::Vector2f movingVector = sf::Vector2f (deltaVector.x / 100, deltaVector.y / 100);
            upperChecker = startPos; // to draw this checker up
            for (int i = 0; i < 100; ++i) {
                std::this_thread::sleep_for(std::chrono::nanoseconds(10000000));
                arrCircles[startPos].move(movingVector);
                if (shadowingPos == i && eatedChecker > 0) {
                    arrCircles[eatedChecker].setRadius(0);
                    level[eatedChecker] = 1;
                }
            }
            arrCircles[startPos].setRadius(0);
            arrCircles[endPos].setRadius(47);
            arrCircles[endPos].setPosition(sf::Vector2f(((endPos % 8) * 100 + 3),
                                                        ((endPos / 8) * 100 + 3)));
            if (level[startPos] < 4) {
                arrCircles[endPos].setFillColor(sf::Color::White);
                arrCircles[endPos].setTexture(&tex);
                arrCircles[endPos].setTextureRect(sf::IntRect(sf::Vector2i(209 +
                                                (level[startPos] - 2) * 100, 3),
                                                sf::Vector2i(85, 85)));
            }
            if ((level[startPos] == 2 && endPos < 8) || level[startPos] == 8) {
                // case turning in white queen
                arrCircles[endPos].setTexture(NULL);
                arrCircles[endPos].setFillColor(sf::Color::White);
            }
            if ((level[startPos] == 3 && endPos >= 56) || level[startPos] == 9) { 
                // case turning in black queen
                arrCircles[endPos].setTexture(NULL);
                arrCircles[endPos].setFillColor(sf::Color::Black);
            }
            ChangeDeskWhenChekerMove(level, startPos, endPos);
            //move.IsItMovingNow = false;
            return true;
        }

    private:
        sf::CircleShape arrCircles[64];

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            states.transform *= getTransform();
            states.texture = NULL;

            for (int i = 0; i < 64; ++i) {
                if (i != upperChecker) {
                    target.draw(arrCircles[i], states);
                }
            }
            target.draw(arrCircles[upperChecker], states);
        }
    };

    bool PlayingWindow(const int* level, Window::Checkers& checkers, Move& move);
    int MouseClick(int x, int y, const int* level, Window::Checkers& checkers, Move& move);
}
