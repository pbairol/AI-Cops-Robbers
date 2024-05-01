#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>


using namespace std;

struct Vertex {
    int id;
    float x;
    float y;

    Vertex();
    Vertex(int id, float x, float y);
};

struct Edge {
    Vertex to;
    float weight;

    Edge(Vertex v, float w);
};

class Graph {
public:
    int vertices;
    vector<Vertex> vertexList;
    vector<vector<Edge>> adjList;
    unordered_map<int, Vertex> vertexMap;

    Graph();
    Graph(int v);

    void addVertex(int id, float xpos, float ypos);
    int getIndex(Vertex v);
    Vertex getVertex(int id);
    void addEdge(Vertex origin, Vertex destination, float weight);
    void printGraph();
    vector<Edge> getNeighbors(Vertex v); 
    bool hasVertexNearby(sf::Vector2f& point, float radius);
    void printGraphToFile(string filename);
};

Vertex vertexFromId(int id, Graph g);
Graph createGraphFromTxt(string filename);

#endif // GRAPH_H
