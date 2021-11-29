#include "includes.hpp"

void left() {
	//straight to neutral mogo
	sOdom->setTarget(0, 1565, 0, {0.3, 0.00001, 0.4}, 20, {2.0, 0.1, 0.4}, 5);


	//back up, drop mogo
	sOdom->setTarget(483, 1093, -40, {0.2, 0.00005, 0.15}, 20, {2.0, 0.0075, 1.5}, 5.0);

	//align with and get alliance mogo

	//pull out and score rings

	//repeat for preloads

}

void middle() {
	//straight to neutral mogo

	//back up, drop mogo

	//align with and get alliance mogo

	//pull out and score rings

	//repeat for preloads

}

void right() {
	//straight to neutral mogo

	//back up, drop mogo

	//align with and get alliance mogo

	//pull out and score rings

	//repeat for preloads

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
		case autonSelect::left:
			left();
			break;
		case autonSelect::middle:
			middle();
			break;
		case autonSelect::right:
			right();
			break;
		case autonSelect::test:
			test();
			break;
    }
	sOdom->waitUntilStop();
	Thread::killTask("move");
	sRobot->stopDrive();
}