#include "Robot.hpp"

Robot* Robot::pInstance = NULL;

Robot::Robot() {
	//dead ports 4 5 10
	motors["BackLeft"] = std::make_shared<Motor>("BackLeft", motorGearset::GS200, 19, true);
	motors["BackRight"] = std::make_shared<Motor>("BackRight", motorGearset::GS200, 12);
	motors["MidLeft"] = std::make_shared<Motor>("MidLeft", motorGearset::GS200, 16);
	motors["MidRight"] = std::make_shared<Motor>("MidRight", motorGearset::GS200, 11, true);
	motors["FrontLeft"] = std::make_shared<Motor>("FrontLeft", motorGearset::GS200, 18, true);
	motors["FrontRight"] = std::make_shared<Motor>("FrontRight", motorGearset::GS200, 13);
	makeMotorGroup("LeftDrive", {"BackLeft", "MidLeft", "FrontLeft"});
	makeMotorGroup("RightDrive", {"BackRight", "MidRight", "FrontRight"});
	makeMotorGroup("Drive", {"BackLeft", "BackRight", "MidLeft", "MidRight", "FrontLeft", "FrontRight"});
	motors["Conveyor"] = std::make_shared<Motor>("Conveyor", motorGearset::GS600, 1);
	motors["Lift"] = std::make_shared<Motor>("Lift", motorGearset::GS100, 17, true);
	
	pistons["FrontClamp"] = std::make_unique<Piston>("FrontClamp", 4);
	pistons["BackClamp"] = std:: make_unique<Piston>("BackClamp", 3);
	pistons["Tilter"] = std::make_unique<Piston>("Tilter", 2);
	
	sensors["Inertial"] = std::make_unique<Sensor>("Inertial", sensorClass::inertial, 20);
	sensors["Left Encoder"] = std::make_unique<Sensor>("Left", sensorClass::encoder, 5, true);
	sensors["Right Encoder"] = std::make_unique<Sensor>("Right", sensorClass::encoder, 7, true);
	sensors["Lift Potentiometer"] = std::make_unique<Sensor>("Lift", sensorClass::potentiometer, 1);

	getInertial("Inertial")->reset();
	pros::delay(3000);
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