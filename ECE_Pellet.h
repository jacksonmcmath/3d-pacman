/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Pellet declarations,
    ECE_PowerPellet declarations, & 
    ECE_PelletGroup declarations
*/

#pragma once

#include <vector>

#include "ECE_Vector2.h"

// A class to represent a pellet in the ECE Pac-Man game
class ECE_Pellet
{
public:

    ECE_Pellet(int x, int y);
    virtual ~ECE_Pellet() {};

    ECE_Vector2 position;
    float color[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float radius;
    int points;
    bool visible;

    void Draw();
};


// A class to represent a power pellet in the ECE Pac-Man game
class ECE_PowerPellet : public ECE_Pellet
{
public:

    ECE_PowerPellet(int x, int y);
    ~ECE_PowerPellet() {};

    double flashTime;
    double timer;

    void Update(double &dt);
};


// A class to make working with many pellets easier
class ECE_PelletGroup
{
public:

    ECE_PelletGroup();
    ~ECE_PelletGroup();
    
    static const int ROWS = 22;
    static const int COLS = 19;
    static const int map[ROWS][COLS];

    std::vector<ECE_Pellet*> pelletList;
    std::vector<ECE_PowerPellet*> powerpellets;

    void Update(double &dt);
    void CreatePelletList();
    bool empty();
    void Draw();

};
