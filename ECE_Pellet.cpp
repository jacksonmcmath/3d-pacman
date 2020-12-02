/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Pellet definitions, 
    ECE_PowerPellet definitions, & 
    ECE_PelletGroup definitions
*/

#if __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "ECE_Pellet.h"

ECE_Pellet::ECE_Pellet(int x, int y)
{
    position = ECE_Vector2(x, y);
    radius = 0.1;
    points = 10;
    visible = true;
}

void ECE_Pellet::Draw()
{
    if (!visible) return;

    GLfloat shininess[] = { 5 };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(0.5, 0.5, 0.5);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(position.x, position.y, 0.0f); // move to correct position
        glutSolidSphere(radius, 10, 10); // sphere
    glPopMatrix();
}


ECE_PowerPellet::ECE_PowerPellet(int x, int y) : ECE_Pellet(x, y)
{
    radius = 0.25;
    points = 50;
    flashTime = 250;
    timer = 0;
}

// flash so it's easier to notice
void ECE_PowerPellet::Update(double &dt)
{
    timer += dt;
    if (timer >= flashTime)
    {
        visible = !visible;
        timer = 0;
    }
}


// 0 - blank
// 1 - pellet
// 2 - power pellet

const int ECE_PelletGroup::map[ECE_PelletGroup::ROWS][ECE_PelletGroup::COLS] = 
    { // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // 1
        {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0}, // 2
        {0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0}, // 3
        {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0}, // 4
        {0, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 2, 0}, // 5
        {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, // 6
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // 7
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, // 8
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, // 9
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, // 10
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, // 11
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, // 12
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, // 13
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, // 14
        {0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0}, // 15
        {0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0}, // 16
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // 17
        {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, // 18
        {0, 2, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 2, 0}, // 19
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // 20
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 21
    };

ECE_PelletGroup::ECE_PelletGroup()
{
    CreatePelletList();
}

ECE_PelletGroup::~ECE_PelletGroup()
{
    for (auto p : pelletList)
        delete p;
}

void ECE_PelletGroup::Update(double &dt)
{
    for (auto P : powerpellets)
        P->Update(dt);
}

void ECE_PelletGroup::CreatePelletList()
{
    for (int row = 0; row < ROWS; ++row)
        for (int col = 0; col < COLS; ++col)
        {
            ECE_Pellet *p;
            switch (map[row][col])
            {
            case 1:
                p = new ECE_Pellet(col, row);
                pelletList.push_back(p);
                break;
            case 2:
                p = new ECE_PowerPellet(col, row);
                pelletList.push_back((ECE_PowerPellet *) p);
                powerpellets.push_back((ECE_PowerPellet *) p);
                break;
            
            default:
                break;
            }
        }
}

bool ECE_PelletGroup::empty()
{
    return pelletList.empty();
}

void ECE_PelletGroup::Draw()
{
    for (auto p : pelletList)
        p->Draw();
}
