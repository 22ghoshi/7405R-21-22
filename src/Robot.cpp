#include "Robot.hpp"

//dead ports 2 4 5 6 10 12 14 19

void Robot::mecanum(int power, int strafe, int turn) {
	get<motors::BackLeft>() = power - strafe + turn;
	get<motors::BackRight>() = power + strafe - turn;
	get<motors::FrontLeft>() = power + strafe + turn;
	get<motors::FrontRight>() = power - strafe - turn;
	
}
void Robot::arcade(int power, int turn) {
	get<motorGroups::RightDrive>() = power - turn;
	get<motorGroups::LeftDrive>() = power + turn;
}
void Robot::tank(int left, int right) {
	get<motorGroups::LeftDrive>() = left;
	get<motorGroups::RightDrive>() = right;
}
void Robot::stopDrive() {
	get<motorGroups::Drive>().stop(brakeType::brake);
}
