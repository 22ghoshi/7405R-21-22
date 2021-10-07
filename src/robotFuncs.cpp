#include "includes.hpp"

namespace robotFuncs {
    void drive(void* params) {
        while (true) {
            // int power = (int)(127.0 * std::pow((double)lefty / 127, (double)13 / 9));
            // int strafe = (int)(127.0 * std::pow((double)leftx / 127, (double)13 / 9));
            // int turn = (int)(127.0 * std::pow((double)rightx / 127, (double)13 / 9));
            // if (lefty < 0) {
            // 	power = -1 * ((int) std::round(127.0 * std::pow((double) (-lefty) / 127, (double) 13 / 9)));
            // }
            // if (leftx < 0) {
            // 	strafe = -1 * (int) std::round(127.0 * std::pow((double) (-leftx) / 127, (double) 13 / 9));
            // }
            // if(rightx < 0) {
            // 	turn = -1 * ((int) std::round(127.0 * std::pow((double) (-rightx) / 127, (double) 13 / 9)));
            // }
            int leftx = sController->getAnalog(pros::E_CONTROLLER_ANALOG_LEFT_X);
            int lefty = sController->getAnalog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int rightx = sController->getAnalog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
            int righty = sController->getAnalog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
            sRobot->tank(lefty, righty);
            pros::delay(20);
        }
    }
}