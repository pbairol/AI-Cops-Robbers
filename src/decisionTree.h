#include "decisionTreeNode.h"
#include "structs.h"
#include "indoorEnv.h"
#include <iostream>
#include <utility>

using namespace std;

struct DecisionResult
{
    SteeringState steeringType;
    Kinematic target;

    // Default constructor
    DecisionResult()
        : steeringType(SteeringState::SIT),
          target(Kinematic(sf::Vector2f(0, 0), 0.0, sf::Vector2f(0, 0), 0.0)) {}
};

class MonsterDecisionTree
{
public:
    MonsterDecisionNode *root;
    IndoorEnvironment env;
    Boid character;

    MonsterDecisionTree(Boid &character, IndoorEnvironment &env)
        : root(nullptr), character(character), env(env) {}

    DecisionResult makeDecision(Boid &chr, IndoorEnvironment &env)
    {
        MonsterDecisionNode *actionNode = root->makeDecision(env, chr);
        string action = actionNode->condition;
        DecisionResult result;
        if (action == "Go to Kitchen")
        {
            result.steeringType = SteeringState::PATHFINDING;
            sf::Vector2f pantry = env.getBurgerPosition();
            result.target = Kinematic(sf::Vector2f(pantry.x, pantry.y), 0.0, sf::Vector2f(0, 0), 0.0);
            return result;
        }
        else if (action == "Go to Enemy")
        {
            result.steeringType = SteeringState::PATHFINDING;
            result.target = Kinematic(env.getCharacterPos(), 0.0, sf::Vector2f(0, 0), 0.0);

            return result;
        }
        else if (action == "Go Dance")
        {
            result.steeringType = SteeringState::DANCING;
            result.target = Kinematic(sf::Vector2f(0, 0), 0.0, sf::Vector2f(0, 0), 0.0);
            return result;
        }

        else if (action == "Wander")
        {
            result.steeringType = SteeringState::WANDERING;
            result.target = Kinematic(sf::Vector2f(0, 0), 0.0, sf::Vector2f(0, 0), 0.0);
            return result;
        }
        else
        {
            result.steeringType = SteeringState::SIT;
            result.target = Kinematic(sf::Vector2f(0, 0), 0.0, sf::Vector2f(0, 0), 0.0);
            return result;

        }
    }
   void printTree(MonsterDecisionNode *node, int depth = 0) {
    if (node == nullptr || depth >= 2)
        return;

    cout << string(depth * 2, ' ') << "- " << node->toString() << endl;

    if (node->lowBranch != nullptr) {
        cout << string(depth * 2 + 2, ' ') << "Low Branch: " << node->lowBranch->toString() << endl;
        printTree(node->lowBranch, depth + 1);
    }

    if (node->middleBranch != nullptr) {
        cout << string(depth * 2 + 2, ' ') << "Middle Branch: " << node->middleBranch->toString() << endl;
        printTree(node->middleBranch, depth + 1);
    }

    if (node->highBranch != nullptr) {
        cout << string(depth * 2 + 2, ' ') << "High Branch: " << node->highBranch->toString() << endl;
        printTree(node->highBranch, depth + 1);
    }
    }
};

class DecisionTree
{
public:
    DecisionTreeNode *root;
    IndoorEnvironment env;
    Boid character;

    DecisionTree(Boid character, IndoorEnvironment env)
        : root(nullptr), character(character), env(env) {}

    // Modify makeDecision to return DecisionResult
    DecisionResult makeDecision(Boid &chr)
    {
        if (root == nullptr)
        {
            cout << "No decision tree available." << endl;
            return DecisionResult();
        }

        DecisionTreeNode *actionNode = root->makeDecision(env, chr);
        if (actionNode == nullptr)
        {
            cout << "No decision could be made." << endl;
            return DecisionResult();
        }
        string action = actionNode->condition;

        DecisionResult result;
        if (action == "Get food")
        {
            result.steeringType = SteeringState::PATHFINDING;
            sf::Vector2f pantry = env.getBurgerPosition();
            result.target = Kinematic(sf::Vector2f(pantry.x, pantry.y), 0.0, sf::Vector2f(0, 0), 0.0);
            return result;
        }
        else if (action == "Go to bed")
        {
            result.steeringType = SteeringState::PATHFINDING;
            sf::Vector2f bed = env.getBedPosition();
            result.target = Kinematic(sf::Vector2f(bed.x, bed.y), 0.0, sf::Vector2f(0, 0), 0.0);

            return result;
        }
        else if (action == "Go practice")
        {
            if (distanceFunc(chr.kinematic.position, sf::Vector2f(100, 100)) < 50)
            {
                result.steeringType = SteeringState::DANCING;
            }
            else
            {
                result.steeringType = SteeringState::PATHFINDING;
            }
            sf::Vector2f ds = env.getDSPosition();
            result.target = Kinematic(sf::Vector2f(100, 100), 0.0, sf::Vector2f(0, 0), 0.0);
            return result;
        }
        else if (action == "Wander")
        {
            result.steeringType = SteeringState::WANDERING;
            result.target = Kinematic(sf::Vector2f(0, 0), 0.0, sf::Vector2f(0, 0), 0.0);
        }
        else
        {
            result.steeringType = SteeringState::SIT;
            result.target = Kinematic(sf::Vector2f(0, 0), 0.0, sf::Vector2f(0, 0), 0.0);
        }

        return result;
    }
};
