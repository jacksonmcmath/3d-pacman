/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Node declarations
*/

#pragma once

#include <map>
#include <stack>
#include <vector>

#include "ECE_Vector2.h"

enum ECE_Direction { STOP, RIGHT, UP, LEFT, DOWN };

// A class to represent nodes on the map
class ECE_Node
{
public:

    ECE_Node(int x, int y);
    ~ECE_Node() {};

    ECE_Vector2 position;
    
    std::map<ECE_Direction, ECE_Node*> neighbors;
    ECE_Node *portalNode;
    int portalVal;
    bool homeEntrance;
    bool spawnNode;

    void Draw();

    inline bool operator==(const ECE_Node& rhs) { return (position.x == rhs.position.x && position.y == rhs.position.y); };

};


class ECE_NodeGroup
{
public:

    ECE_NodeGroup();
    ~ECE_NodeGroup();
    
    static const int ROWS = 22;
    static const int COLS = 19;
    static const int map[ROWS][COLS];

    std::vector<ECE_Node*> nodeList;
    std::stack<ECE_Node*> nodeStack;

    void Draw();

    void CreateNodeList();
    void SetupPortalNodes();

private:

    ECE_Node* FindFirstNode();
    ECE_Node* GetNode(int x, int y);
    ECE_Node* GetNodeFromNode(ECE_Node* node);
    ECE_Node* GetPathNode(ECE_Direction dir, int row, int col);
    bool AddNode(ECE_Node* node);
    bool AddNodeToStack(ECE_Node* node);
    bool NodeInList(ECE_Node* node);
    ECE_Node* FollowPath(ECE_Direction dir, int row, int col);
    ECE_Node* PathToFollow(ECE_Direction dir, int row, int col, int path);

};
