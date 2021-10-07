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
    void stop(brakeType brake = brakeType::coast);
};