#include "Controller.hpp"

Controller* Controller::pInstance = NULL;

Controller::Controller() : master(pros::E_CONTROLLER_MASTER) {};

Controller* Controller::Instance() {
    if(!pInstance) {
        pInstance = new Controller();
    }
    return pInstance;
}

void Controller::update(void* params) {
    while(true) {
        for (int i = pros::E_CONTROLLER_ANALOG_LEFT_X; i <= pros::E_CONTROLLER_ANALOG_RIGHT_Y; i++) {
            sController->analogValues[(pros::controller_analog_e_t)i] = sController->master.get_analog((pros::controller_analog_e_t)i);
        }

        for (int i = pros::E_CONTROLLER_DIGITAL_L1; i <= pros::E_CONTROLLER_DIGITAL_A; i++) {
            sController->buttonStatus[(pros::controller_digital_e_t)i] = sController->master.get_digital((pros::controller_digital_e_t)i);
        }

        for (int i = pros::E_CONTROLLER_DIGITAL_L1; i <= pros::E_CONTROLLER_DIGITAL_A; i++) {
            sController->buttonNewPressStatus[(pros::controller_digital_e_t)i] = sController->master.get_digital_new_press((pros::controller_digital_e_t)i);
        } 

        pros::delay(20);
    }
}

void Controller::act() {
    for (auto const& [button, status] : buttonNewPressStatus) {
        if(status) {
            if(buttonNewPressActions.find(button) != buttonNewPressActions.end()) {
                buttonNewPressActions.at(button)();
            }
        }
    }
    
    for (auto const& [button, status] : buttonStatus) {
        if(status) {
            if(buttonActions.find(button) != buttonActions.end()) {
                buttonActions.at(button)();
            }
        } else if (!status) {
            if(buttonDefaults.find(button) != buttonDefaults.end()) {
                buttonDefaults.at(button)();
            }
        }
    }
}

void Controller::registerButton(pros::controller_digital_e_t button, std::function<void(void)> action, std::function<void(void)> defaultAction) {
    buttonActions[button] = action;
    buttonDefaults[button] = defaultAction;
}

void Controller::registerButtonNewPress(pros::controller_digital_e_t button, std::function<void(void)> action) {
    buttonNewPressActions[button] = action;
}

std::int32_t Controller::getAnalog(pros::controller_analog_e_t stick) {
    return analogValues[stick];
}

bool Controller::getDigital(pros::controller_digital_e_t button) {
    return buttonStatus[button];
}

bool Controller::getDigitalNewPress(pros::controller_digital_e_t button) {
    return buttonNewPressStatus[button];
}