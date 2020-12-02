/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Ghost definitions
*/

#if __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <algorithm>
#include <random>
#include <iostream>
#include <vector>

#include "ECE_Ghost.h"
#include "ECE_GameBoard.h"

// most of these functions are self explanatory by name

ECE_Ghost::ECE_Ghost(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * p) : ECE_Entity(nodes)
{
    pacman = pm;
    pauser = p;
    speed = 0.00004;
    points = 200;
    goal = ECE_Vector2();
    SetupModeStack(); // alternate modes between scatter & chase
    mode = modeStack.top();
    modeStack.pop();
    modeTimer = 0;
    spawnNode = FindSpawnNode();
    SetGuideStack(); // directions to guide the ghost out of the ghost house
}

void ECE_Ghost::SetupModeStack()
{
    modeStack.push(Ghost_Mode(CHASE));
    modeStack.push(Ghost_Mode(SCATTER, 5000));
    modeStack.push(Ghost_Mode(CHASE, 20000));
    modeStack.push(Ghost_Mode(SCATTER, 7000));
    modeStack.push(Ghost_Mode(CHASE, 20000));
    modeStack.push(Ghost_Mode(SCATTER, 7000));
    modeStack.push(Ghost_Mode(CHASE, 20000));
    modeStack.push(Ghost_Mode(SCATTER, 7000));
}

void ECE_Ghost::SetGuideStack()
{
    guide.push_back(UP);
}

void ECE_Ghost::ScatterGoal()
{
    goal = ECE_Vector2(18, 21);
}

void ECE_Ghost::ChaseGoal()
{
    goal = ECE_Vector2(pacman->position);
}

void ECE_Ghost::ModeUpdate(double & dt)
{
    modeTimer += dt;
    if (mode.time != 0)
        if (modeTimer >= mode.time) {
            ReverseDirection();
            mode = modeStack.top();
            modeStack.pop();
            modeTimer = 0;
        }
}

void ECE_Ghost::FreightMode()
{
    if (mode.mode != SPAWN && mode.mode != GUIDE) {
        if (mode.mode != FREIGHT) {
            if (mode.time != 0) {
                double dt = mode.time - modeTimer;
                modeStack.push(Ghost_Mode(mode.mode, dt));
            } else {
                modeStack.push(Ghost_Mode(mode.mode));
            }
            mode = Ghost_Mode(FREIGHT, 7000, 0.5);
            modeTimer = 0;
        } else {
            mode = Ghost_Mode(FREIGHT, 7000, 0.5);
            modeTimer = 0;
        }
        ReverseDirection();
    }
}

void ECE_Ghost::SpawnMode(double s)
{
    mode = Ghost_Mode(SPAWN, 0, s);
    modeTimer = 0;
    for (auto d : guide)
        modeStack.push(Ghost_Mode(GUIDE, 0, 0.5, d));
}

ECE_Node* ECE_Ghost::FindSpawnNode()
{
    for (auto n : nodes->nodeList)
        if (n->spawnNode)
            return n;
    return NULL;
}

void ECE_Ghost::SpawnGoal()
{
    goal = ECE_Vector2(9, 11);
}

void ECE_Ghost::RandomGoal()
{
    std::default_random_engine generator;
    std::uniform_int_distribution<int> col_dist(0, 19);
    std::uniform_int_distribution<int> row_dist(0, 22);
    int x = col_dist(generator);
    int y = row_dist(generator);
    goal = ECE_Vector2(x, y);
}
// calculate new position and update any modes
void ECE_Ghost::Update(double & dt)
{
    mutex.lock();
    // std::cout << "dt: " << dt << std::endl;
    visible = true;
    PortalSlowdown();
    double speedMod = speed * mode.speedMult;
    switch (direction)
    {
    case RIGHT: position.x += speedMod * dt; break;
    case UP:    position.y += speedMod * dt; break;
    case LEFT:  position.x -= speedMod * dt; break;
    case DOWN:  position.y -= speedMod * dt; break;
   
    default: break; // should never get here
    }
    ModeUpdate(dt);
    switch (mode.mode)
    {
    case CHASE:
        // std::cout << "CHASE" << std::endl;
        ChaseGoal();
        break;
    case SCATTER:
        // std::cout << "SCATTER" << std::endl;
        ScatterGoal();
        break;
    case FREIGHT:
        // std::cout << "FREIGHT" << std::endl;
        RandomGoal();
        break;
    case SPAWN:
        // std::cout << "SPAWN" << std::endl;
        SpawnGoal();
        break;

    default: break; // should never get here
    }
    MoveBySelf();

    mutex.unlock();
}

