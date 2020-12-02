/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    GameController declarations
*/

#pragma once

#include "ECE_GameBoard.h"
#include "ECE_Ghost.h"
#include "ECE_Entity.h"
#include "ECE_Node.h"
#include "ECE_Pacman.h"
#include "ECE_Pellet.h"
#include "ECE_Pauser.h"

// a class to consolidate managing all the other classes 
class GameController
{
public:

    GameController();
    ~GameController();

    ECE_GameBoard *gameboard;
    ECE_NodeGroup *nodes;
    ECE_PelletGroup *pellets;
    ECE_Pacman *pacman;
    ECE_GhostGroup *ghosts;
    ECE_Pauser *pauser;
    int pelletsEaten;
    
    static constexpr double MAX_FPS = 60.0;
    static constexpr double MAX_PERIOD = 1000.0 / MAX_FPS;
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;

    double tick();
    void checkPelletEvents();
    void checkGhostEvents();
    void RestartLevel();
    void StartGame();
    void ResolveDeath();
    void ResolveClear();

};
