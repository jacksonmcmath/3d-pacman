/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_GameBoard definitions
*/

#if __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

// #include <stdio.h>

#include "ECE_GameBoard.h"
#include "ECE_Node.h"

// 0 - blank
// 1 - wall
// 2 - coin
// 3 - power-up
// 4 - gate

int ECE_GameBoard::initial_map[ECE_GameBoard::ROWS][ECE_GameBoard::COLS] =
    { // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1}, // 1
        {1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1}, // 2
        {1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 1}, // 3
        {1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1}, // 4
        {1, 3, 2, 1, 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 1, 2, 3, 1}, // 5
        {1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1}, // 6
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1}, // 7
        {1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1}, // 8
        {0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0}, // 9
        {1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1}, // 10
        {0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0}, // 11
        {1, 1, 1, 1, 2, 1, 0, 1, 1, 4, 1, 1, 0, 1, 2, 1, 1, 1, 1}, // 12
        {0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0}, // 13
        {1, 1, 1, 1, 2, 1, 1, 1, 0, 1, 0, 1, 1, 1, 2, 1, 1, 1, 1}, // 14
        {1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 1}, // 15
        {1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1}, // 16
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1}, // 17
        {1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1}, // 18
        {1, 3, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 3, 1}, // 19
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1}, // 20
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 21
    };


ECE_NodeGroup* ECE_GameBoard::nodeGroup = new ECE_NodeGroup();

ECE_GameBoard::ECE_GameBoard(bool dbg /*=false*/) : debug(dbg)
{
    coinCount = 0;
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            if (initial_map[i][j] == 0) coinCount++;
}

ECE_GameBoard::~ECE_GameBoard()
{
    delete nodeGroup;
}

// this was originally going to draw everything, but ended up only drawing the walls
void ECE_GameBoard::Draw()
{
    if (debug) 
    {
        DrawCoordinates();
        DrawNodes();
    }
    
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
            switch (initial_map[y][x])
            {
            case 1: // wall
                DrawWall(x, y);
                break;
            case 2: // pellet
                // DrawPellet(x, y);
                break;
            case 3: // power pellet
                // DrawPowerPellet(x, y);
                break;
            case 4: // gate
                DrawGate(x, y);
                break;

            default: // blank
                break;
            }
}

void ECE_GameBoard::DrawWall(int x, int y)
{
    if (initial_map[y][x] != 1) return; // make sure we're supposed to draw a wall

    // need to check the surrounding walls to see what kind of wall to draw
    //    N
    //  W X E    where X is (x, y)
    //    S
    // example: if there are walls N and W, we draw a corner wall

    // binary encoding for determining what wall sections to draw
    char walls = 0b0000; // 3, 2, 1, 0 = N, E, S, W

    if (y < ROWS - 1 && initial_map[y + 1][x] == 1) walls |= 0b1000; // check north
    if (x < COLS - 1 && initial_map[y][x + 1] == 1) walls |= 0b0100; // check east
    if (y > 0        && initial_map[y - 1][x] == 1) walls |= 0b0010; // check south
    if (x > 0        && initial_map[y][x - 1] == 1) walls |= 0b0001; // check west

    // fix some one-offs at (6, 18), (6, 19), (12, 18) and (12, 19)
    if ((x == 6 || x == 12) && (y == 18 || y == 19)) walls = 0b0101;

    // draw the wall section
    if (walls & 0b1000) DrawWallSection(x, y,  90); // N = 90
    if (walls & 0b0100) DrawWallSection(x, y,   0); // E = 0
    if (walls & 0b0010) DrawWallSection(x, y, 270); // S = 270
    if (walls & 0b0001) DrawWallSection(x, y, 180); // W = 180
}

