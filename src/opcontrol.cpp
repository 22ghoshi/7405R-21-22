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
	//Thread::startTask("lift", robotFuncs::liftPID);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_B, robotFuncs::moveLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_Y, robotFuncs::moveLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_X, robotFuncs::moveLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_A, robotFuncs::moveLift);
	sController->registerButton(pros::E_CONTROLLER_DIGITAL_L1, robotFuncs::manualmoveLift, robotFuncs::manualholdLift);
	sController->registerButton(pros::E_CONTROLLER_DIGITAL_L2, robotFuncs::manualmoveLift, robotFuncs::manualholdLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_RIGHT, robotFuncs::toggleFrontClamp);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_LEFT, robotFuncs::toggleBackClamp);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_UP, robotFuncs::toggleTilter);
	// sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_UP, robotFuncs::toggleConveyor);
	// sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_RIGHT, robotFuncs::toggleConveyor);

	while (true) {
		pros::lcd::set_text(1, "Right: " + std::to_string(sRobot->getEncoder("Right")->get_value()));
		pros::lcd::set_text(2, "Left: " + std::to_string(sRobot->getEncoder("Left")->get_value()));
		pros::lcd::set_text(3, "Lift: " + std::to_string(sRobot->getPotentiometer("Lift")->get_value()));
		// pros::lcd::set_text(3, "Avg: " + std::to_string((sRobot->getRotation("Left")->get_position() + sRobot->getRotation("Right")->get_position()) / 200.0));
		pros::lcd::set_text(4, "Inertial: " + std::to_string(sRobot->getInertial("Inertial")->get_rotation()));

		// pros::lcd::set_text(1, "backleft: " + std::to_string(sRobot->getMotor("BackLeft")->get_temperature()));
		// pros::lcd::set_text(2, "backright: " + std::to_string(sRobot->getMotor("BackRight")->get_temperature()));
		// pros::lcd::set_text(3, "frontleft: " + std::to_string(sRobot->getMotor("FrontLeft")->get_temperature()));
		// pros::lcd::set_text(4, "frontright: " + std::to_string(sRobot->getMotor("FrontRight")->get_temperature()));
		// printf("\nbackleft: %d", (int)sRobot->getMotor("BackLeft")->get_temperature());
		// printf("\nbackright: %d", (int)sRobot->getMotor("BackRight")->get_temperature());
		// printf("\nfrontleft: %d", (int)sRobot->getMotor("FrontLeft")->get_temperature());
		// printf("\nfrontright: %d", (int)sRobot->getMotor("FrontRight")->get_temperature());
		

		
		// pros::lcd::set_text(5, "turnErr: " + std::to_string(sOdom->turnErr));
		// pros::lcd::set_text(5, "Lift: " + std::to_string(sRobot->getPotentiometer("mLift")->get_value()));
		// pros::lcd::set_text(5, "clamp: " + std::to_string(sRobot->getMotor("Clamp")->get_position()));

		sController->act();
		pros::delay(20);
	}
}