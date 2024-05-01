#include "indoorEnv.h" // Assuming you have an Environment class
#include <unordered_map>



#include <vector>

#include <algorithm> // for std::shuffle
#include <random>    // for std::default_random_engine
#include <chrono>    // for std::chrono::system_clock
using namespace std;
using namespace std::chrono;



class Result {
public:
    SteeringState steeringState;
    sf::Vector2f targetPosition;
    bool success; // Flag indicating whether the task was successfully executed

    Result(SteeringState state, sf::Vector2f position, bool success)
        : steeringState(state), targetPosition(position), success(success) {}
};

enum treeNodeString{
    Pathfind_TV,
    Sequence_Selector,
    Pathfind_Enemy,
    Dance,
    Sit,
    Not_Acting,
    Near_Enemy,
    Root,
    Start_Wander,
    Random,

};
class Task{
    public:
    treeNodeString nodeString;
    Boid character;
    vector<Task*> children;

    Task() {
        children.reserve(3);
    }
    Task(Boid & character, treeNodeString nodeString ): character(character), nodeString(nodeString) {
        children.reserve(3);
    }

    void setBoid(Boid & c){
        character = c;
    }

    virtual ~Task(){
        for (auto & child:children){
            delete child;
        }
    }
    virtual bool run(IndoorEnvironment & environment, Result &r) = 0;

    void addChild(Task*child){
        children.push_back(child);
    }

    string enumToString() {
        switch (nodeString) {

            case treeNodeString::Pathfind_TV:
                return "Pathfind_TV";
            case treeNodeString::Sequence_Selector:
                return "Sequence_Selector";
            case treeNodeString::Pathfind_Enemy:
                return "Pathfind_Enemy";
            case treeNodeString::Dance:
                return "Dance";
            case treeNodeString::Start_Wander:
                return "Wander";
            case treeNodeString::Sit:
                return "Sit";
            case treeNodeString::Not_Acting:
                return "Not_Acting";
            case treeNodeString::Near_Enemy:
                return "Near_Enemy";
            case treeNodeString::Root:
                return "Root";
            case treeNodeString::Random:
                return "Random";
            default:
                return "Unknown";
        }
    }


};

class Action: public Task{
    private:
        bool checkRadialDistance(sf::Vector2f position1, sf::Vector2f position2, int radius) {
            return (abs(position1.x - position2.x) <= radius) && (abs(position1.y - position2.y) <= radius);
        }
        bool completed = false;
    public:
    Action(Boid & character, treeNodeString nodeString) : Task(character,nodeString){}
    bool run(IndoorEnvironment & enviornment, Result & r) override {
        sf::Vector2f playerPos = enviornment.getCharacterPos();
        bool nearEnemy = checkRadialDistance(character.kinematic.position, playerPos, 150);

        switch(nodeString) {                
            case Near_Enemy:
                character.boidSteeringState = SIT;
                r = Result(SIT,sf::Vector2f(0,0),false);
                if(nearEnemy){
                    
                    return true;
                }
                return false;
            case Pathfind_Enemy:
                character.boidSteeringState = PATHFINDING;
                r = Result(character.boidSteeringState,  enviornment.getCharacterPos(), true);
                return true;

          
            case Dance:
                if(character.skillLevel<30){
                        r = Result(DANCING,sf::Vector2f(0,0),true);
                        return true;    
                }
                return false;
                
            case Start_Wander:
                if(character.skillLevel<30){
                     r = Result(SIT,sf::Vector2f(0,0),false);
                    return false;
                }
                r = Result(WANDERING,sf::Vector2f(0,0),true);
                return true;
                
        }
        return false;
    }
};


class Selector : public Task {
private:

public:
    Selector(Boid& character,  treeNodeString nodeString) : Task(character, nodeString) {}

    bool run( IndoorEnvironment &environment, Result & r)  {
        


        for (Task* c : children) {
            if (c->run(environment,r)) {
                 return true;
            }
        }
        return  false;
    }
};


class RandomSelector : public Task {
private:

public:
    RandomSelector( Boid& character,  treeNodeString nodeString) : Task(character, nodeString) {}

    bool run( IndoorEnvironment &environment, Result &r) override {
        
        shuffle(children.begin(), children.end(), default_random_engine(system_clock::now().time_since_epoch().count()));

        for (Task* c : children) {
            if (c->run(environment, r)) {
                return  true;
            }
        }
        return  false;
    }
};


class Sequence : public Task {
private:
public:
    Sequence(Boid& character,  treeNodeString nodeString) : Task(character, nodeString) {}

    bool run( IndoorEnvironment &environment, Result &r) override {
        
        for (Task* c : children) {
            if (!c->run(environment, r)) {
                return false;
            }
        }
        return true;
    }
};



class BehaviorTree {
private:
    Task* root;
    Boid character;
    IndoorEnvironment env;


public:
    BehaviorTree(IndoorEnvironment& env, Boid& character)
        : env(env), character(character), root(nullptr) {}

    void setRoot(Task* root_task) {
        root = root_task;
    }
    void setEnviornment(IndoorEnvironment& e)
    {
        env = e;
    }
    void setBoid(Boid& b, Task* node)
    {
         if(node) {
            node->setBoid(b);
            for (Task* child : node->children) {
                setBoid(b, child);
            }
        }
    }
    Boid getBoid(){
        return character;
    }
    IndoorEnvironment  getEnviornment()
    {
        return env;
    }

    Task* getRoot(){
        return root;
    }
    bool run(IndoorEnvironment& e, Boid& c, Result & r) {
        env = e;
        setBoid(c,root);
        return root->run(env, r);
        
    }
    void printTree(Task* node, int level) {
        if (node) {
            // Print the current node
            std::string indentation(level, '\t');
            std::cout << indentation << node->enumToString() << std::endl;

            // Print children recursively
            for (Task* child : node->children) {
                printTree(child, level + 1);
            }
        }
    }

     
    
};
