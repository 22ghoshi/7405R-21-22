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
		Thread::notifyTask("move");
		pros::delay(20);
		Thread::killTask("move");
	}
	Thread::startTask("update", Controller::update);
	Thread::startTask("drive", robotFuncs::drive);
	Thread::startTask("lift", robotFuncs::liftPID);
	Thread::startTask("conveyor", robotFuncs::conveyor);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_X, robotFuncs::moveLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_B, robotFuncs::moveLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_Y, robotFuncs::moveLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_A, robotFuncs::moveLift);
	sController->registerButton(pros::E_CONTROLLER_DIGITAL_R1, robotFuncs::manualmoveLift, robotFuncs::manualholdLift);
	sController->registerButton(pros::E_CONTROLLER_DIGITAL_R2, robotFuncs::manualmoveLift, robotFuncs::manualholdLift);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_L1, robotFuncs::toggleFrontClamp);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_L2, robotFuncs::toggleBackClamp);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_RIGHT, robotFuncs::toggleTilter);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_UP, robotFuncs::conveyorIn);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_DOWN, robotFuncs::conveyorOut);
	sController->registerButtonNewPress(pros::E_CONTROLLER_DIGITAL_LEFT, robotFuncs::showPotentiometer);
	// robotFuncs::toggleFrontClamp();
	// robotFuncs::toggleBackClamp();
	// robotFuncs::toggleTilter();

	// auto encoder = pros::c::adi_encoder_init(7, 8, false);
	while (true) {
		// pros::lcd::set_text(1, "X: " + std::to_string(sOdom->currentPos.x.load()));
		// pros::lcd::set_text(2, "Y: " + std::to_string(sOdom->currentPos.y.load()));
        // pros::lcd::set_text(3, "Inertial: " + std::to_string(sOdom->currentPos.h.load()));
		
		// pros::lcd::set_text(1, "Right: " + std::to_string(Robot::getSensor<sensors::RightEncoder>().get_value()));
		// pros::lcd::set_text(2, "Left: " + std::to_string(Robot::getSensor<sensors::LeftEncoder>().get_value()));
		// pros::lcd::set_text(3, "Avg: " + std::to_string((sRobot->getEncoder("Left")->get_value() + sRobot->getEncoder("Right")->get_value()) / 200.0));
		// pros::lcd::set_text(3, "Inertial: " + std::to_string(Robot::getSensor<sensors::Inertial>().get_rotation()));

		
		// pros::lcd::set_text(2, std::to_string(pros::c::adi_encoder_get(encoder)));
		
		// pros::lcd::set_text(4, "Lift: " + std::to_string(Robot::getSensor<sensors::LiftPotentiometer>().get_value()));
		// pros::lcd::set_text(5, "FrontDist: " + std::to_string(Robot::getSensor<sensors::FrontDistance>().get()) + "   BackDist: " + std::to_string(Robot::getSensor<sensors::BackDistance>().get()));
		// pros::lcd::set_text(6, "roll: " + std::to_string(Robot::getSensor<sensors::Inertial>().get_roll()));

		// printf("\nbackleft: %d", (int)sRobot->getMotor("BackLeft")->get_temperature());
		// printf("\nbackright: %d", (int)sRobot->getMotor("BackRight")->get_temperature());
		// printf("\nfrontleft: %d", (int)sRobot->getMotor("FrontLeft")->get_temperature());
		// printf("\nfrontright: %d", (int)sRobot->getMotor("FrontRight")->get_temperature());


		sController->act();
		pros::delay(20);
	}
}