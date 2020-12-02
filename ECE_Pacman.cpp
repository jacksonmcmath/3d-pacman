/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Pacman definitions
*/

#if __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "ECE_Pacman.h"

ECE_Pacman::ECE_Pacman(ECE_NodeGroup &nodes) : ECE_Entity(nodes)
{
    lives = 3;
    SetStartPosition();
}

void ECE_Pacman::Update(double &dt)
{
    mutex.lock();
    visible = true;
    switch (direction)
    {
    case RIGHT: position.x += speed * dt; break;
    case UP:    position.y += speed * dt; break;
    case LEFT:  position.x -= speed * dt; break;
    case DOWN:  position.y -= speed * dt; break;
    
    default: break;
    }

    if (desired != direction) {
        MoveByKey(desired);
    } else {
        MoveBySelf();
    }
    mutex.unlock();
}

void ECE_Pacman::MoveByKey(ECE_Direction &dir)
{
    if (direction == STOP) {
        if (node->neighbors[dir] != NULL) {
            target = node->neighbors[dir];
            direction = dir;
        }
    } else {
        // if direction and dir are opposites
        if ((dir == RIGHT && direction == LEFT) || (dir == UP && direction == DOWN) || (dir == LEFT && direction == RIGHT) || (dir == DOWN && direction == UP))
            ReverseDirection();
        if (OvershotTarget()) {
            node = target;
            Portal();
            if (node->neighbors[dir] != NULL) { // if node neighbor in desired direction exists
                if (node->homeEntrance) { // can't go into the ghost house
                    if (node->neighbors[direction] != NULL) {
                        target = node->neighbors[direction];
                    } else {
                        SetPosition();
                        direction = STOP;
                    }
                } else {
                    target = node->neighbors[dir];
                    if (direction != dir) {
                        SetPosition();
                        direction = dir;
                    }
                }
            } else {
                if (node->neighbors[direction] != NULL) { // if node neighbor in current direction exists
                    target = node->neighbors[direction];
                } else {
                    SetPosition();
                    direction = STOP;
                }
            }
        }
    }
}

void ECE_Pacman::Draw()
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

ECE_Pellet* ECE_Pacman::EatPellets(std::vector<ECE_Pellet*> &pelletList)
{
    for (auto p : pelletList)
    {
        ECE_Vector2 d = position - p->position;
        double dSq = d.magnitude_squared();
        double rSq = (p->radius + collideRadius) * (p->radius + collideRadius); // squared
        if (dSq <= rSq) return p;
    }
    return NULL;
}

ECE_Ghost* ECE_Pacman::EatGhost(std::vector<ECE_Ghost*> ghosts)
{
    for (auto g : ghosts) {
        ECE_Vector2 d = position - g->position;
        double dsq = d.magnitude_squared();
        double rsq = (collideRadius + g->collideRadius) * (collideRadius + g->collideRadius);
        if (dsq <= rsq) return g;
    }
    return NULL;
}

void ECE_Pacman::SetStartPosition()
{
    for (auto n : nodes->nodeList) {
        if (n->position.x == 9 && n->position.y == 5) {
            node = n;
        }
    }
    target = node;
    SetPosition();
}

void ECE_Pacman::LoseLife()
{
    lives--; // crazy stuff happening here
}

// draw a sphere for each remaining life on the bottom of the screen
void ECE_Pacman::DrawLives()
{
    for (int i = 0; i < lives - 1; ++i)
    {
        GLfloat shininess[] = { 5 };

        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
        glMaterialfv(GL_FRONT, GL_AMBIENT, color);
        glMaterialfv(GL_FRONT, GL_SPECULAR, color);
        // glColor3f(1.0, 1.0, 0.0);
        glPushMatrix();
            // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
            // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
            glTranslatef(i, -1.0f, 0.0f); // move to correct position
            glutSolidSphere(radius, 20, 20); // sphere
        glPopMatrix();
    }
}

void ECE_Pacman::Reset()
{
    SetStartPosition();
}
