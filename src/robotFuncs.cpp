#include "includes.hpp"

namespace robotFuncs {
    liftStates liftState = liftStates::down;
    bool liftRunning = true;
    bool conveyorRunning = false;
    bool manualConveyorStop = false;
    int conveyorDirection = 1;
    double manualLiftHoldVal = 0;
    int manualn = 0;
    bool frontClampState = false;
    bool backClampState =  false;
    bool tilterState = false;
    
    void drive(void* params) {
        while (true) {
            int leftx = sController->getAnalog(pros::E_CONTROLLER_ANALOG_LEFT_X);
            int lefty = sController->getAnalog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int rightx = sController->getAnalog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
            int righty = sController->getAnalog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
            int power = lefty >= 0 ? ((int)(127.0 * std::pow((double)lefty / 127.0, 15.0 / 9.0))) : -1 * ((int)(127.0 * std::pow((double)(-lefty) / 127.0, 15.0 / 9.0)));
            int turn = rightx >= 0 ? ((int)(127.0 * std::pow((double)rightx / 127.0, 21.0 / 9.0))) : -1 * ((int)(127.0 * std::pow((double)(-rightx) / 127.0, 21.0 / 9.0)));
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
        
        sOdom->waitUntilStop();
        sRobot->stopDrive();
        Thread::killTask("move");
        Thread::resumeTask("drive");
    }

    void liftPID(void* params) {
        double kP = 0.7;
        double kI = 0.00000;
        double kD = 0.00;
        double P = 0, I = 0, D = 0;
        double err, prevErr;

        int n = 0;

        while(true) {
            if (pros::Task::notify_take(true, 20)) {
                liftRunning = !liftRunning;
            }

            if (liftRunning) {
                err = (double)liftState - sRobot->getPotentiometer("Lift")->get_value();

                n += 1;
                if (n == 1) {
                    prevErr = err;
                }

                if (fabs(err) > 25) {
                    P = err;
                    I += err;
                    D = err - prevErr;
                    prevErr = err;

                    double liftSpeed = (kP * P) + (kI * I) + (kD * D);
                    
                    *(sRobot->getMotor("Lift")) = liftSpeed;
                }
                else {
                    n = 0;
                    I = 0;
                    sRobot->getMotor("Lift")->move_velocity(0);
                    sRobot->getMotor("Lift")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                }
            }
        }
    }

    void moveLift() {
        if (!liftRunning) {
            Thread::notifyTask("lift");
        }
        if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_B)) {
            if (conveyorRunning) {
                conveyorRunning = false;
                *(sRobot->getMotor("Conveyor")) = 0;
            }
            liftState = liftStates::down;
        } 
        else if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_Y)) {
            if (conveyorRunning) {
                conveyorRunning = false;
                *(sRobot->getMotor("Conveyor")) = 0;
            }
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
            Thread::notifyTask("lift");
        }
        if (!liftRunning) { 
            manualn = 0;
            if (sController->getDigital(pros::E_CONTROLLER_DIGITAL_R1)) {
                if (sRobot->getPotentiometer("Lift")->get_value() > 950 && !conveyorRunning && !manualConveyorStop) {
                    conveyorRunning = true;
                    *(sRobot->getMotor("Conveyor")) = 60 * conveyorDirection;
                }
                *(sRobot->getMotor("Lift")) = 127;
            }
            else if (sController->getDigital(pros::E_CONTROLLER_DIGITAL_R2)) {
                if (sRobot->getPotentiometer("Lift")->get_value() < 950 && conveyorRunning) {
                    conveyorRunning = false;
                    *(sRobot->getMotor("Conveyor")) = 0;
                    manualConveyorStop = false;
                }
                *(sRobot->getMotor("Lift")) = -127;
            }
        }
    }

    void manualholdLift() {
        double kP = 0.7;
        double kI = 0.000;
        double kD = 0.00;
        double P = 0, I = 0, D = 0;
        double err, prevErr;
        int n = 0;

        if (sRobot->getPotentiometer("Lift")->get_value() < 950 && conveyorRunning) {
            conveyorRunning = false;
            manualConveyorStop = false;
            *(sRobot->getMotor("Conveyor")) = 0;
        }
        if (sRobot->getPotentiometer("Lift")->get_value() > 950 && !conveyorRunning && !manualConveyorStop) {
            conveyorRunning = true;
            *(sRobot->getMotor("Conveyor")) = 60 * conveyorDirection;
        }

        if (!liftRunning) {
            if (!sController->getDigital(pros::E_CONTROLLER_DIGITAL_R1) && !sController->getDigital(pros::E_CONTROLLER_DIGITAL_R2)) {
                manualn++;
                if (manualn == 40) {
                    manualLiftHoldVal = sRobot->getPotentiometer("Lift")->get_value();
                }

                err = manualLiftHoldVal - sRobot->getPotentiometer("Lift")->get_value();

                n += 1;
                if (n == 1) {
                    prevErr = err;
                }
                
                if (fabs(err) > 10  && manualn >= 40) {
                    P = err;
                    I += err;
                    D = err - prevErr;
                    prevErr = err;

                    double liftSpeed = (kP * P) + (kI * I) + (kD * D);
                    
                    *(sRobot->getMotor("Lift")) = liftSpeed;
                }
                else {
                    n = 0;
                    I = 0;
                    sRobot->getMotor("Lift")->move_velocity(0);
                    sRobot->getMotor("Lift")->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                }
            }
        }
    }

    void toggleConveyor() {
        if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_UP)) {
            if (conveyorDirection == 1) {
                conveyorRunning = 1 - conveyorRunning;
                if (!conveyorRunning) {
                    manualConveyorStop = true;
                }
                else {
                    manualConveyorStop = false;
                }
            }
            else {
                conveyorDirection = 1;
                conveyorRunning = true;
            }
        }
        else if (sController->getDigitalNewPress(pros::E_CONTROLLER_DIGITAL_DOWN)) {
            if (conveyorDirection == -2) {
                conveyorRunning = 1 - conveyorRunning;
                if (!conveyorRunning) {
                    manualConveyorStop = true;
                }
                else {
                    manualConveyorStop = false;
                }
            }
            else {
                conveyorDirection = -2;
                conveyorRunning = true;
            }
        }

        if (conveyorRunning) {
            *(sRobot->getMotor("Conveyor")) = 60 * conveyorDirection;
        }
        else {
            *(sRobot->getMotor("Conveyor")) = 0;
        }

        pros::lcd::set_text(6, "conveyorRunning: " + std::to_string(conveyorRunning));
        pros::lcd::set_text(7, "conveyorDirection: " + std::to_string(conveyorDirection));
    }

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

    void auton_conveyor(int time) {
        *(sRobot->getMotor("Conveyor")) = 90;
        pros::delay(time);
        *(sRobot->getMotor("Conveyor")) = 0;
    }

    void controllerPrint(void* params) {
        while(true) {
            sController->master.print(0, 14, "bat: %g %", pros::battery::get_capacity());
            pros::delay(50);
            sController->master.print(0, 0, ("X: " + std::to_string(sOdom->currentPos.x.load())).c_str());
            pros::delay(50);
            sController->master.print(1, 0, ("Y: " + std::to_string(sOdom->currentPos.y.load())).c_str());
            pros::delay(50);
            sController->master.print(2, 0, ("H: " + std::to_string(sOdom->currentPos.h.load())).c_str());
            pros::delay(50);
        }
    }
}