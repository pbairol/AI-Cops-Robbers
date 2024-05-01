#pragma once
#include <string.h>
using namespace std;
enum SteeringState
{
    WANDERING,
    PATHFINDING,
    DANCING,
    SIT,
    DONE_PATHFINDING,
    FORWARD,
};

static string boidSteeringToString(SteeringState steerString) {
        switch (steerString) {

            case SteeringState::WANDERING:
                return "WANDERING";
            case SteeringState::PATHFINDING:
                return "PATHFINDING";
             case SteeringState::DANCING:
                return "DANCING";
            case SteeringState::SIT:
                return "SIT";
            case SteeringState::DONE_PATHFINDING:
                return "DONE_PATHFINDING";
            default:
                return "Unknown";
        }
 }
