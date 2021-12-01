#include "includes.hpp"

void left() {
	//straight to neutral mogo
	sOdom->setTarget(0, 1550, 0, {0.23, 0.00001, 0.5}, 20, {1.0, 0.00, 0.4}, 10.0);
	robotFuncs::auton_mLift(mLiftStates::low);
	sOdom->waitUntilStop();
	robotFuncs::toggle_pLift();
	robotFuncs::auton_clamp();
	robotFuncs::auton_mLift(mLiftStates::lowmid);


	//turn around and grab alliance mogo
	sOdom->setTarget(450, 1120, -40, {0.3, 0.00005, 0.2}, 20, {2.0, 0.005, 2.5}, 5.0);
	sOdom->waitUntilStop();
	robotFuncs::toggle_pLift();
	

	//turn around parallel with line, drop neutral mogo

	//turn parallel to wall, drive forward

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
	Thread::startTask("lift", robotFuncs::mLiftPID);
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