/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Node definitions
*/

#if __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <map>
#include <utility> // for std::pair

#include "ECE_Node.h"

// #include <iostream>

ECE_Node::ECE_Node(int x, int y)
{
    position = ECE_Vector2(x, y);

    neighbors[RIGHT] = NULL;
    neighbors[UP]    = NULL;
    neighbors[LEFT]  = NULL;
    neighbors[DOWN]  = NULL;

    portalNode = NULL;
    portalVal = 0;
}

void ECE_Node::Draw()
{
    // draw sphere at node

    GLfloat shininess[] = { 5 };
    float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // rgb(255, 0, 0)

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    // glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
        // glRotatef((GLfloat)deg, 0.0f, 0.0f, 1.0f); // global rotation
        // glTranslatef(-9.0f, -11.0f, 0.0f); // center @ origin
        glTranslatef(position.x, position.y, 0.0f); // move to correct position
        glutSolidSphere(.3, 20, 20); // sphere
    glPopMatrix();

    // draw line to each neighbor
    for (auto const& n : neighbors)
    {
        if (n.second == NULL) continue;
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(position.x, position.y, 0.0f);
        glVertex3f(n.second->position.x, n.second->position.y, 0.0f);
        glEnd();
    }
}


// 0 - blank
// 1 - node
// 2 - vertical path
// 3 - horizontal path
// 4 - ghost house gate
// 5 - portal
// 6 - spawn node

const int ECE_NodeGroup::map[ECE_NodeGroup::ROWS][ECE_NodeGroup::COLS] = 
    { // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0 
        {0, 1, 3, 3, 3, 3, 3, 3, 1, 3, 1, 3, 3, 3, 3, 3, 3, 1, 0}, // 1 
        {0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0}, // 2 
        {0, 1, 1, 3, 1, 0, 1, 3, 1, 0, 1, 3, 1, 0, 1, 3, 1, 1, 0}, // 3 
        {0, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 0}, // 4 
        {0, 1, 1, 0, 1, 3, 1, 3, 1, 1, 1, 3, 1, 3, 1, 0, 1, 1, 0}, // 5 
        {0, 2, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0}, // 6 
        {0, 1, 3, 3, 1, 3, 1, 3, 1, 0, 1, 3, 1, 3, 1, 3, 3, 1, 0}, // 7 
        {0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0}, // 8 
        {0, 0, 0, 0, 2, 0, 1, 3, 3, 3, 3, 3, 1, 0, 2, 0, 0, 0, 0}, // 9 
        {0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0}, // 10
        {5, 3, 3, 3, 1, 3, 1, 0, 1, 6, 1, 0, 1, 3, 1, 3, 3, 3, 5}, // 11
        {0, 0, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 2, 0, 2, 0, 0, 0, 0}, // 12
        {0, 0, 0, 0, 2, 0, 1, 3, 1, 4, 1, 3, 1, 0, 2, 0, 0, 0, 0}, // 13
        {0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0}, // 14
        {0, 1, 3, 3, 1, 0, 1, 3, 1, 0, 1, 3, 1, 0, 1, 3, 3, 1, 0}, // 15
        {0, 2, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 2, 0}, // 16
        {0, 1, 3, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 3, 1, 0}, // 17
        {0, 2, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0}, // 18
        {0, 2, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0}, // 19
        {0, 1, 3, 3, 1, 3, 3, 3, 1, 0, 1, 3, 3, 3, 1, 3, 3, 1, 0}, // 20
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 21
    };


ECE_NodeGroup::ECE_NodeGroup()
{
    CreateNodeList();
    SetupPortalNodes();
}

ECE_NodeGroup::~ECE_NodeGroup()
{
    for (auto n : nodeList)
        delete n;
}

void ECE_NodeGroup::Draw()
{
    for (auto n : nodeList)
        n->Draw();
}

void ECE_NodeGroup::CreateNodeList()
{
    // a very good place to start...
    ECE_Node *startNode = FindFirstNode();
    nodeStack.push(startNode);
    while (!nodeStack.empty()) // loop until the stack is empty
    {
        ECE_Node *node = nodeStack.top();
        nodeStack.pop();

        AddNode(node); // add to list

        // fetch neighbor nodes (NULL if non-existent)
        ECE_Node *rightNode = GetPathNode(RIGHT, node->position.y,     node->position.x + 1);
        ECE_Node *upNode    = GetPathNode(UP,    node->position.y + 1, node->position.x);
        ECE_Node *leftNode  = GetPathNode(LEFT,  node->position.y,     node->position.x - 1);
        ECE_Node *downNode  = GetPathNode(DOWN,  node->position.y - 1, node->position.x);
        node->neighbors[RIGHT] = rightNode;
        node->neighbors[UP]    = upNode;
        node->neighbors[LEFT]  = leftNode;
        node->neighbors[DOWN]  = downNode;

        AddNodeToStack(rightNode);
        AddNodeToStack(upNode);
        AddNodeToStack(leftNode);
        AddNodeToStack(downNode);
    }
    // for (auto n : nodeList)
    //     std::cout << "list node(" << n->x << ", " << n->y << ")" << std::endl;
}

