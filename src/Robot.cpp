#include "Robot.hpp"

Robot* Robot::pInstance = NULL;

Robot::Robot() {
	//dead ports
	motors["BackLeft"] = std::make_shared<Motor>("BackLeft", motorGearset::GS600, 18);
	motors["BackRight"] = std::make_shared<Motor>("BackRight", motorGearset::GS600, 20, true);
	motors["FrontLeft"] = std::make_shared<Motor>("FrontLeft", motorGearset::GS600, 2);
	motors["FrontRight"] = std::make_shared<Motor>("FrontRight", motorGearset::GS600, 7, true);
	motors["Conveyor"] = std::make_shared<Motor>("Conveyor", motorGearset::GS200, 21, true);
	makeMotorGroup("LeftDrive", {"BackLeft", "FrontLeft"});
	makeMotorGroup("RightDrive", {"BackRight", "FrontRight"});
	makeMotorGroup("Drive", {"BackLeft", "BackRight", "FrontLeft", "FrontRight"});
	pistons["RightLift"] = std::make_unique<Piston>("RightLift", 3);
	pistons["LeftLift"] = std:: make_unique<Piston>("LeftLift", 4);
	
	// sensors["Left Encoder"] = std::make_unique<Sensor>("Left", sensorClass::encoder, 1);
	// sensors["Middle Encoder"] = std::make_unique<Sensor>("Middle", sensorClass::encoder, 3);
	// sensors["Right Encoder"] = std::make_unique<Sensor>("Right", sensorClass::encoder, 5);
	sensors["Inertial"] = std::make_unique<Sensor>("Inertial", sensorClass::inertial, 6);
	sensors["Left Rotation"] = std::make_unique<Sensor>("Left", sensorClass::rotation, 5);
	sensors["Right Rotation"] = std::make_unique<Sensor>("Right", sensorClass::rotation, 9);

	getInertial("Inertial")->reset();
	pros::delay(3000);
	getRotation("Right")->reverse();
	getRotation("Left")->reset_position();
	getRotation("Right")->reset_position();
}

Robot* Robot::Instance() {
    if (!pInstance) {
        pInstance = new Robot();
    }
    return pInstance;
}

void Robot::mecanum(int power, int strafe, int turn) {
	*(getMotor("BackLeft")) = power - strafe + turn;
	*(getMotor("BackRight")) = power + strafe - turn;
	*(getMotor("FrontLeft")) = power + strafe + turn;
	*(getMotor("FrontRight")) = power - strafe - turn;
	
}

void Robot::arcade(int power, int turn) {
	*(getMotorGroup("LeftDrive")) = power + turn;
	*(getMotorGroup("RightDrive")) = power - turn;
}

void Robot::tank(int left, int right) {
	*(getMotorGroup("LeftDrive")) = left;
	*(getMotorGroup("RightDrive")) = right;
}


void Robot::stopDrive() {
    getMotorGroup("Drive")->stop(brakeType::brake);
}

void Robot::makeMotorGroup(std::string name, std::vector<std::string> motorNames) {
	std::vector<std::shared_ptr<Motor>> motorGroupMotors;
	for (auto const& motorName : motorNames) {
		motorGroupMotors.push_back(motors[motorName]);
	}
	motorGroups[name] = std::make_unique<MotorGroup>(name, motorGroupMotors);
}

pros::Motor* Robot::getMotor(std::string name) {
	return motors.at(name).get()->getMotor();
}

MotorGroup* Robot::getMotorGroup(std::string name) {
	return motorGroups.at(name).get();
}

pros::ADIDigitalOut* Robot::getPiston(std::string name) {
	return pistons.at(name).get()->getPiston();
}

pros::ADIButton* Robot::getButton(std::string name) {
    return Sensor::button.at(name).get();
}
pros::ADILineSensor* Robot::getLine(std::string name) {
	return Sensor::line.at(name).get();
}
pros::ADIPotentiometer* Robot::getPotentiometer(std::string name) {
	return Sensor::potentiometer.at(name).get();
}
pros::ADIUltrasonic* Robot::getUltrasonic(std::string name) {
	return Sensor::ultrasonic.at(name).get();
}
pros::ADIEncoder* Robot::getEncoder(std::string name) {
	return Sensor::encoder.at(name).get();
}
pros::Rotation* Robot::getRotation(std::string name) {
    return Sensor::rotation.at(name).get();
}
pros::Imu* Robot::getInertial(std::string name) {
	return Sensor::inertial.at(name).get();
}
pros::Vision* Robot::getVision(std::string name) {
	return Sensor::vision.at(name).get();
}