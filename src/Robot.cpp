#include "Robot.hpp"

Robot* Robot::pInstance = NULL;

Robot::Robot() {
	//dead ports
	Motor::motors["BackLeft"] = std::make_unique<Motor>("BackLeft", motorGearset::GS200, 1);
	Motor::motors["BackRight"] = std::make_unique<Motor>("BackRight", motorGearset::GS200, 2);
	Motor::motors["FrontLeft"] = std::make_unique<Motor>("FrontLeft", motorGearset::GS200, 3);
	Motor::motors["FrontRight"] = std::make_unique<Motor>("FrontRight", motorGearset::GS200, 4);
	Motor::makeGroup("LeftDrive", {"BackLeft", "FrontLeft"});
	Motor::makeGroup("RightDrive", {"BackRight", "FrontRight"});
	Motor::makeGroup("Drive", {"BackLeft", "BackRight", "FrontLeft", "FrontRight"});

	Sensor::sensors["Left Encoder"] = std::make_unique<Sensor>("Left", sensorClass::encoder, 1);
	Sensor::sensors["Middle Encoder"] = std::make_unique<Sensor>("Middle", sensorClass::encoder, 3);
	Sensor::sensors["Right Encoder"] = std::make_unique<Sensor>("Right", sensorClass::encoder, 5);
	Sensor::sensors["Inertial"] = std::make_unique<Sensor>("Inertial", sensorClass::inertial, 5);
}

Robot* Robot::Instance() {
    if (!pInstance) {
        pInstance = new Robot();
    }
    return pInstance;
}

void Robot::mecanum(int power, int strafe, int turn) {
	*(Motor::getMotor("BackLeft")) = power - strafe + turn;
	*(Motor::getMotor("BackRight")) = power + strafe - turn;
	*(Motor::getMotor("FrontLeft")) = power + strafe + turn;
	*(Motor::getMotor("FrontRight")) = power - strafe - turn;
	
}

void Robot::arcade(int power, int turn) {
	*(MotorGroup::getMotorGroup("LeftDrive")) = power + turn;
	*(MotorGroup::getMotorGroup("RightDrive")) = power - turn;
}


void Robot::stopDrive() {
    MotorGroup::getMotorGroup("Drive")->stop(brakeType::brake);
}