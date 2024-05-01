#include "heuristic.h"
#include <cmath>

using namespace std;

float manhattanHeuristic::h(Vertex *n, Vertex *end) {
    float xDiff = abs(n->x - end->x);
    float yDiff = abs(n->y - end->y);

    return static_cast<float>(xDiff + yDiff);
}

float euclideanHeuristic::h(Vertex *n, Vertex *end) {
    float xDiff = n->x - end->x;
    float yDiff = n->y - end->y;

    return sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}
