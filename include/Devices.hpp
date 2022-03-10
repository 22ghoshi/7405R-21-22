#pragma once
#include "main.h"

#include <map>
#include <memory>
#include <string>
#include <stdexcept>

enum class motorGearset { GS100, GS200, GS600 };

class Devices {
    private:
    Devices();
    
    public:
    static std::map<std::string, std::unique_ptr<pros::Motor>> motors;
    static std::map<std::string, std::unique_ptr<pros::ADIDigitalOut>> pistons;

    static std::map<std::string, std::unique_ptr<pros::ADIButton>> buttons;
    static std::map<std::string, std::unique_ptr<pros::ADILineSensor>> lines;
    static std::map<std::string, std::unique_ptr<pros::ADIPotentiometer>> potentiometers;
    static std::map<std::string, std::unique_ptr<pros::ADIUltrasonic>> ultrasonics;
    static std::map<std::string, std::unique_ptr<pros::ADIEncoder>> encoders;
    static std::map<std::string, std::unique_ptr<pros::Rotation>> rotations;
    static std::map<std::string, std::unique_ptr<pros::Imu>> inertials;
    static std::map<std::string, std::unique_ptr<pros::Vision>> visions;
    static std::map<std::string, std::unique_ptr<pros::Distance>> distances;

    //defaults to 200rpm/forward
    static pros::Motor* getMotor(std::string name, int port, motorGearset gearset = motorGearset::GS200, bool reversed = false);
    static pros::ADIDigitalOut* getPiston(std::string name, int port);
    
    static pros::ADIButton* getButton(std::string name, int port);
    static pros::ADILineSensor* getLine(std::string name, int port);
    static pros::ADIPotentiometer* getPotentiometer(std::string name, int port);
    static pros::ADIUltrasonic* getUltrasonic(std::string name, int port);
    static pros::ADIEncoder* getEncoder(std::string name, int port, bool reversed = false);
    
    static pros::Rotation* getRotation(std::string name, int port);
    static pros::IMU* getInertial(std::string name, int port);
    static pros::Vision* getVision(std::string name, int port);
    static pros::Distance* getDistance(std::string name, int port);
};