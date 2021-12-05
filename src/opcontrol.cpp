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
	sController->registerButton(pros::E_CONTROLLER_DIGITAL_L1, robotFuncs::manualmove_mLift, robotFuncs::manualhold_mLift);
	sController->registerButton(pros::E_CONTROLLER_DIGITAL_L2, robotFuncs::manualmove_mLift, robotFuncs::manualhold_mLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_LEFT, robotFuncs::toggle_pLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_UP, robotFuncs::toggleConveyor);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_RIGHT, robotFuncs::toggleConveyor);
	sController->registerButton(pros::E_CONTROLLER_DIGITAL_R1, robotFuncs::clamp, robotFuncs::holdClamp);
	sController->registerButton(pros::E_CONTROLLER_DIGITAL_R2, robotFuncs::clamp, robotFuncs::holdClamp);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_DOWN, robotFuncs::resetRotations);

	// *(sRobot->getMotor("Conveyor")) = 127;
	// robotFuncs::conveyorState = true;

	while (true) {
		pros::lcd::set_text(1, "Right: " + std::to_string(sRobot->getRotation("Right")->get_position()));
		pros::lcd::set_text(2, "Left: " + std::to_string(sRobot->getRotation("Left")->get_position()));
		pros::lcd::set_text(3, "Avg: " + std::to_string((sRobot->getRotation("Left")->get_position() + sRobot->getRotation("Right")->get_position()) / 200.0));
		pros::lcd::set_text(4, "Inertial: " + std::to_string(sRobot->getInertial("Inertial")->get_rotation()));
		pros::lcd::set_text(5, "clamp: " + std::to_string(sRobot->getMotor("Clamp")->get_position()));

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