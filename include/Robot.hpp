#pragma once
#include "main.h"

#include "Sensor.hpp"
#include "Motor.hpp"

#include <map>
#include <memory>
#include <string>
#include <queue>
#include <vector>

#define sRobot Robot::Instance()

class Robot {
    private:
    Robot();
    Robot(Robot const&){};
    Robot& operator=(Robot const&) {
        return *this;
    }

    static Robot* pInstance;
    
    std::map<std::string, std::shared_ptr<Motor>> motors;
    std::map<std::string, std::unique_ptr<MotorGroup>> motorGroups;
    std::map<std::string, std::unique_ptr<Piston>> pistons;
    std::map<std::string, std::unique_ptr<Sensor>> sensors;

    public:
    static Robot* Instance();

    void mecanum(int power, int strafe, int turn);
    void arcade(int power, int turn);
    void tank(int left, int right);
    void stopDrive();
    void makeMotorGroup(std::string name, std::vector<std::string> motorNames);
    pros::Motor* getMotor(std::string name);
    MotorGroup* getMotorGroup(std::string name);
    pros::ADIDigitalOut* getPiston(std::string name);
    pros::ADIButton* getButton(std::string name);
    pros::ADILineSensor* getLine(std::string name);
    pros::ADIPotentiometer* getPotentiometer(std::string name);
    pros::ADIUltrasonic* getUltrasonic(std::string name);
    pros::ADIEncoder* getEncoder(std::string name);
    pros::Rotation* getRotation(std::string name);
    pros::Imu* getInertial(std::string name);
    pros::Vision* getVision(std::string name);
};