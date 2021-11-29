#include "includes.hpp"

namespace robotFuncs {
    enum class mLiftStates {down = 1010, low = 1150, mid = 2216, high = 2626};
    mLiftStates mLiftState = mLiftStates::low;
    bool pLiftState = false;
    bool conveyorState = false;
    int conveyorDirection = 1;
    
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
            int power = lefty > 0 ? ((int)(127.0 * std::pow((double)lefty / 127, (double)13 / 9))) : -1 * ((int)(127.0 * std::pow((double)(-lefty) / 127, (double)13 / 9)));
            int turn = rightx > 0 ? ((int)(127.0 * std::pow((double)rightx / 127, (double)13 / 9))) : -1 * ((int)(127.0 * std::pow((double)(-rightx) / 127, (double)13 / 9)));
            sRobot->arcade(power, turn);
            // sRobot->arcade(lefty, rightx);
            pros::delay(20);
        }
    }

    void nothing() {
        //
    }

    void testmove() {
        Thread::pauseTask("drive");
        Thread::startTask("move", Odometry::moveTo);
        pros::delay(100);
        sOdom->setTarget(300, -300, -45, {0.3, 0.001, 0.13}, 20, {3.0, 0.001, 0.5}, 5.0);
        // sOdom->setTargetTurn(90, {1.5, 0.0, 0.1}, 1.0);
        // sOdom->setTarget(0, 0, 0);
        sOdom->waitUntilStop();
        sRobot->stopDrive();
        Thread::killTask("move");
        Thread::resumeTask("drive");
    }

    void mLiftPID(void* params) {
        double kP = 0.2;
        double kI = 0.0001;
        double kD = 0.03;
        double P = 0, I = 0, D = 0;
        double err, prevErr;

        int n = 0;

        while(true) {
            err = (double)mLiftState - sRobot->getPotentiometer("mLift")->get_value();

            n += 1;
            if (n == 1) {
                prevErr = err;
            }

            if (fabs(err) > 10) {
                P = err;
                I += err;
                D = err - prevErr;

                double liftSpeed = (kP * P) + (kI * I) + (kD * D);
                
                *(sRobot->getMotorGroup("mLift")) = liftSpeed;
            }
            else {
                n = 0;
                I = 0;
                sRobot->getMotorGroup("mLift")->stop(brakeType::hold);
            }
            pros::delay(20);
        }
    }

    void move_mLift() {
        if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_B)) {
            mLiftState = mLiftStates::down;
        } 
        else if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_Y)) {
            mLiftState = mLiftStates::low;
        }
        else if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_X)) {
            mLiftState = mLiftStates::mid;
        }
        else if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_A)) {
            mLiftState = mLiftStates::high;
        }
    }

    void toggle_pLift() {
        pLiftState = 1 - pLiftState;
        sRobot->getPiston("Right_pLift")->set_value(pLiftState);
        sRobot->getPiston("Left_pLift")->set_value(pLiftState);
    }

    void toggleConveyor() {
        if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_L1)) {
            if (conveyorDirection == 1) {
                conveyorState = 1 - conveyorState;
            }
            else {
                conveyorDirection = 1;
                conveyorState = true;
            }
        }
        else if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_L2)) {
            if (conveyorDirection == -1) {
                conveyorState = 1 - conveyorState;
            }
            else {
                conveyorDirection = -1;
                conveyorState = true;
            }
        }

        if (conveyorState) {
            *(sRobot->getMotor("Conveyor")) = 127 * conveyorDirection;
        }
        else {
            *(sRobot->getMotor("Conveyor")) = 0;
        }
    }

    void clamp() {
        if (sController->getDigital(pros::E_CONTROLLER_DIGITAL_R1)) {
            *(sRobot->getMotor("Clamp")) = 127;
        }
        else if (sController->getDigital(pros::E_CONTROLLER_DIGITAL_R2)) {
            *(sRobot->getMotor("Clamp")) = -127;
        }
    }

    void holdClamp() {
        if (!(sController->getDigital(pros::E_CONTROLLER_DIGITAL_R1) || sController->getDigital(pros::E_CONTROLLER_DIGITAL_R2))) {
            *(sRobot->getMotor("Clamp")) = 0;
            sRobot->getMotor("Clamp")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        }
    }
}