void ECE_Ghost::ReverseDirection()
{
    if (mode.mode != GUIDE && mode.mode != SPAWN)
        ECE_Entity::ReverseDirection();
}

// add the valid directions to the validDirections list parameter
void ECE_Ghost::GetValidDirections(std::vector<ECE_Direction> & validDirections)
{
    for (auto const &x : node->neighbors)
    { // append direction to list if it's not opposite of current direction
        if (x.second == NULL) continue;
        switch (x.first)
        {
        case RIGHT: 
            if (direction != LEFT) {
                if (mode.mode != SPAWN) {
                    if (!node->homeEntrance) {
                        if (std::find(bannedDirections.begin(), bannedDirections.end(), x.first) == bannedDirections.end()) {
                            validDirections.push_back(x.first);
                        }
                    } else {
                        if (x.first != DOWN)
                            validDirections.push_back(x.first);
                    }
                } else {
                    validDirections.push_back(x.first);
                }
            }
            break;
        case UP: 
            if (direction != DOWN) {
                if (mode.mode != SPAWN) {
                    if (!node->homeEntrance) {
                        if (std::find(bannedDirections.begin(), bannedDirections.end(), x.first) == bannedDirections.end()) {
                            validDirections.push_back(x.first);
                        }
                    } else {
                        if (x.first != DOWN)
                            validDirections.push_back(x.first);
                    }
                } else {
                    validDirections.push_back(x.first);
                }
            }
            break;
        case LEFT: 
            if (direction != RIGHT) {
                if (mode.mode != SPAWN) {
                    if (!node->homeEntrance) {
                        if (std::find(bannedDirections.begin(), bannedDirections.end(), x.first) == bannedDirections.end()) {
                            validDirections.push_back(x.first);
                        }
                    }
                    else {
                        if (x.first != DOWN)
                            validDirections.push_back(x.first);
                    }
                } else {
                    validDirections.push_back(x.first);
                }
            }
            break;
        case DOWN: 
            if (direction != UP) {
                if (mode.mode != SPAWN) {
                    if (!node->homeEntrance) {
                        if (std::find(bannedDirections.begin(), bannedDirections.end(), x.first) == bannedDirections.end()) {
                            validDirections.push_back(x.first);
                        }
                    } else {
                        if (x.first != DOWN)
                            validDirections.push_back(x.first);
                    }
                } else {
                    validDirections.push_back(x.first);
                }
            }
            break;

        default:
            break;
        }
    }
    // std::cout << std::endl;

    if (validDirections.empty())
        validDirections.push_back(ForceBacktrack());
}

ECE_Direction ECE_Ghost::RandomDirection(std::vector<ECE_Direction> & validDirections)
{
    int size = validDirections.size();
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, size-1);
    int index = distribution(generator);
    return validDirections[index];
}

ECE_Direction ECE_Ghost::GetClosestDirection(std::vector<ECE_Direction> & validDirections)
{
    std::vector<double> distances;
    ECE_Vector2 d;
    for (auto dir : validDirections)
    {
        switch (dir)
        {
        case RIGHT: d = ECE_Vector2(1, 0); break;
        case UP: d = ECE_Vector2(0, 1); break;
        case LEFT: d = ECE_Vector2(-1, 0); break;
        case DOWN: d = ECE_Vector2(0, -1); break;
        
        default: break;
        }
        ECE_Vector2 difVec = node->position + d - goal;
        distances.push_back(difVec.magnitude_squared());
    }
    auto index = std::distance(distances.begin(), std::min_element(distances.begin(), distances.end()));
    return validDirections.at(index);
}

void ECE_Ghost::MoveBySelf()
{
    if (OvershotTarget())
    {
        node = target;
        Portal();
        std::vector<ECE_Direction> validDirections;
        GetValidDirections(validDirections);
        direction = GetClosestDirection(validDirections);
        target = node->neighbors[direction];
        SetPosition();
        if (mode.mode == SPAWN)
        {
            if (position == goal)
            {
                mode = modeStack.top();
                modeStack.pop();
                direction = mode.direction;
                target = node->neighbors[direction];
                SetPosition();
            }
        }
        else if (mode.mode == GUIDE)
        {
            mode = modeStack.top();
            modeStack.pop();
            if (mode.mode == GUIDE)
            {
                direction = mode.direction;
                target = node->neighbors[direction];
                SetPosition();
            }
        }
    }
}

