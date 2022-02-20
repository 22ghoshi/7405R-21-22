#include "includes.hpp"

void right() {
	robotFuncs::toggleFrontClamp();

	sOdom->setTarget(0, 2000, {0.135, 0.0000, 1.0}, 40, {3.0, 0.001, 0.4}, 5.0);
	robotFuncs::autonLift(liftStates::low);
	while ((sOdom->currentPos.y < 1400 || sRobot->getUltrasonic("Front")->get_value() > 85) && sOdom->currentPos.y < 1800) {
		pros::delay(2);
	};
	robotFuncs::toggleFrontClamp();
	sOdom->setTargetNow(0, 1150, {0.25, 0.0005, 1.0}, 40, {4.0, 0.001, 1.5}, 7.0);
	robotFuncs::autonLift(liftStates::lowmid);
	pros::delay(1000);
	sOdom->setTarget(-90.0, {1.45, 0.005, 2.4}, 2);
	sOdom->waitUntilStop();
	pros::delay(1000);
	sOdom->setTarget(800, 1100, {0.135, 0.0001, 0.45}, 30, {5.0, 0.02, 1.0}, 8.0);
	robotFuncs::toggleBackClamp();
	sOdom->waitUntilStop();
    robotFuncs::toggleBackClamp();
    pros::delay(250);
	robotFuncs::toggleTilter();
	sOdom->setTarget(-600, 1100, {0.25, 0.005, 0.25}, 80, {5.0, 0.01, 1.0}, 10.0);
	while(sOdom->currentPos.x > -300) {
		pros::delay(2);
	}
	sOdom->setTargetNow(0, 0, {0, 0, 0}, 2000, {0, 0, 0}, 100);
	robotFuncs::autonLift(liftStates::lowmid);
    sOdom->waitUntilStop();
	robotFuncs::auton_conveyor(2000);
	robotFuncs::toggleTilter();
	robotFuncs::toggleBackClamp();
}

void rightneutral() {
	robotFuncs::toggleFrontClamp();

	sOdom->setTarget(0, 2000, {0.14, 0.0000, 1.0}, 40, {3.0, 0.001, 0.4}, 5.0);
	robotFuncs::autonLift(liftStates::downlow);
	while ((sOdom->currentPos.y < 1500 || sRobot->getUltrasonic("Front")->get_value() > 85) && sOdom->currentPos.y < 1800) {
		pros::delay(2);
	};
	robotFuncs::toggleFrontClamp();
	sOdom->setTargetNow(0, 750, {0.24, 0.0004, 1.0}, 60, {4.0, 0.001, 1.5}, 7.0);
	robotFuncs::autonLift(liftStates::lowmid);
	pros::delay(750);
	
	// sOdom->setTarget(50, {1.8, 0.001, 1.5}, 3);
	// sOdom->waitUntilStop();
	// robotFuncs::toggleFrontClamp();
	// robotFuncs::autonLift(liftStates::high);

	// sOdom->setTarget(-36, {1.75, 0.001, 2.1}, 3);
	// robotFuncs::autonLift(liftStates::downlow);
	// sOdom->setTarget(-1250, 2575, {0.16, 0.00001, 1.0}, 40, {2.5, 0.001, 0.5}, 5.0);
	// while ((sOdom->currentPos.y < 1500 || sRobot->getUltrasonic("Front")->get_value() > 70) && sOdom->currentPos.y < 3000) {
	// 	pros::delay(2);
	// }
	// robotFuncs::toggleFrontClamp();
	// sOdom->setTargetNow(-785, 988, {0.18, 0.001, 0.7}, 60, {5.0, 0.001, 0.5}, 15.0);
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
	robotFuncs::toggleFrontClamp();

	sOdom->setTarget(0, 2300, {0.14, 0.000, 1.0}, 40, {3.0, 0.001, 0.4}, 5.0);
	robotFuncs::autonLift(liftStates::low);
	sOdom->waitUntilStop();
	robotFuncs::toggleFrontClamp();
	sOdom->setTarget(0, 500, {0.15, 0.00001, 0.5}, 60, {2.0, 0.001, 0.4}, 10.0);
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
	robotFuncs::toggleFrontClamp();

	sOdom->setTarget(0, 2300, {0.135, 0.0000, 1.1}, 40, {3.0, 0.001, 0.4}, 5.0);
	robotFuncs::autonLift(liftStates::downlow);
	while ((sOdom->currentPos.y < 1400 || sRobot->getUltrasonic("Front")->get_value() > 50) && sOdom->currentPos.y < 2100) {
		pros::delay(2);
	};
	robotFuncs::toggleFrontClamp();
	sOdom->setTargetNow(0, 750, {0.25, 0.0005, 0.8}, 40, {4.0, 0.001, 1.5}, 7.0);
	robotFuncs::autonLift(liftStates::lowmid);
	pros::delay(750);
	
	// sOdom->setTarget(-50, {1.8, 0.001, 1.5}, 3);
	// sOdom->waitUntilStop();
	// robotFuncs::toggleFrontClamp();
	// robotFuncs::autonLift(liftStates::high);
	
	// sOdom->setTarget(30, {1.7, 0.0005, 2.1}, 3);
	// robotFuncs::autonLift(liftStates::downlow);
	// sOdom->setTarget(1380, 2880, {0.16, 0.00001, 1.0}, 40, {4.0, 0.001, 0.5}, 5.0);
	// while ((sOdom->currentPos.y < 1500 || sRobot->getUltrasonic("Front")->get_value() > 70) && sOdom->currentPos.y < 3000) {
	// 	pros::delay(2);
	// }
	// robotFuncs::toggleFrontClamp();
	// sOdom->setTargetNow(143, 1045, {0.18, 0.001, 0.7}, 60, {5.0, 0.001, 0.5}, 15.0);

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
	Thread::startTask("lift", robotFuncs::liftPID);
	robotFuncs::toggleTilter();

	switch (autonselect) {
		case autonSelect::leftwp:
			Thread::startTask("move", Odometry::moveTo);
			leftwp();
			break;
		case autonSelect::leftneutral:
			Thread::startTask("move", Odometry::moveTo);
			leftneutral();
			break;
		case autonSelect::middle:
			Thread::startTask("move", Odometry::moveTo);
			middle();
			break;
		case autonSelect::right:
			Thread::startTask("move", Odometry::moveTo);
			right();
			break;
		case autonSelect::rightneutral:
			Thread::startTask("move", Odometry::moveTo);
			rightneutral();
			break;
		case autonSelect::rightwp:
			Thread::startTask("move", Odometry::moveTo);
			rightwp();
			break;
		case autonSelect::skills:
			Thread::startTask("move", Odometry::moveTo);
			skills();
			break;
		case autonSelect::test:
			Thread::startTask("move", Odometry::moveTo);
			test();
			break;
    }
	sOdom->waitUntilStop();
	Thread::notifyTask("move");
	pros::delay(20);
	Thread::killTask("move");
	sRobot->stopDrive();
}