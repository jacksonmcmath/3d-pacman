/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Pacman declarations
*/

#pragma once

#include "ECE_Entity.h"
#include "ECE_Ghost.h"
#include "ECE_Pellet.h"
class ECE_Ghost; // that same weird circular dependency
class ECE_Pacman : public ECE_Entity
{
public:

    ECE_Pacman(ECE_NodeGroup &nodes);
    ~ECE_Pacman() { alive = false; };

    float color[4] = { 1.0f, 1.0f, 0.0f, 1.0f }; // yellow
    ECE_Direction desired;
    int lives;

    void Update(double &dt);
    void MoveByKey(ECE_Direction &dir);
    void Draw();
    ECE_Pellet* EatPellets(std::vector<ECE_Pellet*> &pelletList);
    ECE_Ghost* EatGhost(std::vector<ECE_Ghost*> ghosts);
    void SetStartPosition();
    void LoseLife();
    void DrawLives();
    void Reset();

};
