#pragma once
#include "main.h"

#include "Sensor.hpp"
#include "Motor.hpp"

#include <map>
#include <memory>
#include <string>
#include <queue>

#define sRobot Robot::Instance()

class Robot {
    private:
    Robot();
    Robot(Robot const&){};
    Robot& operator=(Robot const&) {
        return *this;
    }

    static Robot* pInstance;

    public:
    static Robot* Instance();

    void mecanum(int power, int strafe, int turn);
    void arcade(int power, int turn);
    void stopDrive();
};