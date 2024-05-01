#include "steeringBehavior.h"
#include "helperMethods.h"
#include "graph.h"
#include <iostream>
#include <cmath>

using namespace std;

// method to calculate steering behavior for rotation matching
Steering RotationMatching::calculate(const Kinematic &character, const Kinematic &target)
{
    Steering steering = Steering(sf::Vector2f(0.0f, 0.0f), 0.0);
    float deltaRotation = target.rotation - character.rotation;
    steering.update(sf::Vector2f(0.0f, 0.0f), deltaRotation / timeToTarget, timeToTarget);
    return steering;
}
// method to calculate steering behavior for velocity matching
Steering VelocityMatching::calculate(const Kinematic &character, const Kinematic &target)
{
    Steering steering = Steering(sf::Vector2f(0.0f, 0.0f), 0.0);

    sf::Vector2f deltaVelocity = target.velocity - character.velocity;

    steering.update(deltaVelocity / timeToTarget, 0.0, timeToTarget);
    return steering;
}

// Arrive Algorithm To Move the character kinematic boid object to the mouse location nicely
Steering PositionMatching::calculate(const Kinematic &character, const Kinematic &target)
{
    sf::Vector2f distance = target.position - character.position;
    sf::Vector2f direction = normalize(distance);

    float targetSpeed = 0.0;
    float magDistance = magnitude(distance);

    if (magDistance <= radiusOfSatisfaction)
    {
        targetSpeed = 0.0;
    }

    else if (magDistance <= raidusOfDeceleration)
    {
        targetSpeed = magnitude(distance) / raidusOfDeceleration * maxVelocity;
    }
    else
    {
        targetSpeed = maxVelocity;
    }

    sf::Vector2f goalVelocity = direction * targetSpeed;
    sf::Vector2f steeringVelocity = goalVelocity - character.velocity;
    Steering steering = Steering(sf::Vector2f(0.0f, 0.0f), 0.0);
    steering.update(steeringVelocity / timeToTarget, 0.0, timeToTarget);

    return steering;
}

// helper sub steering method for collsion avoidance it is flee the oppsoite of seek From textbook
Steering Flee::calculate(const Kinematic &character, const Kinematic &target)
{
    sf::Vector2f displacement = character.position - target.position;

    Steering steering = Steering(sf::Vector2f(0.0f, 0.0f), 0.0);
    steering.linearAcceleration = normalize(displacement) * maxLinearAcceleration;

    steering.angularAcceleration = 0.0f;

    return steering;
}

Steering Seek::calculate(const Kinematic &character, const Kinematic &target)
{
    sf::Vector2f displacement = target.position - character.position;

    Steering steering = Steering(sf::Vector2f(0.0f, 0.0f), 0.0);
    steering.linearAcceleration = normalize(displacement) * maxLinearAcceleration;

    steering.angularAcceleration = 0.0f;

    return steering;
}

// Align Algorithm To orient the character kinematic boid object to the mouse location nicely

Steering OrientationMatching::calculate(const Kinematic &character, const Kinematic &target)
{
    float rotation = target.orientation - character.orientation;
    rotation = mapToRange(rotation);

    float rotationSize = abs(rotation);

    float goalRotation = 0.0;
    if (rotationSize <= radiusOfSatisfaction)
    {
        goalRotation = 0.0;
    }
    else if (rotationSize <= raidusOfDeceleration)
    {
        goalRotation = maxRotation * (rotationSize / raidusOfDeceleration);
    }
    else
    {
        goalRotation = maxRotation;
    }

    goalRotation *= rotation / rotationSize;

    float steeringRotDelta = goalRotation - character.rotation;

    Steering steering = Steering(sf::Vector2f(0.0f, 0.0f), 0.0);
    steering.update(sf::Vector2f(0.0f, 0.0f), steeringRotDelta / timeToTarget, timeToTarget);
    return steering;
}

