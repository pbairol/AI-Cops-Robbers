#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "graph.h"

class heuristic {
public:
    virtual float h(Vertex* n, Vertex * end) = 0;
};

class manhattanHeuristic : public heuristic {
public:
   
    float h(Vertex* n, Vertex * end);
};

class euclideanHeuristic : public heuristic {
public:
    
    float h(Vertex* n, Vertex * end);

};

#endif
