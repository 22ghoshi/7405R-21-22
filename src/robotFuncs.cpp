#include "includes.hpp"

namespace robotFuncs {
    liftStates liftState = liftStates::low;
    bool liftRunning = true;
    // bool conveyorState = false;
    // int conveyorDirection = 1;
    double manualLiftHoldVal = 0;
    int manualn = 0;
    bool frontClampState = false;
    bool backClampState = false;
    bool tilterState = false;
    
    void drive(void* params) {
        while (true) {
            int leftx = sController->getAnalog(pros::E_CONTROLLER_ANALOG_LEFT_X);
            int lefty = sController->getAnalog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int rightx = sController->getAnalog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
            int righty = sController->getAnalog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
            int power = lefty >= 0 ? ((int)(127.0 * std::pow((double)lefty / 127.0, 13.0 / 9.0))) : -1 * ((int)(127.0 * std::pow((double)(-lefty) / 127.0, 13.0 / 9.0)));
            int turn = rightx >= 0 ? ((int)(127.0 * std::pow((double)rightx / 127.0, 13.0 / 9.0))) : -1 * ((int)(127.0 * std::pow((double)(-rightx) / 127.0, 13.0 / 9.0)));
            sRobot->arcade(power, turn);
            // sRobot->arcade(lefty, rightx);
            pros::delay(20);
        }
    }

    void nothing() {
        //
    }

    // void testmove() {
    //     Thread::pauseTask("drive");
    //     Thread::startTask("move", Odometry::moveTo);
    //     pros::delay(100);
    //     sOdom->setTarget(300, -300, -45, {0.3, 0.001, 0.13}, 20, {3.0, 0.001, 0.5}, 5.0);
    //     // sOdom->setTargetTurn(90, {1.5, 0.0, 0.1}, 1.0);
    //     // sOdom->setTarget(0, 0, 0);
    //     sOdom->waitUntilStop();
    //     sRobot->stopDrive();
    //     Thread::killTask("move");
    //     Thread::resumeTask("drive");
    // }

    void liftPID(void* params) {
        double kP = 0.2;
        double kI = 0.0001;
        double kD = 0.03;
        double P = 0, I = 0, D = 0;
        double err, prevErr;

        int n = 0;

        while(true) {
            err = (double)liftState - sRobot->getPotentiometer("Lift")->get_value();

            n += 1;
            if (n == 1) {
                prevErr = err;
            }

            if (fabs(err) > 10) {
                P = err;
                I += err;
                D = err - prevErr;

                double liftSpeed = (kP * P) + (kI * I) + (kD * D);
                
                *(sRobot->getMotor("Lift")) = liftSpeed;
            }
            else {
                n = 0;
                I = 0;
                *(sRobot->getMotor("Lift")) = 0;
                sRobot->getMotor("Lift")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            }
            pros::delay(20);
        }
    }

    void moveLift() {
        if (!liftRunning) {
            Thread::resumeTask("lift");
            liftRunning = true;
        }
        if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_B)) {
            liftState = liftStates::down;
        } 
        else if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_Y)) {
            liftState = liftStates::low;
        }
        else if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_X)) {
            liftState = liftStates::mid;
        }
        else if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_A)) {
            liftState = liftStates::high;
        }
    }

    void manualmoveLift() {
        if (liftRunning) {
            Thread::pauseTask("lift");
            liftRunning = false;
        }
        if (!liftRunning) { 
            manualn = 0;
            if (sController->getDigital(pros::E_CONTROLLER_DIGITAL_L1)) {
                *(sRobot->getMotor("Lift")) = 127;
            }
            else if (sController->getDigital(pros::E_CONTROLLER_DIGITAL_L2)) {
                *(sRobot->getMotor("Lift")) = -127;
            }
        }
    }

    void manualholdLift() {
        double kP = 0.2;
        double kI = 0.0001;
        double kD = 0.03;
        double P = 0, I = 0, D = 0;
        double err, prevErr;
        int n = 0;

        if (!liftRunning) {
            if (!sController->getDigital(pros::E_CONTROLLER_DIGITAL_L1) && !sController->getDigital(pros::E_CONTROLLER_DIGITAL_L2)) {
                // manualn++;
                // if (manualn == 1) {
                //     manualLiftHoldVal = sRobot->getPotentiometer("mLift")->get_value();
                // }

                // err = manualLiftHoldVal - sRobot->getPotentiometer("mLift")->get_value();

                // n += 1;
                // if (n == 1) {
                //     prevErr = err;
                // }
                
                // if (fabs(err) > 10) {
                //     P = err;
                //     I += err;
                //     D = err - prevErr;

                //     double liftSpeed = (kP * P) + (kI * I) + (kD * D);
                    
                //     *(sRobot->getMotorGroup("mLift")) = liftSpeed;
                // }
                // else {
                //     n = 0;
                //     I = 0;
                //     sRobot->getMotorGroup("mLift")->stop(brakeType::hold);
                // }
                *(sRobot->getMotor("Lift")) = 0;
                sRobot->getMotor("Lift")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            }
        }
    }

    // void toggle_pLift() {
    //     pLiftState = 1 - pLiftState;
    //     sRobot->getPiston("Right_pLift")->set_value(pLiftState);
    //     sRobot->getPiston("Left_pLift")->set_value(pLiftState);
    // }

    // void toggleConveyor() {
    //     if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_UP)) {
    //         if (conveyorDirection == 1) {
    //             conveyorState = 1 - conveyorState;
    //         }
    //         else {
    //             conveyorDirection = 1;
    //             conveyorState = true;
    //         }
    //     }
    //     else if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
    //         if (conveyorDirection == -1) {
    //             conveyorState = 1 - conveyorState;
    //         }
    //         else {
    //             conveyorDirection = -1;
    //             conveyorState = true;
    //         }
    //     }

    //     if (conveyorState) {
    //         *(sRobot->getMotor("Conveyor")) = 127 * conveyorDirection;
    //     }
    //     else {
    //         *(sRobot->getMotor("Conveyor")) = 0;
    //     }
    // }

    void toggleFrontClamp() {
        frontClampState = 1 - frontClampState;
        sRobot->getPiston("FrontClamp")->set_value(frontClampState);
    }

    void toggleBackClamp() {
        backClampState = 1 - backClampState;
        sRobot->getPiston("BackClamp")->set_value(backClampState);
    }

    void toggleTilter() {
        tilterState = 1 - tilterState;
        sRobot->getPiston("Tilter")->set_value(tilterState);
    }

    void auton_clamp(int time) {
        
    }

    void auton_declamp(int time) {
        
    }

    void autonLift(liftStates setLiftState) {
        liftState = setLiftState;
    }

    // void auton_conveyor(int time) {
    //     *(sRobot->getMotor("Conveyor")) = 127;
    //     pros::delay(time);
    //     *(sRobot->getMotor("Conveyor")) = 0;
    // }
}