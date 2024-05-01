#include "graph.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

Vertex::Vertex(){
    this->id = -1;
    this->x = 0;
    this->y = 0;
}


Vertex::Vertex(int id, float x, float y ){
    this->id = id;
    this->x = x;
    this->y = y;
}

Edge::Edge(Vertex v, float w){
    to = v;
    weight = w;
}

Graph::Graph(){
    vertices = 0;
    adjList = vector<vector<Edge>>();
}

Graph::Graph(int v){
    vertices = v;
    adjList.resize(v);
}

void Graph::addVertex(int id, float xpos, float ypos){
    Vertex v = Vertex(id, xpos, ypos);
    if (vertexMap.find(id) == vertexMap.end()) {
        vertexList.push_back(v);
        vertexMap[id] = v;
    }
}

int Graph::getIndex(Vertex v){
    for (int i = 0; i < vertexList.size(); i++) {
        if (vertexList[i].id == v.id) {
            return i;
        }
    }
    return -1;
}

Vertex Graph::getVertex(int id){
    for (int i = 0; i < vertexList.size(); i++) {
        if (vertexList[i].id == id) {
            return vertexList[i];
        }
    }
    return vertexList[0];
}

void Graph::addEdge(Vertex origin, Vertex destination, float weight) {
    int originIdx = getIndex(origin);
    if (originIdx != -1) {
        if (originIdx >= adjList.size()) {
            adjList.resize(originIdx + 1); // Resize if necessary
        }
        Edge e = Edge(destination, weight);
        adjList[originIdx].push_back(e);

    } else {
        cerr << "Origin vertex not found" << endl;
    }
}

void Graph::printGraph(){
    for (int i = 0; i < adjList.size(); i++) {
        cout << "Vertex  " << vertexList[i].id << " which is at :" << vertexList[i].x << "   " << vertexList[i].y << " is connected to:";
        for (const Edge &e: adjList[i]){
            cout << "->" << e.to.id << " with a weight of " << e.weight << endl;
        }
        cout << endl;
    }
}

vector<Edge> Graph::getNeighbors(Vertex v) {
    int idx = getIndex(v);
    if (idx != -1) {
        return adjList[idx];
    } else {
        std::cerr << "Vertex not found in graph." << std::endl;
        return std::vector<Edge>();
    }
}

Graph createGraphFromTxt(string filename){
    Graph graph = Graph();
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "CSV IS FAILING TO OPEN:   " << filename << endl;
        return graph;
    }

    string line;
    while (getline(file, line)) {
        if (line == "###################") {
            break;
        }
        stringstream lineStream(line);
        string idStr, xStr, yStr;

        getline(lineStream, idStr, ' ');
        getline(lineStream, xStr, ' ');
        getline(lineStream, yStr, ' ');
        graph.addVertex(stoi(idStr), stof(xStr), stof(yStr));
    }

    while (getline(file, line)) {
        stringstream lineStream(line);
        string id1Str, id2Str, wStr;

        getline(lineStream, id1Str, ' ');
        getline(lineStream, id2Str, ' ');
        getline(lineStream, wStr, ' ');

        Vertex v1 = graph.vertexMap[stoi(id1Str)];
        Vertex v2 = graph.vertexMap[stoi(id2Str)];

        graph.addEdge(v1, v2, stof(wStr));
    }

    file.close();
    return graph;
}

bool Graph::hasVertexNearby(sf::Vector2f& point, float radius){
    for (const Vertex& vertex : vertexList) {
        float distance = std::sqrt(std::pow(point.x - vertex.x, 2) + std::pow(point.y - vertex.y, 2));
        if (distance < radius) {
            return true; // There is a vertex nearby
        }
    }
    return false; // No nearby vertices found
}

void Graph::printGraphToFile(string filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Failed to open file for writing: " << filename << endl;
        return;
    }

    // Print vertices
    for (const Vertex& vertex : vertexList) {
        outFile<< vertex.id << " " << vertex.x << " " << vertex.y << endl;
    }
    outFile << "###################" << endl;

    // Print edges
    for (int i = 0; i < adjList.size(); ++i) {
        for (const Edge& edge : adjList[i]) {
            outFile << vertexList[i].id << " " << edge.to.id << " " << edge.weight << endl;
        }
    }

    outFile.close();
    cout << "Graph printed to file: " << filename << endl;
}