ECE_Direction ECE_Ghost::ForceBacktrack()
{
    switch (direction)
    {
    case RIGHT: return LEFT;
    case UP:    return DOWN;
    case LEFT:  return RIGHT;
    case DOWN:  return UP;

    default: return STOP; // should never get here
    }
}

void ECE_Ghost::PortalSlowdown()
{
    speed = 0.005;
    if (node->portalNode || target->portalNode)
        speed *= .5;
}

void ECE_Ghost::Draw()
{
    GLUquadricObj* qobj = gluNewQuadric();
    GLfloat shininess[] = { 5 };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(r, g, b);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(position.x, position.y, -0.25f); // move to correct position
        glRotatef(270.0, 0.0f, 0.0f, 1.0f); // rotate vertical
        gluCylinder(qobj, radius, radius, .5, 20, 5);
        glTranslatef(0.0f, 0.0f, 0.5f);
        glutSolidSphere(radius, 20, 20); // sphere
    glPopMatrix();
}

void blinky_thread_func(Blinky *blinky)
{
    double dt;
    while (blinky->alive)
    {
        dt = blinky->Tick();
        if (blinky->pauser->paused) continue;
        blinky->Update(dt);
    } 
}
Blinky::Blinky(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * pauser) : ECE_Ghost(nodes, pm, pauser)
{
    alive = true;
    SetStartPosition();
    thread = std::thread(blinky_thread_func, this);
    thread.detach();
}
void Blinky::Draw()
{
    if (!visible) return;
    if (mode.mode == SPAWN) return;
    float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // red
    if (mode.mode == FREIGHT)
    {
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
    }
    GLUquadricObj* qobj = gluNewQuadric();
    GLfloat shininess[] = { 5 };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(r, g, b);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(position.x, position.y, -0.25f); // move to correct position
        glRotatef(270.0, 0.0f, 0.0f, 1.0f); // rotate vertical
        gluCylinder(qobj, radius, radius, .5, 20, 5);
        glTranslatef(0.0f, 0.0f, 0.5f);
        glutSolidSphere(radius, 20, 20); // sphere
    glPopMatrix();
}
void Blinky::Update(double & dt)
{
    mutex.lock();
    // std::cout << "dt: " << dt << std::endl;
    visible = true;
    PortalSlowdown();
    double speedMod = speed * mode.speedMult;
    switch (direction)
    {
    case RIGHT: position.x += speedMod * dt; break;
    case UP:    position.y += speedMod * dt; break;
    case LEFT:  position.x -= speedMod * dt; break;
    case DOWN:  position.y -= speedMod * dt; break;
    
    default: break; // should never get here
    }
    ModeUpdate(dt);
    switch (mode.mode)
    {
    case CHASE: ChaseGoal(); break;
    case SCATTER: ScatterGoal(); break;
    case FREIGHT: RandomGoal(); break;
    case SPAWN: SpawnGoal(); break;
    
    default: break; // should never get here
    }
    MoveBySelf();
    
    mutex.unlock();
}
void Blinky::SetStartPosition()
{
    for (auto n : nodes->nodeList) {
        if (n->position.x == 9 && n->position.y == 13) {
            node = n;
            break;
        }
    }
    target = node;
    SetPosition();
}

