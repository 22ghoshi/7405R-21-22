#include "includes.hpp"

//vector of autons
std::vector<std::string> autonNames{"leftwp", "leftneutral", "middle", "right", "rightneutral", "rightwp", "skills", "test"};
autonSelect autonselect = autonSelect::leftneutral;

void on_center_button() {
	
}

void on_left_button() {
    if ((int)autonselect == 0) {
		autonselect = (autonSelect)(autonNames.size() - 1);
	}
	else {
		autonselect = (autonSelect)((int)autonselect - 1);
	}
	
	pros::lcd::set_text(7, autonNames[(int)autonselect]);
}

void on_right_button() {
    if ((int)autonselect == (autonNames.size() - 1)) {
		autonselect = (autonSelect)0;
	}
	else {
		autonselect = (autonSelect)((int)autonselect + 1);
	}

	pros::lcd::set_text(7, autonNames[(int)autonselect]);
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
    pros::delay(100);
	pros::lcd::set_text(7, autonNames[(int)autonselect]);
	pros::lcd::register_btn1_cb(on_center_button);
    pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::register_btn2_cb(on_right_button);

	Robot::Instance();

	Odometry::Instance();
	Thread::startTask("fps", Odometry::FPS);

	sController->master.clear();
	Thread::startTask("print", robotFuncs::controllerPrint);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}