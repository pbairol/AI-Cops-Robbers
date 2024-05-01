#ifndef STRUCTS_H
#define STRUCTS_H

#include <SFML/Graphics.hpp>
#include <vector>

#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <random>
#include <chrono>
#include "boidState.h"
using namespace std;
using namespace std::chrono;

// Define the number of breadcrumbs and loop offset
const int numberOfCrumbs = 9;
const int loopOffSet = 1;

// Struct representing kinematic properties of an object
struct Kinematic
{
    sf::Vector2f position; // Position of the object
    float orientation;     // Orientation (rotation) of the object
    sf::Vector2f velocity; // Velocity of the object
    float rotation;        // Rotation rate of the object

    // Constructor to initialize kinematic properties
    Kinematic(sf::Vector2f pos, float orient, sf::Vector2f vel, float rot);

    // Method to update kinematic properties
    void update(sf::Vector2f vel, float rot, float time);

    // Method to check boundary and adjust position if necessary
    void checkBoundary(float width, float height);
};

// Struct representing steering behaviors
struct Steering
{
    sf::Vector2f linearAcceleration;
    float angularAcceleration;

    // Constructor to initialize steering properties
    Steering(sf::Vector2f linAcc, float angAcc);

    // Method to update steering properties
    void update(sf::Vector2f vel, float rot, float time);
};

// Class representing a breadcrumb
// Adapted from a sample code provided by Derek
class crumb : public sf::CircleShape
{
public:
    // Constructor to create a breadcrumb with given id and radius
    crumb(int id, float r);

    // Method to render the breadcrumb using the given render window
    void draw(sf::RenderWindow *window);

    // Method to set the position of the breadcrumb
    void drop(sf::Vector2f position);

private:
    int id; // Id of the breadcrumb
};

// Class representing a boid
class Boid
{
public:
    int tiredness;   // Tiredness level
    int hunger;      // Hunger level
    int skillLevel;  // Skill level
    int timeSitting; // phsyical count of how long boid is sitting;
    int timeDancing; // phsyical count of how long boid is dancing;
    SteeringState boidSteeringState = SteeringState::SIT;
    bool canGoToTv = true;
    steady_clock::time_point lastUpdateTime;
    sf::Sprite sprite;        // Sprite representing the boid
    Kinematic kinematic;      // Kinematic properties of the boid
    sf::RenderWindow *window; // Pointer to the render window
    float drop_timer;         // Timer for breadcrumb dropping
    int crumb_idx;            // Index for the breadcrumb vector

    // Pointer to the vector of breadcrumbs
    vector<crumb> *breadcrumbs;
    Boid();
    // cnstructor to initialize the boid without breadcrumbs
    Boid(sf::RenderWindow *w, sf::Texture &tex);

    // cnstructor to initialize the boid with breadcrumbs
    Boid(sf::RenderWindow *w, sf::Texture &tex, std::vector<crumb> *crumbs);

    // method to set kinematic properties of the boid
    void setKinematic(const Kinematic &kin);

    // method to get kinematic properties of the boid
    Kinematic getKinematic();

    // method to get the sprite of the boid
    sf::Sprite getSprite();

    // method to move the boid and drop breadcrumbs
    void move();

    // method to draw the boid
    void draw();

    // method to draw breadcrumbs
    void drawBreadCrumbs();
    // update the tired and hunger values
    void updateAttributes();

    // Getters and setters for tiredness and hunger and skill levels
    int getTiredness() const { return tiredness; }
    int getHunger() const { return hunger; }
    void setTiredness(int value) { tiredness = value; }
    void setHunger(int value) { hunger = value; }
    int getSkillLevel() const { return skillLevel; }
    void setSkillLevel(int level) { skillLevel = level; }
    int getTimeSitting() const { return timeSitting; }
    int getTimeDancing() const { return timeDancing; }
};

#endif
