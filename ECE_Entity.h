/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Entity declarations
*/

#pragma once

#include <chrono>
#include <mutex>
#include <thread>

#include "ECE_Node.h"
#include "ECE_Vector2.h"

class ECE_Entity
{
public:

    ECE_Entity(ECE_NodeGroup &nodes);
    virtual ~ECE_Entity() { alive = false; };

    ECE_Vector2 position;
    ECE_Direction direction;
    double speed;
    float radius;
    float collideRadius;
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white;
    ECE_NodeGroup *nodes;
    ECE_Node *node;
    ECE_Node *target;
    bool visible;

    bool alive;
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    std::thread thread;
    std::mutex mutex;

    void SetPosition();
    virtual void Update(double &dt);
    void MoveBySelf();
    bool OvershotTarget();
    void ReverseDirection();
    void Portal();
    void Draw();
    double Tick();

};
