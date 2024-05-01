#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "graph.h"
#include "heuristic.h"
#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string.h>
#include <functional>
#include <algorithm>

class PathFinding
{
private:
    // Custom comparator for the priority queue
    struct CompareVertices
    {
        bool operator()(const pair<float, Vertex> &a, const pair<float, Vertex> &b)
        {
            // Compare vertices based on their distances
            return a.first > b.first;
        }
    };

public:
    int closedLength = 0;
    int openLength = 0;
    vector<Vertex> aStar(Graph *g, Vertex *start, Vertex *end, heuristic *heuristicFunc)
    {
        // Initialize vectors to store visited vertices, distances, and parent vertices
        unordered_map<int, bool> visited;
        unordered_map<int, float> distance;
        unordered_map<int, int> parent;

        // Initialize priority queue with custom comparator
        priority_queue<pair<float, Vertex>, vector<pair<float, Vertex>>, CompareVertices> pq;

        // Initialize distances to infinity for all vertices
        for (const auto &v : g->vertexMap)
        {
            distance[v.first] = numeric_limits<float>::max();
            visited[v.first] = false;
        }

        // Set distance to start vertex to 0 and push it into the priority queue
        distance[start->id] = 0;
        pq.push({0 + heuristicFunc->h(start, end), *start}); // Total cost = distance + heuristic

        while (!pq.empty())
        {
            // Extract the vertex with minimum total cost
            Vertex current = pq.top().second;
            pq.pop();

            // If the vertex is already visited, skip it
            if (visited[current.id])
                continue;

            // Mark the current vertex as visited
            visited[current.id] = true;
            this->closedLength += 1;

            // If the current vertex is the destination, break the loop
            if (current.id == end->id)
                break;

            // Traverse through all neighboring vertices
            for (const auto &edge : g->getNeighbors(current))
            {
                Vertex neighbor = edge.to;
                float newDistance = distance[current.id] + edge.weight;

                // If the new distance is shorter, update the distance and push into the priority queue
                if (newDistance < distance[neighbor.id])
                {
                    distance[neighbor.id] = newDistance;
                    parent[neighbor.id] = current.id;
                    pq.push({newDistance + heuristicFunc->h(&neighbor, end), neighbor}); // Total cost = distance + heuristic
                }
            }
        }

        // Reconstruct the path from start to end
        vector<Vertex> path;
        Vertex current = *end;
        while (current.id != start->id)
        {
            path.push_back(g->getVertex(current.id));
            current.id = parent[current.id];
        }
        path.push_back(*start);
        reverse(path.begin(), path.end());
        this->openLength = pq.size();
        return path;
    }

    vector<Vertex> dijkstras(Graph *g, Vertex *start, Vertex *end)
    {
        // Initialize vectors to store visited vertices and distances
        unordered_map<int, bool> visited;
        unordered_map<int, float> distance;
        unordered_map<int, int> parent;

        // Initialize priority queue with custom comparator
        priority_queue<pair<float, Vertex>, vector<pair<float, Vertex>>, CompareVertices> pq;

        // Initialize distances to infinity for all vertices
        for (const auto &v : g->vertexMap)
        {
            distance[v.first] = numeric_limits<float>::max();
            visited[v.first] = false;
        }

        // Set distance to start vertex to 0 and push it into the priority queue
        distance[start->id] = 0;
        pq.push({0, *start});

        while (!pq.empty())
        {
            // Extract the vertex with minimum distance
            Vertex current = pq.top().second;
            pq.pop();

            // If the vertex is already visited, skip it
            if (visited[current.id])
                continue;

            // Mark the current vertex as visited
            visited[current.id] = true;
            this->closedLength += 1;

            // If the current vertex is the destination, break the loop
            if (current.id == end->id)
                break;

            // Traverse through all neighboring vertices
            for (const auto &edge : g->getNeighbors(current))
            {
                Vertex neighbor = edge.to;
                float newDistance = distance[current.id] + edge.weight;

                // If the new distance is shorter, update the distance and push into the priority queue
                if (newDistance < distance[neighbor.id])
                {
                    distance[neighbor.id] = newDistance;
                    parent[neighbor.id] = current.id;
                    pq.push({newDistance, neighbor});
                }
            }
        }

        // Reconstruct the path from start to end
        vector<Vertex> path;
        Vertex current = *end;
        while (current.id != start->id)
        {
            path.push_back(g->getVertex(current.id));
            current.id = parent[current.id];
        }
        path.push_back(*start);
        this->openLength = pq.size();
        reverse(path.begin(), path.end());

        return path;
    }
};

#endif
