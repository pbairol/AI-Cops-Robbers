#include "indoorEnv.h"
#include <fstream>
#include <iostream>

IndoorEnvironment::IndoorEnvironment()
{
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        std::vector<bool> row;
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            row.push_back(false);
        }
        grid.push_back(row);
    }
}

void IndoorEnvironment::toggleObstacle(int row, int col)
{
    if (row >= 0 && row < GRID_HEIGHT && col >= 0 && col < GRID_WIDTH)
    {
        grid[row][col] = !grid[row][col];
    }
}

bool IndoorEnvironment::isObstacle(int row, int col)
{
    return grid[row][col];
}

void IndoorEnvironment::saveToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            file << (grid[i][j] ? "1" : "0") << " ";
        }
        file << std::endl;
    }

    file.close();
}

void IndoorEnvironment::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return;
    }

    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            int val;
            file >> val;
            grid[i][j] = (val == 1);
        }
    }

    file.close();
}

Graph IndoorEnvironment::generateGraph()
{
    Graph graph;
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            if (grid[i][j]) // Ignore black cells
                continue;

            // Add vertex for white cell with position
            int vertexId = i * GRID_WIDTH + j;
            graph.addVertex(vertexId, j * CELL_SIZE, i * CELL_SIZE);

            // Check neighboring cells
            if (i > 0 && !grid[i - 1][j]) // Up
                graph.addEdge(Vertex(vertexId, j * CELL_SIZE, i * CELL_SIZE), Vertex((i - 1) * GRID_WIDTH + j, j * CELL_SIZE, (i - 1) * CELL_SIZE), 1.0);
            if (i < GRID_HEIGHT - 1 && !grid[i + 1][j]) // Down
                graph.addEdge(Vertex(vertexId, j * CELL_SIZE, i * CELL_SIZE), Vertex((i + 1) * GRID_WIDTH + j, j * CELL_SIZE, (i + 1) * CELL_SIZE), 1.0);
            if (j > 0 && !grid[i][j - 1]) // Left
                graph.addEdge(Vertex(vertexId, j * CELL_SIZE, i * CELL_SIZE), Vertex(i * GRID_WIDTH + (j - 1), (j - 1) * CELL_SIZE, i * CELL_SIZE), 1.0);
            if (j < GRID_WIDTH - 1 && !grid[i][j + 1]) // Right
                graph.addEdge(Vertex(vertexId, j * CELL_SIZE, i * CELL_SIZE), Vertex(i * GRID_WIDTH + (j + 1), (j + 1) * CELL_SIZE, i * CELL_SIZE), 1.0);

            // Check diagonally neighboring cells
            if (i > 0 && j > 0 && !grid[i - 1][j - 1]) // Up Left
                graph.addEdge(Vertex(vertexId, j * CELL_SIZE, i * CELL_SIZE), Vertex((i - 1) * GRID_WIDTH + (j - 1), (j - 1) * CELL_SIZE, (i - 1) * CELL_SIZE), sqrt(2.0));
            if (i > 0 && j < GRID_WIDTH - 1 && !grid[i - 1][j + 1]) // Up Right
                graph.addEdge(Vertex(vertexId, j * CELL_SIZE, i * CELL_SIZE), Vertex((i - 1) * GRID_WIDTH + (j + 1), (j + 1) * CELL_SIZE, (i - 1) * CELL_SIZE), sqrt(2.0));
            if (i < GRID_HEIGHT - 1 && j > 0 && !grid[i + 1][j - 1]) // Down Left
                graph.addEdge(Vertex(vertexId, j * CELL_SIZE, i * CELL_SIZE), Vertex((i + 1) * GRID_WIDTH + (j - 1), (j - 1) * CELL_SIZE, (i + 1) * CELL_SIZE), sqrt(2.0));
            if (i < GRID_HEIGHT - 1 && j < GRID_WIDTH - 1 && !grid[i + 1][j + 1]) // Down Right
                graph.addEdge(Vertex(vertexId, j * CELL_SIZE, i * CELL_SIZE), Vertex((i + 1) * GRID_WIDTH + (j + 1), (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE), sqrt(2.0));
        }
    }
    return graph;
}

Vertex IndoorEnvironment::findClosestVertex(Graph g, float x, float y)
{
    float minDistance = std::numeric_limits<float>::max();
    Vertex closestVertex;

    for (const auto &vertex : g.vertexList)
    {
        float distance = std::sqrt(std::pow(vertex.x - x, 2) + std::pow(vertex.y - y, 2));
        if (distance < minDistance)
        {
            minDistance = distance;
            closestVertex = vertex;
        }
    }
    return closestVertex;
}

sf::Vector2f IndoorEnvironment::getBurgerPosition()
{
    return burgerPosition;
}

sf::Vector2f IndoorEnvironment::getTVPosition()
{
    return tvPosition;
}
sf::Vector2f IndoorEnvironment::getBedPosition()
{
    return bedPosition;
}
sf::Vector2f IndoorEnvironment::getDSPosition()
{
    return dsPosition;
}

sf::Vector2f IndoorEnvironment::getCharacterPos()
{
    return characterPos;
}
void IndoorEnvironment::setCharacterPos(sf::Vector2f &cp)
{
    characterPos = cp;
}
sf::Vector2f IndoorEnvironment::getEnemyPos()
{
    return enemyPos;
}
void IndoorEnvironment::setEnemyPos(sf::Vector2f &ep)
{
    enemyPos = ep;
}
