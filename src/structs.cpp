#include <SFML/Graphics.hpp>
#include "structs.h"          // Include header file defining custom structs
#include "helperMethods.h"    // Include header file defining helper methods
#include "steeringBehavior.h" // Include header file defining steering behaviors
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

// Definition of Kinematic struct
Kinematic ::Kinematic(sf::Vector2f pos, float orient, sf::Vector2f vel, float rot)
{
    position = pos;
    orientation = orient;
    velocity = vel;
    rotation = rot;
}

// Method to update Kinematic properties
void Kinematic::update(sf::Vector2f linAcc, float angAcc, float time)
{
    position += velocity * time;
    orientation += rotation * time;

    velocity = linAcc * time;
    rotation = angAcc * time;
}

// Method to check boundaries and adjust position if necessary
void Kinematic::checkBoundary(float width, float height)
{
    if (position.x > width)
    {
        position = sf::Vector2f(0, position.y);
    }
    if (position.x < 0)
    {
        position = sf::Vector2f(width, position.y);
    }
    if (position.y > height)
    {
        position = sf::Vector2f(position.x, 0);
    }
    if (position.y < 0)
    {
        position = sf::Vector2f(position.x, height);
    }
}

// Definition of Steering struct
Steering ::Steering(sf::Vector2f linAcc, float angAcc)
{
    linearAcceleration = linAcc;
    angularAcceleration = angAcc;
}

// Method to update Steering properties
void Steering::update(sf::Vector2f linAcc, float angAcc, float time)
{
    if (magnitude(linAcc) > maxLinearAcceleration)
    {
        linAcc = (normalize(linAcc)) * maxLinearAcceleration;
    }

    if (abs(angAcc) > maxLinearAcceleration)
    {
        angAcc = angAcc / abs(angAcc) * maxAngularAcceleration;
    }
    linearAcceleration = linAcc;
    angularAcceleration = angAcc;
}

// Definition of crumb struct
// Class implementation adapted from What Derek created, Thank you Derek for giving us the sample code
crumb::crumb(int id, float r)
{
    // set initial position and size breadcrumbs
    this->id = id;
    this->setRadius(r);
    this->setFillColor(sf::Color(0, 0, 255, 255));
    this->setPosition(-100, -100);
}

// Method to draw crumb
void crumb::draw(sf::RenderWindow *window)
{
    window->draw(*this);
}

// Method to drop crumb at a given position
void crumb::drop(sf::Vector2f vec)
{
    this->setPosition(vec);
}



// Definition of Boid struct
// Utilized Dereks code for breadcrumbs to update my Own Boid Class implementation this one sets breadcrumbs to empty vector
Boid::Boid(sf::RenderWindow *win, sf::Texture &tex) : kinematic(sf::Vector2f(0, 0), 0, sf::Vector2f(0, 0), 0)
{
    skillLevel = 0;
    tiredness = 0;
    hunger = 0;
    timeDancing = 0;
    timeSitting = 0;
    lastUpdateTime = steady_clock::now();
    vector<crumb> *empty{};
    window = win;
    drop_timer = 100.f;
    breadcrumbs = empty;
    sprite.setTexture(tex);
    crumb_idx = 0;
}

// Utilized Dereks code for breadcrumbs to update my Own Boid Class implementation this time actually having the crumbs created
Boid::Boid(sf::RenderWindow *win, sf::Texture &tex, vector<crumb> *crumbs) : kinematic(sf::Vector2f(0, 0), 0, sf::Vector2f(0, 0), 0)
{
    
    skillLevel = 0;
    tiredness = 0;
    hunger = 0;
    timeDancing = 0;
    timeSitting = 0;
    lastUpdateTime = steady_clock::now();
    window = win;
    drop_timer = 100.f;
    breadcrumbs = crumbs;
    tex.setSmooth(true);
    sprite.setTexture(tex);
    crumb_idx = 0;
}

// Method to draw the Boid
void Boid::draw()
{
    sprite.setScale(sf::Vector2f(2, 2));
    window->draw(sprite);
}

// Method to update the Boid's position and drop breadcrumbs
void Boid::move()
{
    // basic timer for leaving breadcrumbs
    if (drop_timer > 0)
    {
        drop_timer -= 3.0f;
    }
    else
    {
        drop_timer = 250.0f;
        breadcrumbs->at(crumb_idx).drop(kinematic.position);

        if (crumb_idx < numberOfCrumbs - 1)
            crumb_idx++;
        else
            crumb_idx = 0;
    }

    sprite.setPosition(kinematic.position);
    sprite.setRotation(kinematic.orientation * (180 / M_PI));
}

// Method to set Kinematic properties of the Boid
void Boid::setKinematic(const Kinematic &kin)
{
    kinematic = kin;
}

// Method to get Kinematic properties of the Boid
Kinematic Boid::getKinematic()
{
    return kinematic;
}

// Method to get the Boid's sprite
sf::Sprite Boid::getSprite()
{
    return sprite;
}

void Boid::updateAttributes()
{
    // Get the current time
    steady_clock::time_point now = steady_clock::now();

    // Calculate the duration since the last update
    duration<double> timeSinceLastUpdate = duration_cast<duration<double>>(now - lastUpdateTime);

    // Update the tiredness and hunger levels only if 2 seconds have passed since the last update
    if (timeSinceLastUpdate.count() >= 1.0)
    {
        // Update tiredness and hunger levels here
        tiredness += 7;
        hunger += 15;

        // Reset the last update time to the current time
        lastUpdateTime = now;
    }
}