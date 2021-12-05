#include "includes.hpp"

void right() {
	//straight to neutral mogo
	sOdom->setTarget(1570, 0, {0.27, 0.000025, 0.3}, 20, {1.5, 0.00, 0.4}, 5.0);
	robotFuncs::auton_mLift(mLiftStates::low);
	robotFuncs::auton_declamp(1000);
	sOdom->waitUntilStop();
	robotFuncs::auton_clamp(700);
	robotFuncs::auton_mLift(mLiftStates::lowmid);


	//back up, turn, drop mogo, lower alliance lift
	sOdom->setTarget(-955, 0, {0.35, 0.0001, 0.27}, 20, {3.6, 0.02, 2.5}, 3.0);
	sOdom->setTarget(0, -90.0, {0.0, 0.0, 0.0}, 20, {1.9, 0.005, 1.4}, 1.0);
	sOdom->waitUntilStop();
	robotFuncs::toggle_pLift();
	robotFuncs::auton_declamp(350);
	
	//move back and pick up alliance mogo
	sOdom->setTarget(-350, -90.0, {0.45, 0.001, 0.3}, 20.0, {2.5, 0.0, 0.4});
	sOdom->waitUntilStop();
	robotFuncs::toggle_pLift();
	

	//bring out alliance mogo, score rings
	pros::delay(750);
	sOdom->setTarget(700.0, -90.0, {0.28, 0.0001, 0.25}, 50);
	sOdom->waitUntilStop();
	robotFuncs::auton_conveyor(2000);
}

void rightwp() {
	sOdom->setTarget(550.0, 0.0, {0.3, 0.0001, 0.25}, 10.0);
	sOdom->setTarget(0.0, -90.0, {0, 0, 0}, 20.0, {1.9, 0.005, 1.4}, 1.0);
	robotFuncs::toggle_pLift();

	sOdom->setTarget(-520.0, -90.0, {0.45, 0.001, 0.3});
	sOdom->waitUntilStop();
	robotFuncs::toggle_pLift();
	pros::delay(750);

	sOdom->setTarget(450.0, -90.0, {0.3, 0.0001, 0.2});
	sOdom->waitUntilStop();
	robotFuncs::auton_conveyor(2000);
}

void middle() {
	sOdom->setTarget(0, 49.0, {0.0, 0.0, 0.0}, 20, {2.5, 0.005, 1.4}, 1.0);
	robotFuncs::auton_mLift(mLiftStates::low);
	sOdom->setTarget(2315, 49.0, {0.24, 0.00004, 0.4});
	*(sRobot->getMotor("Conveyor")) = 127;
	robotFuncs::auton_declamp(1000);
	sOdom->waitUntilStop();
	*(sRobot->getMotor("Conveyor")) = 0;
	robotFuncs::auton_clamp(700);
	robotFuncs::auton_mLift(mLiftStates::lowmid);
}

void left() {
	//pick up alliance off platform
	robotFuncs::toggle_pLift();
	sOdom->setTarget(-275, 0, {0.45, 0.001, 0.3}, 10.0, {1.5, 0.0, 0.4}, 4.0);
	sOdom->waitUntilStop();
	robotFuncs::toggle_pLift();
	pros::delay(750);
	sOdom->setTarget(275, 0, {0.45, 0.001, 0.3}, 20.0, {1.5, 0.0, 0.4}, 4.0);
	sOdom->waitUntilStop();
	robotFuncs::auton_conveyor(2000);
	// robotFuncs::auton_mLift(mLiftStates::lowmid);
	// *(sRobot->getMotor("Conveyor")) = 127;
	// pros::delay(1000);
	// sOdom->setTarget(400, 0, {0.4, 0.001, 0.2});
	// sOdom->setTarget(-400, 0, {0.4, 0.001, 0.2});
	// pros::delay(1000);
	// sOdom->setTarget(400, 0, {0.4, 0.001, 0.2});
	// sOdom->setTarget(-400, 0, {0.4, 0.001, 0.2});
	// pros::delay(1000);
	// sOdom->setTarget(400, 0, {0.4, 0.001, 0.2});
	// sOdom->setTarget(-400, 0, {0.4, 0.001, 0.2});
}

void leftneutral() {
	sOdom->setTarget(1725, 0.0, {0.27, 0.00075, 0.3}, 40.0);
	robotFuncs::auton_mLift(mLiftStates::low);
	robotFuncs::auton_declamp(1000);
	sOdom->waitUntilStop();
	robotFuncs::auton_clamp(850);
	robotFuncs::auton_mLift(mLiftStates::lowmid);

	sOdom->setTarget(-1200, 0.0, {0.28, 0.0001, 0.25});
}

void test() {
	robotFuncs::auton_mLift(mLiftStates::low);
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
		case autonSelect::leftneutral:
			leftneutral();
			break;
		case autonSelect::middle:
			middle();
			break;
		case autonSelect::right:
			right();
			break;
		case autonSelect::rightwp:
			rightwp();
			break;
		case autonSelect::test:
			test();
			break;
    }
	sOdom->waitUntilStop();
	Thread::killTask("move");
	sRobot->stopDrive();
}