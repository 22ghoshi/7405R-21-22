#pragma once
#include "main.h"

#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

enum class brakeType { coast, brake, hold };

class MotorGroup {
    private:
    std::vector<std::function<pros::Motor()>> group;

    public:
    MotorGroup();
    MotorGroup(std::vector<std::function<pros::Motor()>> motorGroupMotors);
    void operator=(std::int32_t voltage);
    void moveVelocity(std::int32_t velocity);
    double getEncoders();
    void resetEncoders();
    void stop(brakeType brake = brakeType::coast);
};