// draw the specific wall section
void ECE_GameBoard::DrawWallSection(int x, int y, int angle)
{
    GLUquadricObj* qobj = gluNewQuadric();
    GLfloat shininess[] = { 5 };
    float color[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(x, y, 0.0f); // move to correct position

        if (angle ==   0) glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        if (angle ==  90) glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
        if (angle == 180) glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
        if (angle == 270) glRotatef(270.0f, -1.0f, 0.0f, 0.0f);
        
        gluCylinder(qobj, RADIUS, RADIUS, 0.5, 32, 2);
    glPopMatrix();
}

// unused
void ECE_GameBoard::DrawPellet(int x, int y)
{
    GLfloat shininess[] = { 5 };
    float color[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(0.5, 0.5, 0.5);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(x, y, 0.0f); // move to correct position
        glutSolidSphere(RADIUS, 10, 10); // sphere
    glPopMatrix();
}
// unused
void ECE_GameBoard::DrawPowerPellet(int x, int y)
{
    GLUquadricObj* qobj = gluNewQuadric();
    GLfloat shininess[] = { 5 };
    float color[4] = { .92f, 0.80f, .65f, 1.0f };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(1.0, 0.84, 0.0);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(x, y, -0.1f); // move to correct position
        glRotatef(270.0, 0.0f, 0.0f, 1.0f); // rotate vertical?
        gluCylinder(qobj, 0.35, 0.35, .2, 20, 5);
        gluDisk(qobj, 0, .35, 10, 5);
        glTranslatef(0.0f, 0.0f, .2f);
        gluDisk(qobj, 0, .35, 10, 5);
    glPopMatrix();
}
// unused
void ECE_GameBoard::DrawGate(int x, int y)
{
    GLUquadricObj* qobj = gluNewQuadric();
    GLfloat shininess[] = { 5 };
    float color[4] = { .95f, .69f, .98f, 1.0f };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(x-1, y, 0.0f); // move to correct position

        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        
        gluCylinder(qobj, 0.05f, 0.05f, 2, 32, 2);
    glPopMatrix();
}
// unused
bool ECE_GameBoard::isWall(int x, int y)
{
    // special case for tunnel - (0, 11) or (18, 11)
    if (y == 11 && (x == 0 || x == 18)) return false;

    return (initial_map[y][x] == 1 || initial_map[y][x] == 4);
}

// draw coordinates to help debug
void ECE_GameBoard::DrawCoordinates()
{
    // x
    glColor3f(1.0f,0.0f,0.0f); // red x
    glBegin(GL_LINES);

    glVertex3f(-4.0, 0.0f, 0.0f);
    glVertex3f(4.0, 0.0f, 0.0f);
 
    glVertex3f(4.0, 0.0f, 0.0f);
    glVertex3f(3.0, 1.0f, 0.0f);
 
    glVertex3f(4.0, 0.0f, 0.0f);
    glVertex3f(3.0, -1.0f, 0.0f);
    glEnd();
 
    // y 
    glColor3f(0.0f,1.0f,0.0f); // green y
    glBegin(GL_LINES);
    glVertex3f(0.0, -4.0f, 0.0f);
    glVertex3f(0.0, 4.0f, 0.0f);
 
    glVertex3f(0.0, 4.0f, 0.0f);
    glVertex3f(1.0, 3.0f, 0.0f);
 
    glVertex3f(0.0, 4.0f, 0.0f);
    glVertex3f(-1.0, 3.0f, 0.0f);
    glEnd();
 
    // z 
    glColor3f(0.0f,0.0f,1.0f); // blue z
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0f ,-4.0f );
    glVertex3f(0.0, 0.0f ,4.0f );
 
 
    glVertex3f(0.0, 0.0f ,4.0f );
    glVertex3f(0.0, 1.0f ,3.0f );
 
    glVertex3f(0.0, 0.0f ,4.0f );
    glVertex3f(0.0, -1.0f ,3.0f );
    glEnd();
 
}

// draw nodes to help debug
void ECE_GameBoard::DrawNodes()
{
    for (auto n : nodeGroup->nodeList)
        n->Draw();
}
