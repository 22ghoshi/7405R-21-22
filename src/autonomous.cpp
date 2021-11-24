#include "includes.hpp"

void wp() {
	//put down lift and move forward
	robotFuncs::toggle_pLift();
	pros::delay(1750);
	sOdom->setTargetPoint(0, 550, {0.37, 0.0015, 0.1}, 20);
	sOdom->waitUntilStop();

	//pick up mogo
	robotFuncs::toggle_pLift();
	pros::delay(1000);

	//move back and load rings
	sOdom->setTargetPoint(0, 130, {0.37, 0.0015, 0.1}, 20);
	sOdom->waitUntilStop();
	robotFuncs::toggleConveyor();
	pros::delay(2500);
	robotFuncs::toggleConveyor(); 
}

void nowp() {
	//put down lift and move forward
	robotFuncs::toggle_pLift();
	pros::delay(1750);
	sOdom->setTargetPoint(0, 720, {0.35, 0.0011, 0.1}, 20);
	sOdom->waitUntilStop();

	//pick up mogo
	robotFuncs::toggle_pLift();
	pros::delay(1500);

	//load rings
	sOdom->setTargetPoint(0, 250, {0.37, 0.0015, 0.1}, 20);
	robotFuncs::toggleConveyor();
	pros::delay(2500);
	robotFuncs::toggleConveyor(); 
}

void test() {
	
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    Thread::startTask("move", Odometry::moveTo);
	switch (autonselect) {
		case autonSelect::wp:
			wp();
			break;
		case autonSelect::nowp:
			nowp();
			break;
		case autonSelect::test:
			test();
			break;
    }
	sOdom->waitUntilStop();
	Thread::killTask("move");
	sRobot->stopDrive();
}