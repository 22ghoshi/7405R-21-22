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
	Thread::startTask("lift", robotFuncs::mLiftPID);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_B, robotFuncs::move_mLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_Y, robotFuncs::move_mLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_X, robotFuncs::move_mLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_A, robotFuncs::move_mLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_UP, robotFuncs::toggle_pLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_L1, robotFuncs::toggleConveyor);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_L2, robotFuncs::toggleConveyor);
	sController->registerButton(pros::E_CONTROLLER_DIGITAL_R1, robotFuncs::clamp, robotFuncs::holdClamp);
	sController->registerButton(pros::E_CONTROLLER_DIGITAL_R2, robotFuncs::clamp, robotFuncs::holdClamp);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_LEFT, robotFuncs::testmove);


	while (true) {
		// pros::lcd::set_text(4, "Right: " + std::to_string(sRobot->getRotation("Right")->get_position()));
		// pros::lcd::set_text(5, "Left: " + std::to_string(sRobot->getRotation("Left")->get_position()));
		// pros::lcd::set_text(5, "turnErr: " + std::to_string(sOdom->turnErr));
		pros::lcd::set_text(4, "Lift: " + std::to_string(sRobot->getPotentiometer("mLift")->get_value()));

		sController->act();
		pros::delay(20);
	}
}