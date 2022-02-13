#include "includes.hpp"

void right() {
	sOdom->setTarget(0, 1850, {0.21, 0.0002, 0.5}, 40, {3.0, 0.001, 0.4}, 5.0);
	robotFuncs::toggleFrontClamp();
	while (sOdom->currentPos.y < 1675) {
		pros::delay(2);
	}
    // sOdom->waitUntilStop();
    robotFuncs::toggleFrontClamp();
    
    sOdom->setTargetNow(0, 950, {0.28, 0.0001, 0.65}, 50, {5.0, 0.01, 1.0}, 7.0);
    robotFuncs::autonLift(liftStates::lowmid);
	sOdom->waitUntilStop();

    sOdom->setTarget(-85.0, {1.7, 0.003, 1.8}, 2.0);
	robotFuncs::toggleTilter();
    sOdom->setTarget(585, 1000, {0.27, 0.0005, 0.3}, 30, {5.0, 0.02, 1.0}, 5.0);
    sOdom->waitUntilStop();
    robotFuncs::toggleBackClamp();
    pros::delay(250);
	robotFuncs::toggleTilter();

    // sOdom->setTarget(-900, 1025, {0.2, 0.0005, 0.28}, 80, {2.0, 0.01, 1.0}, 10.0);
	// while(sOdom->currentPos.x > -500) {
	// 	pros::delay(2);
	// }
	// sOdom->setTargetNow(0, 0, {0, 0, 0}, 2000, {0, 0, 0}, 100);
	// robotFuncs::autonLift(liftStates::low);
    // sOdom->waitUntilStop();
	// robotFuncs::auton_conveyor(2000);
	// robotFuncs::toggleTilter();

	// sOdom->setTargetNow(-45, {1.7, 0.001, 1.5}, 3.0);
	// robotFuncs::toggleFrontClamp();
}

void rightwp() {
	// sOdom->setTarget(550.0, 0.0, {0.3, 0.0001, 0.25}, 10.0);
	// sOdom->setTarget(0.0, -90.0, {0, 0, 0}, 20.0, {1.9, 0.005, 1.4}, 1.0);
	// robotFuncs::toggle_pLift();

	// sOdom->setTarget(-520.0, -90.0, {0.45, 0.001, 0.3});
	// sOdom->waitUntilStop();
	// robotFuncs::toggle_pLift();
	// pros::delay(750);

	// sOdom->setTarget(450.0, -90.0, {0.3, 0.0001, 0.2});
	// sOdom->waitUntilStop();
	// robotFuncs::auton_conveyor(2000);
}

void middle() {
	// sOdom->setTarget(0, 49.0, {0.0, 0.0, 0.0}, 20, {2.5, 0.005, 1.4}, 1.0);
	// robotFuncs::auton_mLift(mLiftStates::low);
	// sOdom->setTarget(2315, 49.0, {0.24, 0.00004, 0.4});
	// *(sRobot->getMotor("Conveyor")) = 127;
	// robotFuncs::auton_declamp(1000);
	// sOdom->waitUntilStop();
	// *(sRobot->getMotor("Conveyor")) = 0;
	// robotFuncs::auton_clamp(700);
	// robotFuncs::auton_mLift(mLiftStates::lowmid);
}

void leftwp() {
	sOdom->setTarget(0, -335, {0.25, 0.0001, 0.2}, 30, {1.0, 0.001, 0.5}, 5);
	robotFuncs::autonLift(liftStates::low);
	robotFuncs::toggleTilter();
	sOdom->waitUntilStop();
	robotFuncs::toggleBackClamp();
	robotFuncs::toggleTilter();
	sOdom->setTarget(0, 0, {0.25, 0.0001, 0.2}, 70, {1.0, 0.0, 0.7}, 10);
	sOdom->waitUntilStop();
	robotFuncs::auton_conveyor(2000);
}

void leftneutral() {
	sOdom->setTarget(0, 1900, {0.21, 0.0002, 0.5}, 40, {3.0, 0.001, 0.4}, 5.0);
    robotFuncs::toggleFrontClamp();
	while (sOdom->currentPos.y < 1725) {
		pros::delay(2);
	}
	robotFuncs::toggleFrontClamp();
    
    sOdom->setTargetNow(0, 600, {0.15, 0.00005, 0.65}, 30, {5.0, 0.01, 1.0}, 7.0);
    // robotFuncs::autonLift(liftStates::mid);
	// sOdom->setTarget(-100, {1.75, 0.005, 1.5}, 2.0);
	// sOdom->setTarget(600, 170, {0.2, 0.00035, 0.25}, 30, {5.0, 0.02, 1.0}, 5.0);
}

void skills() {
	sOdom->setTarget(0, 1800, {0.16, 0.0003, 0.5}, 40, {3.0, 0.001, 0.4}, 5.0);
    robotFuncs::toggleFrontClamp();
    robotFuncs::toggleBackClamp();
    sOdom->waitUntilStop();
    robotFuncs::toggleFrontClamp();
    
    sOdom->setTarget(0, 1130, {0.27, 0.0001, 0.3}, 30, {5.0, 0.01, 1.0}, 7.0);
    robotFuncs::autonLift(liftStates::low);

    sOdom->setTarget(-68.0, {1.5, 0.001, 1.5}, 2.0);
    sOdom->setTarget(697, 850, {0.32, 0.0001, 0.2}, 30, {5.0, 0.03, 1.0}, 5.0);
    sOdom->waitUntilStop();
    robotFuncs::toggleBackClamp();
    pros::delay(250);

    sOdom->setTarget(472, 933, {0.4, 0.0001, 0.3}, 50, {5.0, 0.01, 1.0}, 5.0);
    sOdom->waitUntilStop();
    robotFuncs::toggleTilter();
    sOdom->setTarget(0, {1.8, 0.001, 1.5}, 2.0);
    sOdom->setTarget(426, 4000, {0.15, 0.00001, 0.3}, 100, {3.0, 0.01, 1.0}, 5.0);
	sOdom->waitUntilStop();
	// robotFuncs::toggleFrontClamp();

	// sOdom->setTarget(554, 2284, {0.3, 0.0001, 0.2}, 30, {2.0, 0.01, 0.5}, 5.0);
	// sOdom->setTarget(90, {1.5, 0.001, 1.5}, 2.0);
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
	Thread::startTask("lift", robotFuncs::liftPID);

	switch (autonselect) {
		case autonSelect::leftwp:
			leftwp();
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
		case autonSelect::skills:
			skills();
			break;
		case autonSelect::test:
			test();
			break;
    }
	sOdom->waitUntilStop();
	Thread::killTask("move");
	sRobot->stopDrive();
}