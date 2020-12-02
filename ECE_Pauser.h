/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Pauser declarations
*/

#pragma once

enum PauseType { NONETYPE, CLEAR, DIE, GHOST };

// a class for handling pausing the game
class ECE_Pauser
{
public:

    ECE_Pauser(bool paused=false);

    bool paused;
    double timer;
    double pauseTime;
    bool playerPaused;
    PauseType type;

    void Update(double & dt);
    void StartTimer(double time, PauseType type=NONETYPE);
    void Player();
    void Force(bool p);
    void SettlePause(void(*die)(void), void(clear)(void));

};
