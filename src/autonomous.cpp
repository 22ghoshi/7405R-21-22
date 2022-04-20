#include "includes.hpp"

int time = 0;
int startTime = 0;
int currTime = 0;

void right() {
	startTime = pros::millis();
	printf("\nstartTime = %d", startTime);
	robotFuncs::toggleFrontClamp();
	robotFuncs::toggleTilter();

	sOdom->setTarget(0, 2200, {0.28, 0.0001, 0.45}, 40, {4.0, 0.001, 0.4}, 15.0);
	robotFuncs::autonLift(liftStates::low);
	while ((sOdom->currentPos.y < 1000 || Robot::get<sensors::FrontDistance>().get() > 95 || fabs(Robot::get<sensors::FrontDistance>().get()) < 1) && sOdom->currentPos.y < 2100) {
		pros::delay(2);
	};
	robotFuncs::toggleFrontClamp();
	robotFuncs::autonLift(liftStates::lowmid);
	robotFuncs::conveyorIn();
	robotFuncs::distanceBack(735);
	sOdom->setTargetNow(-90, {1.5, 0.0065, 2.6}, 1.5);
	sOdom->waitUntilStop();
	sOdom->setTarget(800, 55, {0.15, 0.0001, 0.45}, 50, {4.0, 0.02, 1.0}, 8.0);
	sOdom->waitUntilStop();
    robotFuncs::toggleBackClamp();
    pros::delay(250);
	robotFuncs::toggleTilter();
	sOdom->setTarget(580, 65, {0.25, 0.005, 0.4}, 80, {5.0, 0.01, 1.0}, 10.0);
	sOdom->setTarget(0, {1.4, 0.005, 2.4}, 1.5);
	robotFuncs::conveyorIn();
	robotFuncs::autonLift(liftStates::mid);
	sOdom->setTarget(530, 1400, {0.06, 0.00003, 0.35}, 160, {1.0, 0.01, 3.0});
	currTime = pros::millis() - startTime;
	printf("\ncurrTime = %d", currTime);
	while (sOdom->currentPos.y.load() < 1200 && currTime < 12100) {
		currTime = pros::millis() - startTime;
		printf("\ncurrTIme = %d", currTime);
		pros::delay(20);
	}
	sOdom->setTargetNow(530, -100, {0.07, 0.001, 1.0}, 100, {2.0, 0.01, 2.0});
    sOdom->waitUntilStop();
	robotFuncs::autonLift(liftStates::lowmid);
	robotFuncs::toggleTilter();
	pros::delay(250);
	robotFuncs::toggleBackClamp();
}

void rightneutral() {
	robotFuncs::toggleFrontClamp();

	sOdom->setTarget(0, 2000, {0.14, 0.0000, 1.0}, 40, {3.0, 0.001, 0.4}, 5.0);
	robotFuncs::autonLift(liftStates::downlow);
	while ((sOdom->currentPos.y < 1500 || Robot::get<sensors::FrontDistance>().get() > 85) && sOdom->currentPos.y < 1800) {
		pros::delay(2);
	};
	robotFuncs::toggleFrontClamp();
	sOdom->setTargetNow(0, 750, {0.24, 0.0004, 1.0}, 60, {4.0, 0.001, 1.5}, 7.0);
	robotFuncs::autonLift(liftStates::lowmid);
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
}

