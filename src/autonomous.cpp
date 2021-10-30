#include "includes.hpp"

void wp() {
	//put down lift and move forward
	robotFuncs::toggleLift();
	sOdom->setTargetPoint(0, 600, {0.2, 0.0, 0.15});
	sOdom->waitUntilStop();

	//pick up mogo
	robotFuncs::toggleLift();
	pros::delay(1500);

	// //move back and load rings
	// sOdom->setTargetPoint(0, , { , , });
	// robotFuncs::toggleConveyor();
	// pros::delay(2500);
	// robotFuncs::toggleConveyor(); 
}

void nowp() {
	// //move forward
	// sOdom->setTargetPoint(0, , { , , });
	// sOdom->waitUntilStop();

	// //pick up mogo
	// robotFuncs::toggleLift();
	// pros::delay(1500);

	// //move back and load rings
	// sOdom->setTargetPoint(0, , { , , });
	// robotFuncs::toggleConveyor();
	// pros::delay(2500);
	// robotFuncs::toggleConveyor(); 
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