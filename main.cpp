/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    main entry-point
*/

#if __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>


#include "GameController.h"


GameController *gc;

// because i can't pass non-static methods as parameters
void resolve_death() { gc->ResolveDeath(); }
void resolve_clear() { gc->ResolveClear(); }

// run continuously
void update()
{
    // clock tick
    double dt /*ms*/ = gc->tick();
    // update all entities
    if (!gc->pauser->paused)
    {
        gc->pacman->Update(dt); 
        gc->ghosts->Update(dt);
        if (gc->pauser->type != NONETYPE)
            gc->pauser->SettlePause(resolve_death, resolve_clear);
        // check for any events
        gc->checkPelletEvents();
        gc->checkGhostEvents();
    }
    gc->pauser->Update(dt);
    gc->pellets->Update(dt);
    // (re) draw the scene
    glutPostRedisplay();
}

// run once to init stuff
void init()
{
    gc = new GameController();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    float color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, color);
}

// draw everything in the scene
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity(); // reset transformations

    gluLookAt(9.0, -17.0, 25.0,    // camera location
              9.0,  11.0,  0.0,    // looking at
              0.0,   0.0,  1.0);   // up vector

    // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f);
    GLfloat position[] = { 80.0f, 80.0f, 80.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glPushMatrix();
        gc->gameboard->Draw();
        gc->pacman->Draw();
        gc->pellets->Draw();
        gc->ghosts->Draw();
        gc->pacman->DrawLives();
    glPopMatrix();

    glutSwapBuffers();
}

// when the window is resized
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (GLfloat)w / (GLfloat)h, 5.0, 50.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    // only used to pause/un-pause
    switch (key)
    {
    // pause on p or space
    case ' ':
    case 'p':
        gc->pauser->Player();
        break;
    
    default: // un-pause on any keypress, unless the player paused the game
        if (gc->pauser->paused)
            gc->pauser->Force(false);
        break;
    }
}

void special(int key, int x, int y)
{
    // un-pause unless the player paused the game
    if (gc->pauser->paused)
        gc->pauser->Force(false);
    switch(key)
    { // only used to control pacman
    case GLUT_KEY_RIGHT:
		gc->pacman->desired = RIGHT;
        break;
    case GLUT_KEY_UP:
		gc->pacman->desired = UP;
        break;
	case GLUT_KEY_LEFT:
		gc->pacman->desired = LEFT;
        break;
    case GLUT_KEY_DOWN:
		gc->pacman->desired = DOWN;
        break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1080, 1080);
    glutInitWindowPosition(100, 100);
    glutCreateWindow( "3D Pac-Man" );
    glutDisplayFunc(display);       // (re)display callback
    glutReshapeFunc(reshape);       // window reshape callback
    glutIdleFunc(update);           // idle callback (continuously called)
    glutKeyboardFunc(keyboard);     // process standard key clicks
    glutSpecialFunc(special);       // process special keys (arrow keys)

    init();
    glutMainLoop();
    return 0;
}
