#ifndef STEERING_BEHAVIOR
#define STEERING_BEHAVIOR

#include <SFML/Graphics.hpp>
#include "structs.h"
#include "graph.h"
#include "helperMethods.h"

// Constants for steering behaviors
const float radiusOfSatisfaction = .2f; // Radius of satisfaction for position matching
const float raidusOfDeceleration = 100.0f; // Radius of deceleration for position matching
const float maxVelocity = 35.0f; // Maximum velocity for position matching
const float maxRotation = 5.5f; // Maximum rotation for position matching
const float timeToTarget = .4f; // Time to target for steering behaviors
const float maxLinearAcceleration = 35.0f; // Maximum linear acceleration for steering behaviors
const float maxAngularAcceleration = 15.0; // Maximum angular acceleration for steering behaviors

const float threshold = 65.0f; // Threshold for separation behavior
const float decayCoefficient = 10000.0f; // Decay coefficient for separation behavior

// Base class for all steering behaviors
class SteeringBehavior
{
public:
    SteeringBehavior() {} 
    virtual Steering calculate(const Kinematic &character, const Kinematic &target) = 0; // Abstract method to calculate steering
    virtual ~SteeringBehavior() {} // Virtual destructor to avoid vtable errors
};

// Class for position matching steering behavior
class PositionMatching : public SteeringBehavior
{
public:
    PositionMatching() {}
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate position matching steering
};

// Class for flee steering behavior
class Flee : public SteeringBehavior
{
public:
    Flee() {} 
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate flee steering
};

// Class for orientation matching steering behavior
class OrientationMatching : public SteeringBehavior
{
public:
    OrientationMatching() {} 
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate orientation matching steering
};

class Seek : public SteeringBehavior
{
public:
    Seek() {} 
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate orientation matching steering
};
class Face : public SteeringBehavior
{
public:
    Face() {} 
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate orientation matching steering
};


// Class for rotation matching steering behavior
class RotationMatching : public SteeringBehavior
{
public:
    RotationMatching() {} 
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate rotation matching steering
};

// Class for velocity matching steering behavior
class VelocityMatching : public SteeringBehavior
{
public:
    VelocityMatching() {} 
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate velocity matching steering
};

// Class for wander steering behavior
class Wander : public SteeringBehavior
{
    float wanderOrientation;
    float wanderOffSet;
    float wanderRate;
    float wanderRadius;

public:
    
    Wander(float wOS, float wRt, float wRd)
    {
        wanderOrientation = 0.0f;
        wanderOffSet = wOS;
        wanderRate = wRt;
        wanderRadius = wRd;
    }
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate wander steering
};

// Class for wander2 steering behavior
class Wander2 : public SteeringBehavior
{
public:
    Wander2() {} 
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate wander2 steering
};

// Class for separation steering behavior
class Separation : public SteeringBehavior
{
public:
    Separation() {} 
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate separation steering
};

// Class for avoidance steering behavior
class Avoidance : public SteeringBehavior
{
public:
    Avoidance() {} 
    Steering calculate(const Kinematic &character, const Kinematic &target) override; // Method to calculate avoidance steering
};

class PathFollowing : public SteeringBehavior
{
public:
    PathFollowing(){
        path;
        pathOffset = 0.0f;
        currentIndex = 0;
        predictTime = 0.1f;
    }

    PathFollowing(vector<Vertex> p, float pOS,Graph g)
    {   
        graph = g;
        path = p;
        pathOffset = pOS;
        currentIndex = 0;
        predictTime = 0.1f;
    }
    Steering calculate(const Kinematic &character, const Kinematic &target) override;
    int getIndex(int currIndex, sf::Vector2f futurePos);

private:
    Graph graph;
    vector<Vertex> path;
    float pathOffset;
    int currentIndex;
    float predictTime = 0.1f; // Adjust predictTime as needed
};



#endif 