void pinky_thread_func(Pinky *pinky)
{
    double dt;
    while (pinky->alive)
    {
        dt = pinky->Tick();
        if (pinky->pauser->paused) continue;
        pinky->Update(dt);
    } 
}
Pinky::Pinky(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * pauser) : ECE_Ghost(nodes, pm, pauser)
{
    alive = true;
    SetStartPosition();
    thread = std::thread(pinky_thread_func, this);
    thread.detach();
}
void Pinky::ScatterGoal() { goal = ECE_Vector2(0, 21); } // upper left corner
void Pinky::ChaseGoal() // chases 4 tiles in front of pacman
{
    switch (pacman->direction)
    {
    case RIGHT: goal = ECE_Vector2(pacman->position.x + 4, pacman->position.y); break;
    case UP:    goal = ECE_Vector2(pacman->position.x, pacman->position.y + 4); break;
    case LEFT:  goal = ECE_Vector2(pacman->position.x - 4, pacman->position.y); break;
    case DOWN:  goal = ECE_Vector2(pacman->position.x, pacman->position.y - 4); break;
    
    default: break; // should never get here
    }
}
void Pinky::Draw()
{
    if (!visible) return;
    if (mode.mode == SPAWN) return;
    float color[4] = { 1.0f, .39f, .59f, 1.0f }; // pink
    if (mode.mode == FREIGHT)
    {
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
    }
    GLUquadricObj* qobj = gluNewQuadric();
    GLfloat shininess[] = { 5 };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(r, g, b);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(position.x, position.y, -0.25f); // move to correct position
        glRotatef(270.0, 0.0f, 0.0f, 1.0f); // rotate vertical
        gluCylinder(qobj, radius, radius, .5, 20, 5);
        glTranslatef(0.0f, 0.0f, 0.5f);
        glutSolidSphere(radius, 20, 20); // sphere
    glPopMatrix();
}
void Pinky::Update(double & dt)
{
    mutex.lock();
    // std::cout << "dt: " << dt << std::endl;
    visible = true;
    PortalSlowdown();
    double speedMod = speed * mode.speedMult;
    switch (direction)
    {
    case RIGHT: position.x += speedMod * dt; break;
    case UP:    position.y += speedMod * dt; break;
    case LEFT:  position.x -= speedMod * dt; break;
    case DOWN:  position.y -= speedMod * dt; break;
    
    default: break; // should never get here
    }
    ModeUpdate(dt);
    switch (mode.mode)
    {
    case CHASE: ChaseGoal(); break;
    case SCATTER: ScatterGoal(); break;
    case FREIGHT: RandomGoal(); break;
    case SPAWN: SpawnGoal(); break;
    
    default: break; // should never get here
    }
    MoveBySelf();
    
    mutex.unlock();
}
void Pinky::SetStartPosition()
{
    for (auto n : nodes->nodeList) {
        if (n->position.x == 9 && n->position.y == 11) {
            node = n;
            break;
        }
    }
    target = node;
    SetPosition();
}

void inky_thread_func(Inky *inky)
{
    double dt;
    while (inky->alive)
    {
        dt = inky->Tick();
        if (inky->pauser->paused) continue;
        inky->Update(dt);
    }
}
Inky::Inky(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * pauser) : ECE_Ghost(nodes, pm, pauser)
{
    alive = true;
    SetStartPosition();
    pelletsForRelease = 30;
    released = false;
    bannedDirections.push_back(UP);
    spawnNode = node;
    SetGuideStack();
    thread = std::thread(inky_thread_func, this);
    thread.detach();
}
void Inky::ScatterGoal() { goal = ECE_Vector2(18, 0); } // lower right corner
void Inky::ChaseGoal()
{
    ECE_Vector2 vec1;
    switch (pacman->direction) // get two tiles in front of pacman
    {
    case RIGHT: vec1 = ECE_Vector2(pacman->position.x + 2, pacman->position.y); break;
    case UP:    vec1 = ECE_Vector2(pacman->position.x, pacman->position.y + 2); break;
    case LEFT:  vec1 = ECE_Vector2(pacman->position.x - 2, pacman->position.y); break;
    case DOWN:  vec1 = ECE_Vector2(pacman->position.x, pacman->position.y - 2); break;
    
    default: break; // should never get here
    }
    ECE_Vector2 vec2 = (vec1 - blinky->position) * 2;
    goal = blinky->position + vec2;
}
void Inky::Draw()
{
    if (!visible) return;
    if (mode.mode == SPAWN) return;
    float color[4] = { .39f, 1.0f, 1.0f, 1.0f }; // teal
    if (mode.mode == FREIGHT)
    {
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
    }
    GLUquadricObj* qobj = gluNewQuadric();
    GLfloat shininess[] = { 5 };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(r, g, b);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(position.x, position.y, -0.25f); // move to correct position
        glRotatef(270.0, 0.0f, 0.0f, 1.0f); // rotate vertical
        gluCylinder(qobj, radius, radius, .5, 20, 5);
        glTranslatef(0.0f, 0.0f, 0.5f);
        glutSolidSphere(radius, 20, 20); // sphere
    glPopMatrix();
}
void Inky::Update(double & dt)
{
    mutex.lock();
    // std::cout << "dt: " << dt << std::endl;
    visible = true;
    PortalSlowdown();
    double speedMod = speed * mode.speedMult;
    switch (direction)
    {
    case RIGHT: position.x += speedMod * dt; break;
    case UP:    position.y += speedMod * dt; break;
    case LEFT:  position.x -= speedMod * dt; break;
    case DOWN:  position.y -= speedMod * dt; break;
    
    default: break; // should never get here
    }
    ModeUpdate(dt);
    switch (mode.mode)
    {
    case CHASE: ChaseGoal(); break;
    case SCATTER: ScatterGoal(); break;
    case FREIGHT: RandomGoal(); break;
    case SPAWN: SpawnGoal(); break;
    
    default: break; // should never get here
    }
    MoveBySelf();
    
    mutex.unlock();
}
void Inky::SetStartPosition()
{
    for (auto n : nodes->nodeList) {
        if (n->position.x == 8 && n->position.y == 11) {
            node = n;
            break;
        }
    }
    target = node;
    SetPosition();
}
void Inky::SetGuideStack()
{
    guide.push_back(RIGHT);
    guide.push_back(UP);
}

