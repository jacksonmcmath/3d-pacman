/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_GameBoard declarations
*/

#pragma once

#include <list>

#include "ECE_Node.h"

class ECE_GameBoard
{
public:

    ECE_GameBoard(bool debug=false);
    ~ECE_GameBoard();

    static const int ROWS = 22;
    static const int COLS = 19;
    static constexpr float RADIUS = 0.1;

    int coinCount;

    static int initial_map[ROWS][COLS];

    static bool isWall(int x, int y);

    void Draw();
    
    static ECE_NodeGroup *nodeGroup;

private:

    void DrawWall(int x, int y);
    void DrawGate(int x, int y);
    void DrawPellet(int x, int y);
    void DrawPowerPellet(int x, int y);
    void DrawWallSection(int x, int y, int angle);
    void DrawCoordinates();
    void DrawNodes();
    void CreateNodes();

    bool debug;

};
