#include "includes.hpp"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	if (Thread::existsTask("move")) {
		Thread::killTask("move");
	}
	Thread::startTask("update", Controller::update);
	Thread::startTask("drive", robotFuncs::drive);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_B, robotFuncs::testmove);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_A, robotFuncs::toggleLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_X, robotFuncs::toggleConveyor);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_L1, robotFuncs::slowConveyor);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_L2, robotFuncs::fastConveyor);

	while (true) {
		// pros::lcd::set_text(4, "Right: " + std::to_string(sRobot->getRotation("Right")->get_position()));
		// pros::lcd::set_text(5, "Left: " + std::to_string(sRobot->getRotation("Left")->get_position()));
		// pros::lcd::set_text(5, "turnErr: " + std::to_string(sOdom->turnErr));

		sController->act();
		pros::delay(20);
	}
}