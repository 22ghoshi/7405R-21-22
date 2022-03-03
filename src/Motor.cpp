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

void MotorGroup::moveVelocity(int velocity) {
    for (auto const& motor : this->motors) {
        motor.get()->getMotor()->move_velocity(velocity);
    }
}

//returns average of all encoders in group
double MotorGroup::getEncoders() {
    double totalVal = 0;
    for (auto const& motor : this->motors) {
        totalVal += motor.get()->getMotor()->get_position();
    }
    return (totalVal / this->motors.size());
}

void MotorGroup::resetEncoders() {
    for (auto const& motor : this->motors) {
        motor.get()->getMotor()->tare_position();
    }
}

//coasts by default
void MotorGroup::stop(brakeType brake) {
    switch(brake) {
        case brakeType::coast:
            for (auto const& motor : this->motors) {
                motor.get()->getMotor()->move_velocity(0);
                motor.get()->getMotor()->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            }
            break;
        case brakeType::brake:
            for (auto const& motor : this->motors) {
                motor.get()->getMotor()->move_velocity(0);
                motor.get()->getMotor()->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
            }
            break;
        case brakeType::hold:
            for (auto const& motor : this->motors) {
                motor.get()->getMotor()->move_velocity(0);
                motor.get()->getMotor()->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            }
            break;
    }
}

Piston::Piston() {}

Piston::Piston(std::string pistonName, std::uint8_t pistonPort) {
    if (pistonPort < 1 || pistonPort > 8) {
        throw std::invalid_argument("piston triport 1 - 8 only");
    }

    this->name = pistonName;
    this->port = pistonPort;

    piston = std::make_unique<pros::ADIDigitalOut>(port);
}

Piston::Piston(std::string pistonName, std::pair<std::uint8_t, std::uint8_t> pistonPorts) {
    if (pistonPorts.second < 1 || pistonPorts.second > 8) {
        throw std::invalid_argument("piston triport 1 - 8 only");
    }

    this->name = pistonName;
    this->port = pistonPorts.second;

    piston = std::make_unique<pros::ADIDigitalOut>(std::make_pair(pistonPorts.first, port));
}

pros::ADIDigitalOut* Piston::getPiston() {
    return this->piston.get();
}