// Wandering 1 algorihtm from the textbook + class this is part 3a and uses helper methods to do wander steering
Steering Wander::calculate(const Kinematic &character, const Kinematic &target)
{
    Steering steering = Steering(sf::Vector2f(0.0f, 0.0f), 0.0);
    wanderOrientation += randomBinomial() * wanderRate;

    float targetOrientation = wanderOrientation + character.orientation;

    sf::Vector2f targetCenter = character.position + (wanderOffSet * orientationToDirection(character.orientation));

    targetCenter += wanderRadius * orientationToDirection(targetOrientation);

    Kinematic wanderTarget = Kinematic(targetCenter, targetOrientation, character.velocity, character.rotation);
    PositionMatching posMatch = PositionMatching();
    OrientationMatching orMatch = OrientationMatching();
    steering.linearAcceleration = posMatch.calculate(character, wanderTarget).linearAcceleration;
    steering.angularAcceleration = orMatch.calculate(character, wanderTarget).angularAcceleration;
    return steering;
}

// Wrote the book psudocode This is speration steering needed for flocking
Steering Separation::calculate(const Kinematic &character, const Kinematic &target)
{
    Steering steering = Steering(sf::Vector2f(0.0f, 0.0f), 0.0);

    sf::Vector2f displacement = character.position - target.position;
    displacement = sf::Vector2f(displacement.x + rand(), displacement.y + rand());
    float distance = magnitude(displacement);
    float strength = 0.0f;

    if (distance == 0)
    {
        return steering;
    }

    if (distance < 40.0f)
    {
        strength = min((decayCoefficient / (distance * distance)), maxLinearAcceleration);
        normalize(displacement);
        steering.linearAcceleration = -strength * displacement;
    }

    return steering;
}

// Wandering 2 algorihtm from the textbook , this is part 3b and uses helper methods to do another  wander steering

Steering Wander2::calculate(const Kinematic &character, const Kinematic &target)
{
    Steering steering = Steering(sf::Vector2f(0.0f, 0.0f), 0.0);
    steering.linearAcceleration = maxVelocity * orientationToDirection(character.orientation);
    steering.angularAcceleration = maxRotation * randomBinomial();
    return steering;
}
// Wrote the book psudocode This is collsion avoidance steering needed for flockin, utiilizes flee as well

Steering Avoidance::calculate(const Kinematic &character, const Kinematic &target)
{
    Steering steering = Steering(sf::Vector2f(0.0f, 0.0f), 0.0);
    sf::Vector2f dp = character.position - target.position;
    sf::Vector2f dv = character.velocity - target.velocity;

    float t = (-((dp.x) * dv.x) + ((dp.y) * dv.y)) / pow(magnitude(sf::Vector2f(dv.x, dv.y)), 2);

    sf::Vector2f characterClosest = character.position + character.velocity * t;
    sf::Vector2f targetClosest = target.position + target.velocity * t;

    float distance = magnitude(characterClosest - targetClosest);

    if (distance < 90.0f)
    {
        Kinematic charCA = Kinematic(characterClosest, 0.0, sf::Vector2f(0, 0), 0.0);
        Kinematic targCA = Kinematic(targetClosest, 0.0, sf::Vector2f(0, 0), 0.0);

        return Flee().calculate(charCA, targCA);
    }

    return steering;
}



Steering Face::calculate(const Kinematic &character, const Kinematic &target)
{
    sf::Vector2f direction = target.position - character.position;

    float distance = magnitude(direction);
    Kinematic newTarget = target;
    newTarget.orientation = atan2(direction.y, direction.x);

    return OrientationMatching().calculate(character, newTarget);
}
int PathFollowing::getIndex(int currIndex, sf::Vector2f target)
{
    int closestIndex = 0;
    float closestDistance = 99999;
    for (int i = currIndex; i < path.size(); i++)
    {
        float distance = distanceFunc(target, sf::Vector2f(path[i].x, path[i].y));
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestIndex = i;
        }
    }
    return closestIndex;
}
Steering PathFollowing::calculate(const Kinematic &character, const Kinematic &target)
{

    sf::Vector2f futurePos = character.position + (character.velocity * predictTime);
    int closestIndex = this->getIndex(currentIndex, futurePos);
    // Update current index after finding the closest vertex
    int cI = closestIndex + 1;
    Steering steer = PositionMatching().calculate(character, Kinematic(sf::Vector2f(path[cI].x, path[cI].y), 0, sf::Vector2f(0, 0), 0));
    steer.angularAcceleration = Face().calculate(character, Kinematic(sf::Vector2f(path[cI].x, path[cI].y), 0, sf::Vector2f(0, 0), 0)).angularAcceleration;
    return steer;
}

