#pragma once
#include "main.h"

#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

enum class motorGearset { GS100, GS200, GS600 };
enum class brakeType { coast, brake, hold };

class Motor {
    private:
    std::unique_ptr<pros::Motor> motor;
    std::string name;
    motorGearset gearset;
    std::uint8_t port;
    bool reverse;

    public:
    Motor();
    Motor(std::string motorName, motorGearset motorGearset, std::uint8_t motorPort, bool reversed = false);
    pros::Motor* getMotor();
};

class MotorGroup {
    private:
    std::string name;
    std::vector<std::shared_ptr<Motor>> motors;

    public:
    MotorGroup();
    MotorGroup(std::string motorGroupName, std::vector<std::shared_ptr<Motor>> motorGroupMotors);
    void operator=(std::int32_t voltage);
    double getEncoders();
    void resetEncoders();
    void stop(brakeType brake = brakeType::coast);
};

class Piston {
    private:
    std::unique_ptr<pros::ADIDigitalOut> piston;
    std::string name;
    std::uint8_t port;

    public:
    Piston();
    Piston(std::string pistonName, std::uint8_t pistonPort);
    pros::ADIDigitalOut* getPiston();
};