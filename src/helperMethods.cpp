/**
 * Helper Method CPP that has some helper methods that are used throghout this homeowrk assignment
 * author Pratik Bairoliya pbairol@ncsu.edu
 */
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "structs.h"
#include "helperMethods.h"
using namespace std;

// function to map angle to the range [-pi, pi] this is to like trim the angle down
float mapToRange(float theta)
{

    theta = fmod(theta, 2 * M_PI); // Using fmod to ensure theta is in the range [0, 2*pi)
    if (abs(theta) <= M_PI)
    {
        return theta;
    }
    else if (theta > M_PI)
    {
        return (theta - (2 * M_PI)); // Wrap around the angle if it's greater than pi
    }
    else
    {
        return (theta + (2 * M_PI)); // Wrap around the angle if it's less than -pi
    }
}

// function to calculate the magnitude of a 2D vector this is to help with distnaces of boids needed for alot of different other methods
float magnitude(sf::Vector2f vec)
{
    return sqrt((vec.x * vec.x) + (vec.y * vec.y)); // Calculating magnitude using Euclidean distance formula
}

// functions to generate a random number between -1 and 1  returns a float this is used for wander and other steering behaviors.
float randomBinomial()
{
    float rand1 = static_cast<float>(random()) / RAND_MAX; // Generating random number between 0 and 1
    float rand2 = static_cast<float>(random()) / RAND_MAX; // Generating another random number between 0 and 1
    return rand1 - rand2;                                  // Subtracting the two random numbers to get a number between -1 and 1
}

// Function to normalize a 2D vector  needed for some steering behavior classes
sf::Vector2f normalize(sf::Vector2f vec)
{
    float mag = magnitude(vec); // Calculate magnitude of the vector
    if (mag != 0.0f)            // Check if magnitude is not zero to avoid division by zero
    {
        return vec / mag; // Normalize the vector by dividing each component by its magnitude
    }
    return vec; // Return the original vector if its magnitude is zero
}

// function to convert orientation which is a float to a 2D direction vector needed for wandering and other algoirhtms
sf::Vector2f orientationToDirection(float orientation)
{
    float x = cos(orientation); // Calculate x component of the direction vector
    float y = sin(orientation); // Calculate y component of the direction vector

    return normalize(sf::Vector2f(x, y)); // Return normalized direction vector
}

float distanceFunc(const sf::Vector2f v1, const sf::Vector2f v2) {
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    return sqrt(dx * dx + dy * dy); 
}
