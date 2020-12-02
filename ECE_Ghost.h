/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Ghost declarations
*/

#pragma once

#include <stack>
#include <vector>

#include "ECE_Entity.h"
#include "ECE_Node.h"
#include "ECE_Pacman.h"
#include "ECE_Pauser.h"

enum Mode { NONE, SCATTER, CHASE, FREIGHT, SPAWN, GUIDE };

struct Ghost_Mode
{
public:
    Ghost_Mode() {};
    Ghost_Mode(Mode m) : mode(m), time(0), speedMult(1), direction(STOP) {};
    Ghost_Mode(Mode m, double time, double mult=1, ECE_Direction dir=STOP) : mode(m), time(time), speedMult(mult), direction(dir) {};

    Mode mode;
    double time; // in ms
    double speedMult; // 1 = 100%
    ECE_Direction direction;

};

class ECE_Pacman; // weird circular dependencies here with the *pacman parameter
class ECE_Ghost : public ECE_Entity
{
public:

    ECE_Ghost(ECE_NodeGroup  &nodes, ECE_Pacman *pacman, ECE_Pauser * pauser);
    ~ECE_Ghost() {};

    int points;
    ECE_Vector2 goal;
    Ghost_Mode mode;
    double modeTimer;
    std::stack<Ghost_Mode> modeStack;
    std::vector<ECE_Direction> guide;
    ECE_Node *spawnNode;
    int pelletsForRelease;
    bool released;
    std::vector<ECE_Direction> bannedDirections;
    ECE_Pauser *pauser;
    ECE_Pacman *pacman;

    void SetupModeStack();
    void SetGuideStack();
    void ScatterGoal();
    void ChaseGoal();
    void ModeUpdate(double & dt);
    void FreightMode();
    void SpawnMode(double s=1);
    ECE_Node* FindSpawnNode();
    void SpawnGoal();
    void RandomGoal();
    void Update(double & dt);
    void ReverseDirection();
    void GetValidDirections(std::vector<ECE_Direction> &validDirections);
    ECE_Direction RandomDirection(std::vector<ECE_Direction> & validDirections);
    ECE_Direction GetClosestDirection(std::vector<ECE_Direction> & validDirections);
    void MoveBySelf();
    ECE_Direction ForceBacktrack();
    void PortalSlowdown();
    void Draw();

};


class Blinky : public ECE_Ghost
{
public:

    Blinky(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * pauser);
    ~Blinky() {};

    void Draw();
    void Update(double & dt);
    void SetStartPosition();

};


class Pinky : public ECE_Ghost
{
public:

    Pinky(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * pauser);
    ~Pinky() {};

    void ScatterGoal();
    void ChaseGoal();
    void Draw();
    void Update(double & dt);
    void SetStartPosition();

};


class Inky : public ECE_Ghost
{
public:

    Inky(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * pauser);
    ~Inky() {};

    Blinky *blinky;

    void ScatterGoal();
    void ChaseGoal();
    void Draw();
    void Update(double & dt);
    void SetStartPosition();
    void SetGuideStack();

};


class Clyde : public ECE_Ghost
{
public:

    Clyde(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * pauser);
    ~Clyde() {};

    void ScatterGoal();
    void ChaseGoal();
    void Draw();
    void Update(double & dt);
    void SetStartPosition();
    void SetGuideStack();

};


class ECE_GhostGroup
{
public:

    ECE_GhostGroup(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * pauser);
    ~ECE_GhostGroup();

    ECE_NodeGroup *nodes;
    std::vector<ECE_Ghost*> ghosts;

    void Update(double & dt);
    void FreightMode();
    void UpdatePoints();
    void ResetPoints();
    void Hide();
    void Draw();
    void Release(int numEaten);
    void KillThreads();

};