void clyde_thread_func(Clyde *clyde)
{
    double dt;
    while (clyde->alive)
    {
        dt = clyde->Tick();
        if (clyde->pauser->paused) continue;
        clyde->Update(dt);
    }
}
Clyde::Clyde(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * pauser) : ECE_Ghost(nodes, pm, pauser)
{
    alive = true;
    SetStartPosition();
    pelletsForRelease = 60;
    released = false;
    bannedDirections.push_back(UP);
    spawnNode = node;
    SetGuideStack();
    thread = std::thread(clyde_thread_func, this);
    thread.detach();
}
void Clyde::ScatterGoal() { goal = ECE_Vector2(0, 0); } // lower left corner
void Clyde::ChaseGoal()
{
    ECE_Vector2 d = pacman->position - position;
    double ds = d.magnitude_squared();
    if (ds <= 64) { // if closer than 8 tiles
        ScatterGoal();
    } else { // otherwise, act like Pinky
        switch (pacman->direction)
        {
        case RIGHT: goal = ECE_Vector2(pacman->position.x + 4, pacman->position.y); break;
        case UP:    goal = ECE_Vector2(pacman->position.x, pacman->position.y + 4); break;
        case LEFT:  goal = ECE_Vector2(pacman->position.x - 4, pacman->position.y); break;
        case DOWN:  goal = ECE_Vector2(pacman->position.x, pacman->position.y - 4); break;
    
        default: break; // should never get here
        }
    }
}
void Clyde::Draw()
{
    if (!visible) return;
    if (mode.mode == SPAWN) return;
    float color[4] = { .90f, .75f, .16f, 1.0f }; // orange
    if (mode.mode == FREIGHT)
    {
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
    }
    GLUquadricObj* qobj = gluNewQuadric();
    GLfloat shininess[] = { 5 };

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(r, g, b);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(position.x, position.y, -0.25f); // move to correct position
        glRotatef(270.0, 0.0f, 0.0f, 1.0f); // rotate vertical
        gluCylinder(qobj, radius, radius, .5, 20, 5);
        glTranslatef(0.0f, 0.0f, 0.5f);
        glutSolidSphere(radius, 20, 20); // sphere
    glPopMatrix();
}
void Clyde::Update(double & dt)
{
    mutex.lock();
    // std::cout << "dt: " << dt << std::endl;
    visible = true;
    PortalSlowdown();
    double speedMod = speed * mode.speedMult;
    switch (direction)
    {
    case RIGHT: position.x += speedMod * dt; break;
    case UP:    position.y += speedMod * dt; break;
    case LEFT:  position.x -= speedMod * dt; break;
    case DOWN:  position.y -= speedMod * dt; break;
    
    default: break; // should never get here
    }
    ModeUpdate(dt);
    switch (mode.mode)
    {
    case CHASE: ChaseGoal(); break;
    case SCATTER: ScatterGoal(); break;
    case FREIGHT: RandomGoal(); break;
    case SPAWN: SpawnGoal(); break;
    
    default: break; // should never get here
    }
    MoveBySelf();
    
    mutex.unlock();
}
void Clyde::SetStartPosition()
{
    for (auto n : nodes->nodeList) {
        if (n->position.x == 10 && n->position.y == 11) {
            node = n;
            break;
        }
    }
    target = node;
    SetPosition();
}
void Clyde::SetGuideStack()
{
    guide.push_back(LEFT);
    guide.push_back(UP);
}

