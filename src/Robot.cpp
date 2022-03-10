#include "Robot.hpp"

//dead ports 2 4 5 6 10 12 14 19

std::map<std::string, std::unique_ptr<pros::ADIEncoder>> encoders;

void Robot::mecanum(int power, int strafe, int turn) {
	getMotor<motors::BackLeft>() = power - strafe + turn;
	getMotor<motors::BackRight>() = power + strafe - turn;
	getMotor<motors::FrontLeft>() = power + strafe + turn;
	getMotor<motors::FrontRight>() = power - strafe - turn;
	
}
void Robot::arcade(int power, int turn) {
	getMotorGroup<motorGroups::RightDrive>() = power - turn;
	getMotorGroup<motorGroups::LeftDrive>() = power + turn;
}
void Robot::tank(int left, int right) {
	getMotorGroup<motorGroups::LeftDrive>() = left;
	getMotorGroup<motorGroups::RightDrive>() = right;
}
void Robot::stopDrive() {
	getMotorGroup<motorGroups::Drive>().stop(brakeType::brake);
}
