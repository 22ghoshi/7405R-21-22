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
        robotFuncs::toggleTilter();
        robotFuncs::toggleFrontClamp();

        //align with red mogo
        sOdom->setTarget(0, -200, {0.26, 0.00, 0.22}, 15);
        sOdom->setTarget(54, {1.4, 0.0, 1.6}, 1.5);

        // pick up red mogo
        sOdom->setTarget(-575, -615, {0.13, 0.001, 0.5}, 40, {3.0, 0.01, 1.0}, 6);
        sOdom->waitUntilStop();
        robotFuncs::toggleBackClamp();
        pros::delay(500);
        robotFuncs::toggleTilter();
        robotFuncs::autonLift(liftStates::lowmid);
        pros::delay(500);

        //go to middle mogo
        sOdom->setTargetNow(160, {1.5, 0.0, 2.25}, 1.5);
        sOdom->setTarget(0, -1800, {0.1, 0.001, 0.3}, 70, {3.0, 0.01, 1.0}, 7);
        while(sRobot->getUltrasonic("Front")->get_value() > 70) {
            if (sOdom->currentPos.y < -1425) {
                robotFuncs::autonLift(liftStates::down);
            }
            pros::delay(2);
        }
        robotFuncs::toggleFrontClamp();

        //score neutral
        // sOdom->setTarget(150);
        sOdom->setTargetNow(1285, -4000, {0.1, 0.0001, 1.6}, 100, {5.0, 0.01, 2.0}, 15);
        robotFuncs::autonLift(liftStates::high);
        while (sOdom->currentPos.y > -3600) {
            pros::delay(2);
        }
        sOdom->waitUntilStop();
        pros::delay(500);
        robotFuncs::autonLift(liftStates::mid);
        pros::delay(750);
        robotFuncs::toggleFrontClamp();
        robotFuncs::autonLift(liftStates::high);
        pros::delay(250);

        //back up and drop red
        sOdom->setTarget(890, -3330, {0.2, 0.0005, 0.4}, 40, {4.0, 0.01, 2.0}, 5);
        pros::delay(500);
        robotFuncs::autonLift(liftStates::lowmid);
        sOdom->waitUntilStop();
        pros::delay(250);
        robotFuncs::toggleTilter();
        pros::delay(250);
        robotFuncs::toggleBackClamp();
        pros::delay(250);
        sOdom->setTarget(1060, -3620, {0.25, 0.001, 0.3}, 50, {4.0, 0.01, 2.0}, 5);
        robotFuncs::autonLift(liftStates::down);
        sOdom->setTarget(-26, {1.3, 0.001, 3.0}, 1.5);
        robotFuncs::autonLift(liftStates::low);
        sOdom->setTarget(950, -3300, {0.23, 0.0015, 0.3}, 30, {3.5, 0.01, 2.5}, 5);
        sOdom->waitUntilStop();
        robotFuncs::toggleFrontClamp();
        pros::delay(250);
        robotFuncs::autonLift(liftStates::high);

        //score red
        sOdom->setTarget(-192, {1.1, 0.0001, 4.7}, 1.5);
        sOdom->setTarget(1220, -4300, {0.15, 0.0001, 0.55}, 65, {4.0, 0.01, 2.0}, 15);
        sOdom->waitUntilStop();
        pros::delay(250);
        robotFuncs::autonLift(liftStates::mid);
        pros::delay(500);
        robotFuncs::toggleFrontClamp();
        robotFuncs::autonLift(liftStates::high);
        pros::delay(250);
        // pros::delay(500);
        // robotFuncs::autonLift(liftStates::down);
        // pros::delay(250);
        // robotFuncs::autonLift(liftStates::high);
        // pros::delay(400);
        // robotFuncs::autonLift(liftStates::down);

        // //back out and go to blue mogo
        sOdom->setTarget(1000, -3466, {0.1, 0.001, 0.6}, 50, {2.5, 0.001, 3.0}, 10);
        sOdom->setTarget(-127, {1.9, 0.001, 2.7}, 1.5);
        // robotFuncs::autonLift(liftStates::lowmid);
        // sOdom->setTarget(-20, -4500, {0.1, 0.0001, 0.4}, 40, {3.0, 0.001, 2.0}, 7);
        // sOdom->setTarget(-222, {1.5, 0.001, 2.3}, 1.5);
        // sOdom->setTarget(260, -4830, {0.16, 0.001, 0.45}, 40, {3.5, 0.01, 2.0}, 10);
        robotFuncs::autonLift(liftStates::low);
        sOdom->waitUntilStop();
        robotFuncs::toggleFrontClamp();

        sOdom->waitUntilStop();
        Thread::notifyTask("move");
        pros::delay(20);
        Thread::killTask("move");
        sRobot->stopDrive();
        // Thread::resumeTask("drive");
    }

    void liftPID(void* params) {
        double kP = 0.35;
        double kI = 0.008;
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
            else if (sController->getDigital(pros::E_CONTROLLER_DIGITAL_R2) && sRobot->getPotentiometer("Lift")->get_value() > 1080) {
                *(sRobot->getMotor("Lift")) = sRobot->getPotentiometer("Lift")->get_value() > 1300 ? -127 : -60;
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
                    manualLiftHoldVal = sRobot->getPotentiometer("Lift")->get_value() > 1050 ? sRobot->getPotentiometer("Lift")->get_value() : 1050;
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
        if (sRobot->getPotentiometer("Lift")->get_value() > 1230) {
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
        if (sRobot->getPotentiometer("Lift")->get_value() > 1230) {
            if (conveyorDirection == -1.2) {
                conveyorRunning = 1 - conveyorRunning;
            }
            else {
                conveyorDirection = -1.2;
                conveyorRunning = true;
            }
        }
    }

    void conveyor(void* params) {
        while (true) {
            if (sRobot->getPotentiometer("Lift")->get_value() > 1230 && conveyorRunning) {
                *(sRobot->getMotor("Conveyor")) = 100 * conveyorDirection;
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
        double mkP = 0.5;
        double mkI = 0.001;
        double mkD = 0.1;
        double mP = 0, mI = 0, mD = 0;
        double dist, prevDist;
        double moveSpeed;

        double tkP = 0;
        double tkI = 0;
        double tkD = 0;
        double tP = 0, tI = 0, tD = 0;
        double turnErr, prevTurnErr;
        double turnSpeed;

        Thread::pauseTask("move");
        sRobot->arcade(127, 0);
        while (sRobot->getInertial("Inertial")->get_roll() > 3) {
            pros::delay(2);
        }
        dist = sRobot->getDistance("Back")->get() - 400.0;
        prevDist = dist;
        turnErr = -1 * (sRobot->getInertial("Inertial")->get_rotation());
        prevTurnErr = turnErr;
        while (dist > 10 || turnErr > 2) {
            dist = sRobot->getDistance("Back")->get() - 400.0;
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

            sRobot->arcade(moveSpeed, turnSpeed);

            printf("\ndist = %d, moveSpeed = %d, turnErr = %d, turnSpeed = %d", (int)dist, (int)moveSpeed, (int)turnErr, (int)turnSpeed);
            pros::delay(20);
        }

        sRobot->stopDrive();
        pros::delay(500);
        Thread::notifyTask("fps");
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