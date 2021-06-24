#pragma once
#include "main.h"

#include <map>
#include <memory>
#include <string>
#include <functional>

#define sController Controller::Instance()

class Controller {
    private:
    Controller();


    std::map<pros::controller_digital_e_t, std::function<void(void)>> buttonActions;
    std::map<pros::controller_digital_e_t, std::function<void(void)>> buttonDefaults;
    std::map<pros::controller_digital_e_t, std::function<void(void)>> buttonNewPressActions;

    static Controller* pInstance;

    public:
    static Controller* Instance();

    pros::Controller master;

    std::map<pros::controller_analog_e_t, std::int32_t> analogValues;
    std::map<pros::controller_digital_e_t, bool> buttonStatus;
    std::map<pros::controller_digital_e_t, bool> buttonNewPressStatus;

    static void update(void* params);
    void act();
    void registerButton(pros::controller_digital_e_t button, std::function<void(void)> action, std::function<void(void)> defaultAction);
    void registerButtonNewPress(pros::controller_digital_e_t button, std::function<void(void)> action);

    std::int32_t getAnalog(pros::controller_analog_e_t stick);
    bool getDigital(pros::controller_digital_e_t button);
};