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
        sRobot->getInertial("Inertial")->tare_rotation();
        Thread::startTask("move", Odometry::moveTo);
        pros::delay(100);

        sOdom->waitUntilStop();
        Thread::notifyTask("move");
        pros::delay(20);
        Thread::killTask("move");
        sRobot->stopDrive();
        // Thread::resumeTask("drive");
    }

    void liftPID(void* params) {
        double kP = 0.5;
        double kI = 0.003;
        double kD = 0.75;
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

                if (fabs(err) > 10) {
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
            else if (sController->getDigital(pros::E_CONTROLLER_DIGITAL_R2) && sRobot->getPotentiometer("Lift")->get_value() > 900) {
                *(sRobot->getMotor("Lift")) = sRobot->getPotentiometer("Lift")->get_value() > 1200 ? -127 : -60;
            }
        }
    }

    void manualholdLift() {
        double kP = 0.5;
        double kI = 0.001;
        double kD = 0.1;
        double P = 0, I = 0, D = 0;
        double err, prevErr;
        int n = 0;

        if (!liftRunning) {
            if (!sController->getDigital(pros::E_CONTROLLER_DIGITAL_R1) && !sController->getDigital(pros::E_CONTROLLER_DIGITAL_R2)) {
                manualn++;
                if (manualn == 40) {
                    manualLiftHoldVal = sRobot->getPotentiometer("Lift")->get_value() > 900 ? sRobot->getPotentiometer("Lift")->get_value() : 900;
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
        if (conveyorDirection == 1) {
            conveyorRunning = 1 - conveyorRunning;
        }
        else {
            conveyorDirection = 1;
            conveyorRunning = true;
        }
    }

    void conveyorOut() {
        if (conveyorDirection == -1) {
            conveyorRunning = 1 - conveyorRunning;
        }
        else {
            conveyorDirection = -1;
            conveyorRunning = true;
        }
    }

    void conveyor(void* params) {
        while (true) {
            if (sRobot->getPotentiometer("Lift")->get_value() > 1069 && conveyorRunning) {
                *(sRobot->getMotor("Conveyor")) = 127 * conveyorDirection;
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

    void autonLift(liftStates setLiftState) {
        liftState = setLiftState;
    }

    void auton_conveyor(int time) {
        *(sRobot->getMotor("Conveyor")) = 70;
        pros::delay(time);
        *(sRobot->getMotor("Conveyor")) = 0;
    }

    void distanceBack(double distance) {
        double mkP = 0.3;
        double mkI = 0.000001;
        double mkD = 0.7;
        double mP = 0, mI = 0, mD = 0;
        double dist, prevDist;
        double moveSpeed;

        double tkP = 4.0;
        double tkI = 0.01;
        double tkD = 2.0;
        double tP = 0, tI = 0, tD = 0;
        double turnErr, prevTurnErr;
        double turnSpeed;

        Thread::pauseTask("move");
        sRobot->arcade(-97, -30);
        pros::delay(250);
        while (sRobot->getInertial("Inertial")->get_roll() > 2.5) {
            moveSpeed = -127;
            turnSpeed = -4.0 * (sRobot->getInertial("Inertial")->get_rotation());

            double speedSum = fabs(moveSpeed) + fabs(turnSpeed);
            double maxMoveSpeed = moveSpeed * (127 / speedSum);
            double maxTurnSpeed = turnSpeed * (127 / speedSum);
            if (fabs(moveSpeed) > fabs(maxMoveSpeed)) {
                moveSpeed = maxMoveSpeed;
            }
            if (fabs(turnSpeed) > fabs(maxTurnSpeed)) {
                turnSpeed = maxTurnSpeed;
            }

            sRobot->arcade(moveSpeed, turnSpeed);
            printf("\nturn = %d", (int)sRobot->getInertial("Inertial")->get_rotation());
            pros::delay(20);
        }
        sRobot->stopDrive();
        pros::delay(500);
        dist = sRobot->getDistance("Back")->get() - distance;
        prevDist = dist;
        turnErr = -1 * (sRobot->getInertial("Inertial")->get_rotation());
        prevTurnErr = turnErr;
        while (fabs(dist) > 15 || fabs(turnErr) > 2) {
            dist = sRobot->getDistance("Back")->get() - distance;
            turnErr = -1 * (sRobot->getInertial("Inertial")->get_rotation());

            mP = dist;
            mI += dist;
            mD = dist - prevDist;
            prevDist = dist;

            moveSpeed = (mkP * mP) + (mkI * mI) + (mkD * mD);

            tP = turnErr;
            tI += turnErr;
            tD = turnErr - prevTurnErr;
            prevTurnErr = turnErr;

            turnSpeed = (tkP * tP) + (tkI * tI) + (tkD * tD);

            double speedSum = fabs(moveSpeed) + fabs(turnSpeed);
            double maxMoveSpeed = moveSpeed * (127 / speedSum);
            double maxTurnSpeed = turnSpeed * (127 / speedSum);
            if (fabs(moveSpeed) > fabs(maxMoveSpeed)) {
                moveSpeed = maxMoveSpeed;
            }
            if (fabs(turnSpeed) > fabs(maxTurnSpeed)) {
                turnSpeed = maxTurnSpeed;
            }

            if (dist < 0) {
                sRobot->arcade(-70, turnSpeed);
            }   
            else {
                sRobot->arcade(-moveSpeed, turnSpeed);
            }

            printf("\ndist = %d, moveSpeed = %d, turnErr = %d, turnSpeed = %d", (int)dist, (int)moveSpeed, (int)turnErr, (int)turnSpeed);
            pros::delay(20);
        }
        printf("\nfinished");
        sRobot->stopDrive();
        pros::delay(500);
        Thread::notifyTask("fps");
        pros::delay(500);
        Thread::resumeTask("move");
    }

    void controllerPrint(void* params) {
        while(true) {
            sController->master.print(0, 12, "bat: %g %", pros::battery::get_capacity());
            pros::delay(50);
            if (Thread::existsTask("fps")) {
                sController->master.print(0, 0, ("X: " + std::to_string(sOdom->currentPos.x.load())).c_str());
                pros::delay(50);
                sController->master.print(1, 0, ("Y: " + std::to_string(sOdom->currentPos.y.load())).c_str());
                pros::delay(50);
                sController->master.print(2, 0, ("H: " + std::to_string(sOdom->currentPos.h.load())).c_str());
                pros::delay(50);
            }
        }
    }
}