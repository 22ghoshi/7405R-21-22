#pragma once
#include "main.h"

#include "Sensor.hpp"

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

    std::map<std::string, std::unique_ptr<pros::Motor>> motors;

    static Robot* pInstance;

    public:
    static Robot* Instance();

    void mecanum(int power, int strafe, int turn);
    void arcade(int power, int turn);
    

    pros::Motor* getMotor(std::string name);
    pros::ADIButton* getButton(std::string name);
    pros::ADILineSensor* getLine(std::string name);
    pros::ADIPotentiometer* getPotentiometer(std::string name);
    pros::ADIUltrasonic* getUltrasonic(std::string name);
    pros::ADIEncoder* getEncoder(std::string name);
    pros::Imu* getInertial(std::string name);
    pros::Vision* getVision(std::string name);
};