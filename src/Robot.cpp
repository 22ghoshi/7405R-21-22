#include "Robot.hpp"

Robot* Robot::pInstance = NULL;

Robot::Robot() {
    motors["BackLeft"] = std::make_unique<pros::Motor>(1);
	motors["BackRight"] = std::make_unique<pros::Motor>(2);
	motors["FrontLeft"] = std::make_unique<pros::Motor>(3);
	motors["FrontRight"] = std::make_unique<pros::Motor>(4);

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
	*motors["BackLeft"] = power - strafe + turn;
	*motors["BackRight"] = power + strafe - turn;
	*motors["FrontLeft"] = power + strafe + turn;
	*motors["FrontRight"] = power - strafe - turn;
}

void Robot::arcade(int power, int turn) {
	*motors["BackLeft"] = power + turn;
	*motors["BackRight"] = power - turn;
	*motors["FrontLeft"] = power + turn;
	*motors["FrontRight"] = power - turn;
}

pros::Motor* Robot::getMotor(std::string name) {
	return motors.at(name).get();
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
pros::Imu* Robot::getInertial(std::string name) {
	return Sensor::inertial.at(name).get();
}
pros::Vision* Robot::getVision(std::string name) {
	return Sensor::vision.at(name).get();
}