void ECE_NodeGroup::SetupPortalNodes()
{
    // a dict mapping portalVal to a pair of ECE_Node pointers
    std::map<int, std::pair<ECE_Node*, ECE_Node*> > portalDict;
    // pair of ECE_Node pointers
    std::pair<ECE_Node*, ECE_Node*> portalPair;

    // save the portal nodes into a dictionary based on portalVal
    for (auto n : nodeList)
        if (n->portalVal != 0)
        {
            if (portalDict.count(n->portalVal))
            { // key found
                portalDict[n->portalVal].second = n;
            }
            else
            { // key not found
                portalPair.first = n;
                portalDict[n->portalVal] = portalPair;
            }
        }
    // link portals with the same portalVal together
    for (auto const& x : portalDict)
    {
        // x.first is the portalValue integer
        // x.second is the pair of ECE_Node pointers (also accessed with .first and .second)
        x.second.first->portalNode = x.second.second;
        x.second.second->portalNode = x.second.first;
    }


}

ECE_Node* ECE_NodeGroup::FindFirstNode()
{
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
            if (map[y][x] == 1 || map[y][x] == 5 || map[y][x] == 4) // if node or portal or home entrance
            {
                ECE_Node *node = new ECE_Node(x, y);
                if (map[y][x] == 5) // if portal
                    node->portalVal = map[y][x];
                if (map[y][x] == 4) // if home entrance
                    node->homeEntrance = true;
                if (map[y][x] == 6) // if spawn node
                    node->spawnNode = true;
                return node;
            }
    return NULL;
}

ECE_Node* ECE_NodeGroup::GetNode(int x, int y)
{
    for (auto n : nodeList)
        if (n->position.x == x && n->position.y == y)
            return n;
    return NULL;
}

ECE_Node* ECE_NodeGroup::GetNodeFromNode(ECE_Node * node)
{
    if (node != NULL)
        for (auto n : nodeList)
            if (*node == *n)
            {
                // printf("deleting node(%d, %d)\n", node->x, node->y);
                delete node; // the node already exists in the list, so delete it
                return n;   // and return the one in the list
            }
    return node;
}

ECE_Node* ECE_NodeGroup::GetPathNode(ECE_Direction dir, int row, int col)
{
    ECE_Node *tmp = FollowPath(dir, row, col);
    return GetNodeFromNode(tmp);
}

bool ECE_NodeGroup::AddNode(ECE_Node * node)
{
    if (node != NULL && !NodeInList(node))
    {
        nodeList.push_back(node);
        return true;
    }
    return false;
}

bool ECE_NodeGroup::AddNodeToStack(ECE_Node * node)
{
    if (node != NULL && !NodeInList(node))
    {
        nodeStack.push(node);
        return true;
    }
    return false;
}

bool ECE_NodeGroup::NodeInList(ECE_Node* node)
{
    if (node == NULL) return false;
    for (auto n : nodeList)
        if (*node == *n)
            return true;
    return false;
}

ECE_Node* ECE_NodeGroup::FollowPath(ECE_Direction dir, int row, int col)
{
    if (dir == RIGHT && col < COLS)
        return PathToFollow(RIGHT, row, col, 3);
    if (dir == UP && row < ROWS)
        return PathToFollow(UP, row, col, 2);
    if (dir == LEFT && col >= 0)
        return PathToFollow(LEFT, row, col, 3);
    if (dir == DOWN && row >= 0)
        return PathToFollow(DOWN, row, col, 2);
    return NULL;
}

ECE_Node* ECE_NodeGroup::PathToFollow(ECE_Direction dir, int row, int col, int path)
{
    if (map[row][col] == path || map[row][col] == 1 || map[row][col] == 5 || map[row][col] == 4 || map[row][col] == 6)
    {
        // while not at a node or portal or home entrance or spawn node
        while (map[row][col] != 1 && map[row][col] != 5 && map[row][col] != 4 && map[row][col] != 6) 
            switch (dir)
            {
            case RIGHT: col++; break;
            case UP:    row++; break;
            case LEFT:  col--; break;
            case DOWN:  row--; break;
            default: break;
            }
        // printf("creating node(%d, %d)\n", col, row);
        ECE_Node *node = new ECE_Node(col, row);
        if (map[row][col] == 5) // if portal
            node->portalVal = map[row][col];
        if (map[row][col] == 4) // if home entrance
            node->homeEntrance = true;
        if (map[row][col] == 6) // if spawn node
            node->spawnNode = true;
        return node;
    }
    return NULL;
}
