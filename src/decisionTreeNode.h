#include "indoorEnv.h"
#include "structs.h"

using namespace std;

class MonsterDecisionNode
{

public:
    string condition;
    MonsterDecisionNode *lowBranch;
    MonsterDecisionNode *middleBranch;
    MonsterDecisionNode *highBranch;
    MonsterDecisionNode(string condition) : condition(condition), lowBranch(nullptr), middleBranch(nullptr), highBranch(nullptr) {}
    /**
     * Recursively iterates through through the tree
     * @param environment The indoor environment
     * @param boid The boid character
     */
    MonsterDecisionNode *makeDecision(IndoorEnvironment &environment, Boid &boid)
    {
        float distCharacter = distanceFunc(boid.kinematic.position, environment.getCharacterPos());

        float distFood = distanceFunc(boid.kinematic.position, environment.getBurgerPosition());
        if (lowBranch == nullptr || middleBranch == nullptr || highBranch == nullptr)
        {
            return this;
        }
        if (condition == "Enemy_Dist")
        {
            if (distCharacter < 135)
            {
                return lowBranch->makeDecision(environment, boid);
            }
            else if (distCharacter < 175)
            {
                return middleBranch->makeDecision(environment, boid);
            }
            else
            {
                return highBranch->makeDecision(environment, boid);
            }
        }

        if (condition == "Food_Dist")
        {
            if (distFood < 120)
            {
                return lowBranch->makeDecision(environment, boid);
            }
            else if (distFood < 160)
            {
                return middleBranch->makeDecision(environment, boid);
            }
            else
            {
                return highBranch->makeDecision(environment, boid);
            }
        }
        if (condition == "Skill_Level")
        {
            if (boid.skillLevel < 15)
            {
                return lowBranch->makeDecision(environment, boid);
            }
            else if (boid.skillLevel > 15)
            {
                return middleBranch->makeDecision(environment, boid);
            }
            else
            {
                return highBranch->makeDecision(environment, boid);
            }
        }
        return nullptr;
    }

    void setLowBranch(MonsterDecisionNode *node)
    {
        lowBranch = node;
    }

    void setMiddleBranch(MonsterDecisionNode *node)
    {
        middleBranch = node;
    }

    void setHighBranch(MonsterDecisionNode *node)
    {
        highBranch = node;
    }

    string toString()
    {
        return this->condition;
    }
};

class DecisionTreeNode
{
public:
    string condition;
    bool isDecision = false;
    bool isAction = false;
    DecisionTreeNode *trueBranch;
    DecisionTreeNode *falseBranch;

    DecisionTreeNode(string condition) : condition(condition), trueBranch(nullptr), falseBranch(nullptr) {}

    /**
     * Recursively iterates through through the tree
     * @param environment The indoor environment
     * @param boid The boid character
     */
    DecisionTreeNode *makeDecision(IndoorEnvironment &environment, Boid &boid)
    {
        if (trueBranch == nullptr && falseBranch == nullptr)
        {

            return this;
        }

        bool isHungry = boid.hunger >= 100;
        bool needSleep = boid.tiredness >= 100;
        bool cantDance = boid.skillLevel < 10;

        if (condition == "Tired?")
        {
            if (needSleep)
            {
                return trueBranch->makeDecision(environment, boid);
            }
            else
            {
                return falseBranch->makeDecision(environment, boid);
            }
        }
        else if (condition == "Hungry?")
        {
            if (isHungry)
            {
                return trueBranch->makeDecision(environment, boid);
            }
            else
            {
                return falseBranch->makeDecision(environment, boid);
            }
        }
        else if (condition == "Can't Dance?")
        {
            if (cantDance)
            {
                // Go practice
                return trueBranch->makeDecision(environment, boid);
            }
            else
            {
                // Go dance to increase skill level
                return falseBranch->makeDecision(environment, boid);
            }
        }

        return nullptr;
    }

    std::string toString()
    {
        return this->condition;
    }

    void setTrueNode(DecisionTreeNode *node)
    {
        trueBranch = node;
    }

    void setFalseNode(DecisionTreeNode *node)
    {
        falseBranch = node;
    }
};
