/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Entity definitions
*/

#if __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <chrono>

#include "ECE_Entity.h"

const double MAX_FPS = 60.0;
const double MAX_PERIOD = 1000.0 / MAX_FPS;

// constructor
ECE_Entity::ECE_Entity(ECE_NodeGroup &nodes) : nodes(&nodes)
{
    direction = STOP;
    speed = 0.005;
    radius = 0.45;
    collideRadius = 0.24;
    node = this->nodes->nodeList[0]; // set initial node as the first node just because
    target = node;
    SetPosition();
    visible = true;

    bool alive = true;
    start_time = std::chrono::steady_clock::now();
    end_time = std::chrono::steady_clock::now();
}

void ECE_Entity::SetPosition()
{
    position.x = node->position.x;
    position.y = node->position.y;
}

void ECE_Entity::Update(double &dt) // dt is in milliseconds
{
    switch (direction)
    { // position = velocity * time
    case RIGHT: position.x += speed * dt; break;
    case UP:    position.y += speed * dt; break;
    case LEFT:  position.x -= speed * dt; break;
    case DOWN:  position.y -= speed * dt; break;
    
    default: break;
    }
    MoveBySelf();
}

void ECE_Entity::MoveBySelf()
{
    if (direction != STOP)
        if (OvershotTarget())
        {
            node = target;
            Portal();
            if (node->neighbors[direction] != NULL)
            {
                target = node->neighbors[direction];
            }
            else
            {
                SetPosition();
                direction = STOP;
            }
        }
}

// check if we went passed the target node
bool ECE_Entity::OvershotTarget()
{
    if (target != NULL)
    {
        ECE_Vector2 vec1 = target->position - node->position;
        ECE_Vector2 vec2 = position - node->position;
        double node2target = vec1.magnitude_squared();
        double node2self = vec2.magnitude_squared();
        return node2self >= node2target;
    }
    return false;
}

void ECE_Entity::ReverseDirection()
{
    switch (direction)
    {
    case RIGHT: direction = LEFT;  break;
    case UP:    direction = DOWN;  break;
    case LEFT:  direction = RIGHT; break;
    case DOWN:  direction = UP;    break;
    
    default: break;
    }
    ECE_Node *tmp = node;
    node = target;
    target = tmp;
}

// check if we hit a portal node
void ECE_Entity::Portal()
{
    if (node->portalNode)
    {
        node = node->portalNode;
        SetPosition();
    }
}

// basic draw function that isn't used
void ECE_Entity::Draw()
{
    if (!visible) return;

    GLfloat shininess[] = { 5 };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(position.x, position.y, 0.0f); // move to correct position
        glutSolidSphere(radius, 20, 20); // sphere
    glPopMatrix();
}

// calculates one game tick for 60 FPS (about 33ms) and sleeps if we are working ahead of schedule
double ECE_Entity::Tick()
{
    start_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> work_time = start_time - end_time;

    if (work_time.count() < MAX_PERIOD)
    {
        std::chrono::duration<double, std::milli> delta(MAX_PERIOD - work_time.count());
        auto delta_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta);
        std::this_thread::sleep_for(std::chrono::milliseconds(delta_duration.count()));
    }
    end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> sleep_time = end_time - start_time;
    // std::cout << "Time: " << (int) (work_time + sleep_time).count() << std::endl;
    return (work_time + sleep_time).count();
}