void left() {
	robotFuncs::toggleFrontClamp();
	robotFuncs::toggleTilter();

	sOdom->setTarget(0, 2200, {0.28, 0.0001, 0.45}, 40, {4.0, 0.001, 0.4}, 15.0);
	robotFuncs::autonLift(liftStates::low);
	while ((sOdom->currentPos.y < 1400 || Robot::get<sensors::FrontDistance>().get() > 95 || fabs(Robot::get<sensors::FrontDistance>().get()) < 1) && sOdom->currentPos.y < 2050) {
		pros::delay(2);
	};
	robotFuncs::toggleFrontClamp();
	robotFuncs::autonLift(liftStates::lowmid);
	robotFuncs::conveyorIn();
	robotFuncs::distanceBack(170);
	
	sOdom->setTargetNow(-100, {1.8, 0.001, 2.0}, 1.5);
	sOdom->waitUntilStop();
	Thread::pauseTask("move");
	pros::delay(500);
	
	// sOdom->setTarget(430, sOdom->currentPos.y.load(), {0.18, 0.001, 0.5}, 70, {4.5, 0.01, 3.5});
	Robot::tank(-55, -77);
	while (Robot::get<sensors::BackDistance>().get() > 40 && time < 625) {
		printf("\ndist = %d", (int)Robot::get<sensors::BackDistance>().get());
		pros::delay(2);
		time += 2;
	}
	Robot::stopDrive();
	robotFuncs::toggleBackClamp();
	pros::delay(250);
	robotFuncs::toggleTilter();
	pros::delay(250);
	Robot::arcade(80, 0);
	pros::delay(250);
	Robot::arcade(40, 0);
	robotFuncs::conveyorIn();
	pros::delay(1000);
	Robot::stopDrive();
	pros::delay(1500);
	// robotFuncs::toggleTilter();
	// robotFuncs::toggleBackClamp();
}

