#pragma once
#include "main.h"

#include <map>
#include <memory>
#include <string>
#include <stdexcept>

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
    static std::map<std::string, std::unique_ptr<Motor>> motors;
    Motor();
    Motor(std::string motorName, motorGearset motorGearset, std::uint8_t motorPort, bool reversed = false);
    static void makeGroup(std::string motorGroupName, std::vector<std::string> motorGroupMotors);
    static pros::Motor* getMotor(std::string name);
};

class MotorGroup {
    private:
    std::string name;
    std::vector<std::string> motors;

    public:
    static std::map<std::string, std::unique_ptr<MotorGroup>> motorGroups;
    MotorGroup();
    MotorGroup(std::string motorGroupName, std::vector<std::string> motorGroupMotors);
    void operator=(std::int32_t voltage);
    void stop(brakeType brake);
    static MotorGroup* getMotorGroup(std::string name);
};