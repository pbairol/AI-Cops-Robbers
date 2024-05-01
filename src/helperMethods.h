#ifndef HELPER_METHODS_H
#define HELPER_METHODS_H
#include <SFML/Graphics.hpp>
#include "graph.h"
#include <vector>

// function declarations for helper methods
float mapToRange(float theta); // maps angle to the range, clips it between negative pi and pi 
float magnitude(sf::Vector2f vec); // calculates magnitude of a 2D vector ( distance)
float randomBinomial(); // generates a random number between -1 and 1 , needed for wander and other algorithms 
sf::Vector2f normalize(sf::Vector2f vec); // normalizes a 2D vector (makes its magnitude 1)
sf::Vector2f orientationToDirection(float orientation); // converts orientation (angle) to a 2D direction vector
float distanceFunc(const sf::Vector2f v1, const sf::Vector2f v2);
#endif