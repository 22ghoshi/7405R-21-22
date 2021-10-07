#include "Motor.hpp"

Motor::Motor() {}

Motor::Motor(std::string motorName, motorGearset motorGearset, std::uint8_t motorPort, bool reversed) {
    if (motorPort < 1 || motorPort > 21) {
        throw std::invalid_argument("motor port 1 - 21 only");
    }
    
    this->name = motorName;
    this->gearset = motorGearset;
    this->port = motorPort;
    this->reverse = reversed;

    switch(gearset) {
        case motorGearset::GS100:
            motor = std::make_unique<pros::Motor>(port, pros::E_MOTOR_GEARSET_36, reverse);
            break;
        case motorGearset::GS200:
            motor = std::make_unique<pros::Motor>(port, pros::E_MOTOR_GEARSET_18, reverse);
            break;
        case motorGearset::GS600:
            motor = std::make_unique<pros::Motor>(port, pros::E_MOTOR_GEARSET_06, reverse);
            break;
    }
}

pros::Motor* Motor::getMotor() {
    return this->motor.get();
}

MotorGroup::MotorGroup() {}

MotorGroup::MotorGroup(std::string motorGroupName, std::vector<std::shared_ptr<Motor>> motorGroupMotors) {
    this->name = motorGroupName;
    this->motors = motorGroupMotors;
}

void MotorGroup::operator=(std::int32_t voltage) {
    for (auto const& motor : this->motors) {
        *(motor.get()->getMotor()) = voltage;
    }
}

//coasts by default
void MotorGroup::stop(brakeType brake) {
    *(this) = 0;
    switch(brake) {
        case brakeType::coast:
            for (auto const& motor : this->motors) {
                motor.get()->getMotor()->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            }
            break;
        case brakeType::brake:
            for (auto& motor : this->motors) {
                motor.get()->getMotor()->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
            }
            break;
        case brakeType::hold:
            for (auto& motor : this->motors) {
                motor.get()->getMotor()->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            }
            break;
    }
}