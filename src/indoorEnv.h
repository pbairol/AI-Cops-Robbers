#ifndef INDOOR_ENVIRONMENT_H
#define INDOOR_ENVIRONMENT_H

#include <vector>
#include <string>
#include <cmath>
#include "graph.h"
#include "structs.h"

using namespace std;

const int GRID_WIDTH = 40;  // Number of grid cells horizontally
const int GRID_HEIGHT = 30; // Number of grid cells vertically
const int CELL_SIZE = 20;   // Size of each grid cell

class IndoorEnvironment
{
private:
    sf::Vector2f characterPos;
    sf::Vector2f enemyPos;
    sf::Vector2f bedPosition = sf::Vector2f(50, 450);
    sf::Vector2f tvPosition = sf::Vector2f(600, 500);
    sf::Vector2f burgerPosition = sf::Vector2f(660, 50);
    sf::Vector2f dsPosition = sf::Vector2f(100, 100);
    vector<vector<bool>>grid;

public:
    IndoorEnvironment();
    bool cangotoTV = true;
    sf::Vector2f targetPos = sf::Vector2f(0, 0);
    void toggleObstacle(int row, int col);
    bool isObstacle(int row, int col);
    void saveToFile(const string &filename);
    void loadFromFile(const string &filename);
    Graph generateGraph();
    Vertex findClosestVertex(Graph g, float x, float y);

    

    sf::Vector2f getBedPosition();
    sf::Vector2f getTVPosition();
    sf::Vector2f getBurgerPosition();
    sf::Vector2f getDSPosition();
    sf::Vector2f getCharacterPos();
    void setCharacterPos(sf::Vector2f & pos);
    sf::Vector2f getEnemyPos();
    void setEnemyPos(sf::Vector2f & pos);
};

#endif