void skills() {
	robotFuncs::toggleTilter();
	robotFuncs::toggleFrontClamp();

	//align with red mogo
	sOdom->setTarget(0, -200, {0.26, 0.00, 0.22}, 15);
	sOdom->setTarget(54, {1.4, 0.0, 1.6}, 1.5);

	// pick up red mogo
	sOdom->setTarget(-575, -615, {0.13, 0.001, 0.5}, 40, {3.0, 0.01, 1.0}, 6);
	sOdom->waitUntilStop();
	robotFuncs::toggleBackClamp();
	pros::delay(500);
	robotFuncs::toggleTilter();
	robotFuncs::autonLift(liftStates::lowmid);
	pros::delay(500);

	//go to middle mogo
	sOdom->setTargetNow(160, {1.5, 0.0, 2.25}, 1.5);
	sOdom->setTarget(0, -1800, {0.1, 0.001, 0.3}, 70, {4.5, 0.01, 1.0}, 7);
	while((Robot::get<sensors::FrontDistance>().get() > 48 || fabs(Robot::get<sensors::FrontDistance>().get()) < 1) && time < 2500) {
		if (sOdom->currentPos.y < -1375) {
			robotFuncs::autonLift(liftStates::down);
		}
		time += 2;
		pros::delay(2);
	}
	robotFuncs::toggleFrontClamp();

	//score neutral
	sOdom->setTargetNow(1350, -4000, {0.095, 0.0003, 1.5}, 115, {5.7, 0.06, 2.3}, 15);
	robotFuncs::autonLift(liftStates::veryhigh);
	while (sOdom->currentPos.y.load() > -3600) {
		pros::delay(2);
	}
	sOdom->waitUntilStop();
	pros::delay(500);
	robotFuncs::autonLift(liftStates::mid);
	while (Robot::get<sensors::LiftPotentiometer>().get_value() > 1940) {
		pros::delay(2);
	}
	robotFuncs::toggleFrontClamp();
	robotFuncs::autonLift(liftStates::high);
	pros::delay(250);

	//back up and drop red
	sOdom->setTarget(890, -3330, {0.2, 0.0005, 0.4}, 50, {4.0, 0.01, 2.0}, 5);
	pros::delay(500);
	robotFuncs::autonLift(liftStates::lowmid);
	sOdom->waitUntilStop();
	pros::delay(250);
	robotFuncs::toggleTilter();
	pros::delay(250);
	robotFuncs::toggleBackClamp();
	pros::delay(250);
	sOdom->setTarget(1100, -3620, {0.25, 0.001, 0.3}, 50, {4.0, 0.01, 2.0}, 7);
	sOdom->setTarget(-32, {1.5, 0.001, 3.0}, 1.5);
	robotFuncs::autonLift(liftStates::low);
	sOdom->waitUntilStop();
	pros::delay(150);
	sOdom->setTarget(960, -3300, {0.23, 0.0015, 0.3}, 45, {3.5, 0.01, 2.5}, 5);
	sOdom->waitUntilStop();
	robotFuncs::toggleFrontClamp();
	pros::delay(250);
	robotFuncs::autonLift(liftStates::high);

	//score red
	sOdom->setTarget(-192, {1.2, 0.0001, 4.7}, 1.5);
	sOdom->setTarget(1260, -4300, {0.12, 0.00007, 0.55}, 100, {4.5, 0.01, 2.0}, 20);
	sOdom->waitUntilStop();
	pros::delay(250);
	robotFuncs::autonLift(liftStates::mid);
	while (Robot::get<sensors::LiftPotentiometer>().get_value() > 1975) {
		pros::delay(2);
	}
	robotFuncs::toggleFrontClamp();
	robotFuncs::autonLift(liftStates::high);
	pros::delay(500);

	//back out and go to blue mogo
	sOdom->setTarget(1000, -3466, {0.1, 0.0005, 0.6}, 50, {2.5, 0.001, 3.0}, 10);
	sOdom->setTarget(-127, {1.9, 0.001, 2.7}, 1.5);
	sOdom->setTarget(-75, -4300, {0.13, 0.00005, 0.5}, 55, {4.5, 0.001, 2.0}, 7);
	robotFuncs::autonLift(liftStates::lowmid);
	sOdom->setTarget(-220, {1.0, 0.002, 2.7}, 2);
	pros::delay(300);
	robotFuncs::autonLift(liftStates::low);
	sOdom->setTarget(350, -4725, {0.17, 0.001, 0.45}, 65, {4.0, 0.02, 2.0}, 10);
	sOdom->waitUntilStop();
	robotFuncs::toggleFrontClamp();
	pros::delay(250);
	sOdom->setTarget(-120, -4142, {0.25, 0.001, 0.35}, 30, {4.0, 0.01, 2.5});
	robotFuncs::autonLift(liftStates::lowmid);

	// go to blue platform
	sOdom->setTarget(-330, {1.5, 0.001, 2.2}, 1.5);
	sOdom->setTarget(1530, -730, {0.07, 0.00004, 0.9}, 100, {3.0, 0.01, 2.5}, 15);
	pros::delay(800);
	robotFuncs::autonLift(liftStates::veryhigh);
	sOdom->setTarget(-363, {2.0, 0.01, 2.1});
	sOdom->waitUntilStop();
	robotFuncs::autonLift(liftStates::mid);
	pros::delay(800);
	robotFuncs::toggleFrontClamp();
	robotFuncs::autonLift(liftStates::high);

	//get middle
	sOdom->setTarget(1500, -1089, {0.3, 0.001, 0.3}, 30, {4.0, 0.01, 3.0}, 9);
	sOdom->setTarget(-550, {1.2, 0.0005, 2.6}, 1.5);
	robotFuncs::autonLift(liftStates::lowmid);
	sOdom->setTarget(1620, -2020, {0.15, 0.0003, 0.65}, 70, {4.0, 0.01, 2.0});
	// while (sOdom->currentPos.y.load() > -1760) {
	// 	pros::delay(2);
	// }
	// robotFuncs::autonLift(liftStates::low);
	sOdom->waitUntilStop();
	// robotFuncs::toggleFrontClamp();
	// pros::delay(250);
	// robotFuncs::autonLift(liftStates::lowmid);

	//bring it to zone
	sOdom->setTarget(-588, {1.7, 0.01, 2.3}, 2.0);
	sOdom->setTarget(3800, -4350, {0.09, 0.00005, 0.8}, 100, {4.0, 0.01, 2.0});
	sOdom->waitUntilStop();
	// robotFuncs::toggleFrontClamp();

	//back up and get alliance
	sOdom->setTarget(3520, -4080, {0.25, 0.0001, 0.3}, 30, {3.0, 0.01, 2.0}, 7);
	sOdom->setTarget(-472, {1.2, 0.003, 2.9}, 1.5);
	sOdom->setTarget(3970, -3920, {0.1, 0.004, 0.6}, 50, {3.0, 0.01, 1.5}, 8);
	sOdom->waitUntilStop();
	robotFuncs::toggleBackClamp();
	pros::delay(250);
	robotFuncs::toggleTilter();

	//get left neutral
	sOdom->setTarget(-386, {1.3, 0.001, 3.0}, 1.5);
	sOdom->setTarget(3360, -2720, {0.12, 0.0005, 0.55}, 40);
	time = 0;
	while((Robot::get<sensors::FrontDistance>().get() > 48 || fabs(Robot::get<sensors::FrontDistance>().get()) < 1) && time < 2500) {
		if (sOdom->currentPos.y > -3450) {
			robotFuncs::autonLift(liftStates::low);
		}
		pros::delay(2);
	}
	robotFuncs::toggleFrontClamp();

	//score neutral
	sOdom->setTargetNow(2245, -664, {0.08, 0.000015, 1.9}, 100, {5.0, 0.01, 2.0});
	robotFuncs::autonLift(liftStates::veryhigh);
	sOdom->waitUntilStop();
	robotFuncs::autonLift(liftStates::mid);
	pros::delay(1000);
	robotFuncs::toggleFrontClamp();
	robotFuncs::autonLift(liftStates::high);
	pros::delay(250);

	//back up and drop blue
	sOdom->setTarget(2555, -1400, {0.2, 0.0005, 0.4}, 50, {4.0, 0.01, 2.0}, 5);
	pros::delay(500);
	robotFuncs::autonLift(liftStates::lowmid);
	sOdom->waitUntilStop();
	pros::delay(250);
	robotFuncs::toggleTilter();
	pros::delay(250);
	robotFuncs::toggleBackClamp();
	pros::delay(250);
	sOdom->setTarget(2475, -1125, {0.25, 0.001, 0.3}, 50, {4.0, 0.01, 2.0}, 7);
	sOdom->setTarget(-562, {1.1, 0.0005, 3.1}, 2.0);
	robotFuncs::autonLift(liftStates::low);
	sOdom->waitUntilStop();
	pros::delay(250);
	sOdom->setTarget(2535, -1470, {0.23, 0.002, 0.3}, 45, {4.0, 0.01, 2.5}, 5);
	sOdom->waitUntilStop();
	robotFuncs::toggleFrontClamp();
	pros::delay(250);
	robotFuncs::autonLift(liftStates::veryhigh);

	//score blue
	sOdom->setTarget(-390, {1.0, 0.002, 3.4}, 1.5);
	sOdom->setTarget(1980, -390, {0.11, 0.001, 0.6}, 60, {4.0, 0.01, 2.5}, 10);
	sOdom->waitUntilStop();
	robotFuncs::toggleFrontClamp();
	pros::delay(500);

	//go to next alliance goal
	sOdom->setTarget(2430, -1260, {0.1, 0.0005, 0.6}, 50, {2.5, 0.001, 3.0}, 10);
	// sOdom->setTarget(-305, {1.9, 0.001, 2.7}, 1.5);
	// sOdom->setTarget(3240, -750, {0.12, 0.00005, 0.5}, 35, {4.5, 0.001, 2.0});
	// robotFuncs::autonLift(liftStates::lowmid);
	// sOdom->setTarget(-380, {1.3, 0.001, 2.5}, 2);
	// pros::delay(300);
	// robotFuncs::autonLift(liftStates::low);
	// sOdom->setTarget(2875, 50, {0.16, 0.002, 0.45}, 65, {4.5, 0.02, 2.0});
	// sOdom->waitUntilStop();
	// robotFuncs::toggleFrontClamp();
	// pros::delay(250);
	// sOdom->setTarget(3700, -480, {0.25, 0.001, 0.35}, 30, {4.0, 0.01, 2.5});

	//drive to other side
	// sOdom->setTarget(-550, {1.4, 0.001, 2.5});
	// robotFuncs::autonLift(liftStates::lowmid);
	// sOdom->setTarget(4014, -3650, {0.1, 0.000001, 1.0}, 200, {5.0, 0.01, 2.5}, 100);
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
	Thread::startTask("conveyor", robotFuncs::conveyor);
	Robot::get<sensors::Inertial>().tare_rotation();
	pros::delay(20);
	Thread::startTask("move", Odometry::moveTo);

	switch (autonselect) {
		case autonSelect::leftwp:
			leftwp();
			break;
		case autonSelect::left:
			left();
			break;
		case autonSelect::middle:
			middle();
			break;
		case autonSelect::right:
			right();
			break;
		case autonSelect::rightneutral:
			rightneutral();
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
	Thread::notifyTask("move");
	pros::delay(20);
	Thread::killTask("move");
	Robot::stopDrive();
}