ECE_GhostGroup::ECE_GhostGroup(ECE_NodeGroup &nodes, ECE_Pacman *pm, ECE_Pauser * pauser) : nodes(&nodes)
{
    ghosts.push_back(new Blinky(nodes, pm, pauser));
    ghosts.push_back(new Pinky(nodes, pm, pauser));
    ghosts.push_back(new Inky(nodes, pm, pauser));
    ghosts.push_back(new Clyde(nodes, pm, pauser));

    dynamic_cast<Inky*>(ghosts.at(2))->blinky = dynamic_cast<Blinky*>(ghosts.at(0));
}
ECE_GhostGroup::~ECE_GhostGroup()
{
    KillThreads();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    for (auto g : ghosts)
        delete g;
}

void ECE_GhostGroup::Update(double & dt)
{
    // dynamic_cast<Blinky*>(ghosts.at(0))->Update(dt);
    // dynamic_cast<Pinky*>(ghosts.at(1))->Update(dt);
    // dynamic_cast<Inky*>(ghosts.at(2))->Update(dt);
    // dynamic_cast<Clyde*>(ghosts.at(3))->Update(dt);
}
void ECE_GhostGroup::FreightMode()
{
    dynamic_cast<Blinky*>(ghosts.at(0))->FreightMode();
    dynamic_cast<Pinky*>(ghosts.at(1))->FreightMode();
    dynamic_cast<Inky*>(ghosts.at(2))->FreightMode();
    dynamic_cast<Clyde*>(ghosts.at(3))->FreightMode();
}
void ECE_GhostGroup::UpdatePoints()
{
    dynamic_cast<Blinky*>(ghosts.at(0))->points *= 2;
    dynamic_cast<Pinky*>(ghosts.at(1))->points *= 2;
    dynamic_cast<Inky*>(ghosts.at(2))->points *= 2;
    dynamic_cast<Clyde*>(ghosts.at(3))->points *= 2;
}
void ECE_GhostGroup::ResetPoints()
{
    dynamic_cast<Blinky*>(ghosts.at(0))->points = 200;
    dynamic_cast<Pinky*>(ghosts.at(1))->points = 200;
    dynamic_cast<Inky*>(ghosts.at(2))->points = 200;
    dynamic_cast<Clyde*>(ghosts.at(3))->points = 200;
}
void ECE_GhostGroup::Hide()
{
    dynamic_cast<Blinky*>(ghosts.at(0))->visible = false;
    dynamic_cast<Pinky*>(ghosts.at(1))->visible = false;
    dynamic_cast<Inky*>(ghosts.at(2))->visible = false;
    dynamic_cast<Clyde*>(ghosts.at(3))->visible = false;
}
void ECE_GhostGroup::Draw()
{
    dynamic_cast<Blinky*>(ghosts.at(0))->Draw();
    dynamic_cast<Pinky*>(ghosts.at(1))->Draw();
    dynamic_cast<Inky*>(ghosts.at(2))->Draw();
    dynamic_cast<Clyde*>(ghosts.at(3))->Draw();
}
void ECE_GhostGroup::Release(int numEaten)
{
    Inky *i = dynamic_cast<Inky*>(ghosts.at(2));
    Clyde *c = dynamic_cast<Clyde*>(ghosts.at(3));

    if (!(i->released))
        if (numEaten >= i->pelletsForRelease) {
            i->bannedDirections.clear();
            i->SpawnMode();
            i->released = true;
        }
    if (!(c->released))
        if (numEaten >= c->pelletsForRelease) {
            c->bannedDirections.clear();
            c->SpawnMode();
            c->released = true;
        }
}
// we need to kill the thread before we destruct the ghost so that the 
// ghost thread doesn't accidentally use an invalid pointer and seg fault
void ECE_GhostGroup::KillThreads() 
{
    for (auto g : ghosts)
    {
        g->alive = false;
    }
}
