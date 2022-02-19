#include "includes.hpp"

namespace robotFuncs {
    liftStates liftState = liftStates::down;
    bool liftRunning = true;
    double manualLiftHoldVal = 0;
    int manualn = 0;

    bool conveyorRunning = true;
    double conveyorDirection = 1;

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
        double kP = 0.5;
        double kI = 0.001;
        double kD = 0.4;
        double P = 0, I = 0, D = 0;
        double err, prevErr;

        int n = 0;

        while(true) {
            if (pros::Task::notify_take(true, 20)) {
                liftRunning = !liftRunning;
            }

            if (liftRunning) {
                err = (double)liftState - sRobot->getMotor("Lift")->get_position();

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
            Thread::notifyTask("lift");
        }
        if (!liftRunning) { 
            manualn = 0;
            if (sController->getDigital(pros::E_CONTROLLER_DIGITAL_R1)) {
                *(sRobot->getMotor("Lift")) = 127;
            }
            else if (sController->getDigital(pros::E_CONTROLLER_DIGITAL_R2)) {
                *(sRobot->getMotor("Lift")) = -127;
            }
        }
    }

    void manualholdLift() {
        double kP = 0.3;
        double kI = 0.001;
        double kD = 0.1;
        double P = 0, I = 0, D = 0;
        double err, prevErr;
        int n = 0;

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

    void conveyorIn() {
        if (sRobot->getPotentiometer("Lift")->get_value() > 950) {
            if (conveyorDirection == 1) {
                conveyorRunning = 1 - conveyorRunning;
            }
            else {
                conveyorDirection = 1;
                conveyorRunning = true;
            }
        }
    }

    void conveyorOut() {
        if (sRobot->getPotentiometer("Lift")->get_value() > 950) {
            if (conveyorDirection == -1.5) {
                conveyorRunning = 1 - conveyorRunning;
            }
            else {
                conveyorDirection = -1.5;
                conveyorRunning = true;
            }
        }
    }

    void conveyor(void* params) {
        while (true) {
            if (sRobot->getPotentiometer("Lift")->get_value() > 950 && conveyorRunning) {
                *(sRobot->getMotor("Conveyor")) = 70 * conveyorDirection;
            }
            else {
                *(sRobot->getMotor("Conveyor")) = 0;
            }
            
            pros::delay(20);
        }
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

    void auton_mogo(int ultrasonicLimit, double yLimit) {
        while ((sOdom->currentPos.y.load() < 400 || sRobot->getUltrasonic("Front")->get_value() > ultrasonicLimit) && sOdom->currentPos.y.load() < yLimit) {
            double turnErr = sRobot->getInertial("Inertial")->get_rotation();
            // double speedSum = 200 + (1.5 * turnErr);
            // double moveSpeed = 200 * (200 / speedSum);
            // double turnSpeed = (turnErr * 1.5) * (200 / speedSum);
            // sRobot->getMotorGroup("LeftDrive")->moveVelocity(moveSpeed + turnSpeed);
            // sRobot->getMotorGroup("RightDrive")->moveVelocity(moveSpeed - turnSpeed);

            double moveSpeed = 127;
            if (sOdom->currentPos.y.load() > 1450) {
                moveSpeed -= 0.027 * sOdom->currentPos.y.load();
            }
            double turnSpeed = turnErr * 1.5;
            double speedSum = moveSpeed + fabs((turnSpeed));
            double maxMoveSpeed = moveSpeed * (127 / speedSum);
            double maxTurnSpeed = turnSpeed * (127 / speedSum);
            if (moveSpeed > maxMoveSpeed) {
                moveSpeed = maxMoveSpeed;
            }
            if (fabs(turnSpeed) > fabs(maxTurnSpeed)) {
                turnSpeed = maxTurnSpeed;
            }
            sRobot->arcade(moveSpeed, -turnSpeed);
            printf("\ncurrentPos = (%d, %d, %d), ultrasonic: %d, moveSpeed = %d, turnSpeed = %d, speedSum = %d", (int)sOdom->currentPos.x, (int)sOdom->currentPos.y, (int)sOdom->currentPos.h, sRobot->getUltrasonic("Front")->get_value(), (int)moveSpeed, (int)turnSpeed, (int)speedSum);
            pros::delay(40);
        }
    }

    void auton_clamp(int time) {
        
    }

    void auton_declamp(int time) {
        
    }

    void autonLift(liftStates setLiftState) {
        liftState = setLiftState;
    }

    void auton_conveyor(int time) {
        *(sRobot->getMotor("Conveyor")) = 70;
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
            // sController->master.print(1, 0, ("ultrasonic: " + std::to_string(sRobot->getUltrasonic("Front")->get_value())).c_str());
            pros::delay(50);
        }
    }
}