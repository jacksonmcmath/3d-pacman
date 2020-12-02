/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    Pauser definitions
*/

#include "ECE_Pauser.h"

ECE_Pauser::ECE_Pauser(bool paused/*=false*/) : paused(paused)
{
    timer = 0;
    pauseTime = 0;
    playerPaused = paused;
    type = NONETYPE;
}

void ECE_Pauser::Update(double & dt)
{
    if (!playerPaused)
    {
        if (paused)
        {
            timer += dt;
            if (timer >= pauseTime)
            {
                timer = 0;
                paused = false;
            }
        }
    }
}

void ECE_Pauser::StartTimer(double ntime, PauseType ntype/*=NONETYPE*/)
{
    pauseTime = ntime;
    type = ntype;
    timer = 0;
    paused = true;
}

// player toggled pause
void ECE_Pauser::Player()
{
    playerPaused = !playerPaused;
    paused = playerPaused;
}

// force a pause or un-pause
void ECE_Pauser::Force(bool p)
{
    paused = p;
    playerPaused = p;
    timer = 0;
    pauseTime = 0;
}

// perform an action if we need to
void ECE_Pauser::SettlePause(void(* die)(void), void(clear)(void))
{
    switch (type)
    {
    case DIE: (*die)(); break;
    case CLEAR: (*clear)(); break;
    
    default:
        break;
    }
}
