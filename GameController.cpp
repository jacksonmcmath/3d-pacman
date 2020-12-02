/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    GameController definitions
*/

#include "GameController.h"

GameController::GameController()
{
    gameboard = new ECE_GameBoard(/*debug=*/false);
    pauser = new ECE_Pauser(/*paused=*/true);
    nodes = new ECE_NodeGroup();
    pellets = new ECE_PelletGroup();
    pacman = new ECE_Pacman(*nodes);
    ghosts = new ECE_GhostGroup(*nodes, pacman, pauser);
    pelletsEaten = 0;
    start = std::chrono::steady_clock::now();
    end = std::chrono::steady_clock::now();
}

GameController::~GameController()
{
    delete gameboard;
    delete nodes;
    delete pellets;
    delete pacman;
    delete ghosts;
    delete pauser;
}

// limits program to MAX_FPS and returns ms since last called
double GameController::tick()
{
    start = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> work_time = start - end;

    if (work_time.count() < MAX_PERIOD)
    {
        std::chrono::duration<double, std::milli> delta(MAX_PERIOD - work_time.count());
        auto delta_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta);
        std::this_thread::sleep_for(std::chrono::milliseconds(delta_duration.count()));
    }
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> sleep_time = end - start;
    // std::cout << "Time: " << (int) (work_time + sleep_time).count() << std::endl;
    return (work_time + sleep_time).count();
}

void GameController::checkPelletEvents()
{
    // get pellet (NULL if nothing is consumed)
    ECE_Pellet *pellet = pacman->EatPellets(pellets->pelletList);

    // remove the pellet from the list (pass if NULL)
    pellets->pelletList.erase(std::remove(pellets->pelletList.begin(), pellets->pelletList.end(), pellet), pellets->pelletList.end());

    // increment the pellet counter
    if (pellet != NULL) pelletsEaten++;

    // check if pellet was power pellet
    std::vector<ECE_PowerPellet*>::iterator pos = std::find(pellets->powerpellets.begin(), pellets->powerpellets.end(), pellet);
    if (pos != pellets->powerpellets.end())
    {
        pellets->powerpellets.erase(pos);
        ghosts->FreightMode();
    }

    // free allocated pellet
    delete pellet;

    if (pellets->pelletList.empty()) // if empty, pacman wins
    {
        pacman->visible = false;
        ghosts->Hide();
        pauser->StartTimer(3000, CLEAR);
    }
}

void GameController::checkGhostEvents()
{
    // check if ready to release ghosts
    ghosts->Release(pelletsEaten);

    // check if collision with ghost (NULL if no collision)
    ECE_Ghost *ghost = pacman->EatGhost(ghosts->ghosts);
    if (ghost != NULL)
        switch (ghost->mode.mode) // if power pellet has been eaten
        {
        case FREIGHT:
            ghost->SpawnMode(2); // respawn the ghost
            pauser->StartTimer(1000, NONETYPE);
            pacman->visible = false;
            ghost->visible = false;
            break;
        case CHASE:
        case SCATTER:
            pacman->LoseLife();
            ghosts->Hide();
            pauser->StartTimer(3000, DIE);

        default: break;
        }

}

void GameController::StartGame()
{ // easier to just allocate new ones than reset them
    delete ghosts;
    delete pacman;
    delete pellets;
    delete nodes;

    nodes = new ECE_NodeGroup();
    pellets = new ECE_PelletGroup();
    pacman = new ECE_Pacman(*nodes);
    ghosts = new ECE_GhostGroup(*nodes, pacman, pauser);
    pelletsEaten = 0;
    pauser->Force(true);
}

void GameController::RestartLevel()
{
    pacman->Reset();
    delete ghosts; // again, easier to just alloc new ones
    ghosts = new ECE_GhostGroup(*nodes, pacman, pauser);
    pauser->Force(true);
}

void GameController::ResolveDeath()
{
    if (pacman->lives == 0)
        StartGame();
    else
        RestartLevel();

    pauser->type = NONETYPE;
}

void GameController::ResolveClear()
{
    StartGame();
    pauser->type = NONETYPE;
}
