#include "Motor.hpp"

MotorGroup::MotorGroup() {}

MotorGroup::MotorGroup(std::vector<std::function<pros::Motor()>> motorGroupMotors) {
    this->group = motorGroupMotors;
}

void MotorGroup::operator=(std::int32_t voltage) {
    for (auto const& motor : this->group) {
        motor() = voltage;
    }
}

void MotorGroup::moveVelocity(std::int32_t velocity) {
    for (auto const& motor : this->group) {
        motor().move_velocity(velocity);
    }
}

double MotorGroup::getEncoders() {
    double totalVal = 0;
    for (auto const& motor : this->group) {
        totalVal += motor().get_position();
    }
    return (totalVal / this->group.size());
}

void MotorGroup::resetEncoders() {
    for (auto const& motor : this->group) {
        motor().tare_position();
    } 
}

void MotorGroup::stop(brakeType brake) {
    switch(brake) {
        case brakeType::coast:
            for (auto const& motor : this->group) {
                motor().move_velocity(0);
                motor().set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            }
            break;
        case brakeType::brake:
            for (auto const& motor : this->group) {
                motor().move_velocity(0);
                motor().set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
            }
            break;
        case brakeType::hold:
            for (auto const& motor : this->group) {
                motor().move_velocity(0);
                motor().set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            }
            break;
    }
}