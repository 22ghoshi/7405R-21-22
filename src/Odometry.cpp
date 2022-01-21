#include "Odometry.hpp"

Odometry* Odometry::pInstance = NULL;

Odometry::Odometry() {
    currentPos.x = 0;
    currentPos.y = 0;
    currentPos.h = sRobot->getInertial("Inertial")->get_rotation();
    targetPos.x = 0;
    targetPos.y = 0;
    targetPos.h = 0;
    moveDist = 0;
    turnErr = 0;
    mkP = 0;
    mkI = 0;
    mkD = 0;
    macc = 20;
    tkP = 0;
    tkI = 0;
    tkD = 0;
    tacc = 4;
    moves = 0;
    n = 0;
}

Odometry* Odometry::Instance() {
    if (!pInstance) {
        pInstance = new Odometry();
    }
    return pInstance;
}

void Odometry::FPS(void* params) {
    double previousHeading = sOdom->currentPos.h;
    while(true) {
        sOdom->currentPos.h = sRobot->getInertial("Inertial")->get_rotation();
        double heading = sOdom->currentPos.h.load();

        double deltaRotation = heading - previousHeading;
        double verticalMovement = ((sRobot->getEncoder("Left")->get_value() + sRobot->getEncoder("Right")->get_value()) / 2.0);

        Point deltaPoint;
        deltaPoint.x = (verticalMovement * sin(sOdom->toRadians(heading)));
        deltaPoint.y = (verticalMovement * cos(sOdom->toRadians(heading)));

        sOdom->currentPos += deltaPoint;

        pros::lcd::set_text(1, "X: " + std::to_string(sOdom->currentPos.x.load()));
		pros::lcd::set_text(2, "Y: " + std::to_string(sOdom->currentPos.y.load()));
        pros::lcd::set_text(3, "Inertial: " + std::to_string(sOdom->currentPos.h.load()));
        
        sRobot->getEncoder("Left")->reset();
        sRobot->getEncoder("Right")->reset();
        previousHeading = heading;

        pros::delay(20);
    }
}

// void Odometry::oldmoveTo(void* params) {
//     pros::delay(20);
//     double turn;
//     Point movePoint;
//     Point relativePoint;
//     double relativesum;

//     double mP = 0, mI = 0, mD = 0;
//     double prevMoveDist;

//     double tP = 0, tI = 0, tD = 0;
//     double prevTurnErr;

//     while(true) {
//         // pros::lcd::set_text(4, std::to_string(sOdom->moveDist));
//         turn = sOdom->currentPos.h.load();
//         if (fabs(turn) > 360.0) {
//             turn = std::fmod(turn, 360.0);
//         }
//         movePoint = sOdom->targetPos - sOdom->currentPos;
//         movePoint.h = sOdom->currentPos.angleTo(sOdom->targetPos);
        
//         sOdom->moveDist = sOdom->currentPos.distanceTo(sOdom->targetPos);
//         sOdom->turnErr = sOdom->targetPos.h.load() - sOdom->currentPos.h.load();
//         relativePoint.h = movePoint.h.load() - turn;
//         relativePoint.x = sOdom->moveDist * sin(sOdom->toRadians(relativePoint.h.load()));
//         relativePoint.y = sOdom->moveDist * cos(sOdom->toRadians(relativePoint.h.load()));
//         relativesum = fabs(relativePoint.x.load()) + fabs(relativePoint.y.load());

//         sOdom->n += 1;
//         if (sOdom->n == 1) {
//             prevMoveDist = sOdom->moveDist;
//             prevTurnErr = sOdom->turnErr;
//         }
        

//         if(sOdom->moveDist > sOdom->macc || sOdom->turnErr > sOdom->tacc || sOdom->turnErr < (-1 * sOdom->tacc)) {
//             mP = sOdom->moveDist;
//             mI += sOdom->moveDist;
//             mD = sOdom->moveDist - prevMoveDist;
//             prevMoveDist = sOdom->moveDist;

//             double moveSpeed = (sOdom->mkP *  mP) + (sOdom->mkI * mI) + (sOdom->mkD * mD);

//             tP = sOdom->turnErr;
//             tI += sOdom->turnErr;
//             tD = sOdom->turnErr - prevTurnErr;
//             prevTurnErr = sOdom->turnErr;

//             double turnSpeed = (sOdom->tkP * tP) + (sOdom->tkI * tI) + (sOdom->tkD * tD);

//             double speedSum = moveSpeed + fabs(turnSpeed);
//             double maxMoveSpeed = moveSpeed * (127 / speedSum);
//             double maxTurnSpeed = turnSpeed * (127 / speedSum);
//             if (moveSpeed > maxMoveSpeed) {
//                 moveSpeed = maxMoveSpeed;
//             }
//             if (fabs(turnSpeed) > fabs(maxTurnSpeed)) {
//                 turnSpeed = maxTurnSpeed;
//             }

//             *(sRobot->getMotor("BackLeft")) = ((relativePoint.y - relativePoint.x) * (moveSpeed / relativesum) + turnSpeed);
//             *(sRobot->getMotor("BackRight")) = ((relativePoint.y + relativePoint.x) * (moveSpeed / relativesum) - turnSpeed);
//             *(sRobot->getMotor("FrontLeft")) = ((relativePoint.y + relativePoint.x) * (moveSpeed / relativesum) + turnSpeed);
//             *(sRobot->getMotor("FrontRight")) = ((relativePoint.y - relativePoint.x) * (moveSpeed / relativesum) - turnSpeed);
//             printf("\nmoveDist = %d, moveSpeed = %d, turnErr = %d, turnSpeed = %d", (int)sOdom->moveDist, (int)moveSpeed, (int)sOdom->turnErr, (int)turnSpeed);
//         }
//         else {
//             sOdom->n = 0;
//             mI = 0;
//             tI = 0;
//             sRobot->stopDrive();
//         }

//         pros::delay(20);
//     }
// }

void Odometry::moveTo(void* params) {
    pros::delay(20);
    double turn;
    Point movePoint;

    int direction;
    double mP = 0, mI = 0, mD = 0;
    double prevMoveDist;

    double tP = 0, tI = 0, tD = 0;
    double prevTurnErr;

    while(true) {
        turn = sOdom->currentPos.h;
        if (fabs(turn) > 360.0) {
            turn = std::fmod(turn, 360.0);
        }
        movePoint = sOdom->targetPos - sOdom->currentPos;
        
        sOdom->moveDist = sOdom->currentPos.distanceTo(sOdom->targetPos);

        if (sOdom->moveDist > 200.0) {
            sOdom->targetPos.h = sOdom->currentPos.angleTo(sOdom->targetPos);

        }

        sOdom->turnErr = sOdom->targetPos.h.load() - sOdom->currentPos.h.load();

        direction = cos(sOdom->toRadians(sOdom->turnErr)) > 0 ? 1 : -1;
        if(direction < 0) {
            sOdom->turnErr = sOdom->turnErr > 0 ? sOdom->turnErr - 180 : sOdom->turnErr + 180;
        }

        sOdom->n += 1;
        if (sOdom->n == 1) {
            prevMoveDist = sOdom->moveDist;
            prevTurnErr = sOdom->turnErr;
            mI = 0;
            tI = 0;
        }

        if (fabs(sOdom->turnErr) > sOdom->tacc) {
            tP = sOdom->turnErr;
            tI += sOdom->turnErr;
            tD = sOdom->turnErr - prevTurnErr;
            prevTurnErr = sOdom->turnErr;

            double turnSpeed = (sOdom->tkP * tP) + (sOdom->tkI * tI) + (sOdom->tkD * tD);
            
            sRobot->arcade(0, turnSpeed);
            if (sOdom->n % 2 == 0) { printf("\ncurrentPos = (%d, %d, %d), move = %d, moveDist = %d, moveSpeed = turnbefore, turnErr = %d, turnSpeed = %d", (int)sOdom->currentPos.x, (int)sOdom->currentPos.y, (int)sOdom->currentPos.h, sOdom->moves, (int)sOdom->moveDist, (int)sOdom->turnErr, (int)turnSpeed); }
        }
        else if (sOdom->moveDist > sOdom->macc) {
            tP = sOdom->turnErr;
            tI += sOdom->turnErr;
            tD = sOdom->turnErr - prevTurnErr;
            prevTurnErr = sOdom->turnErr;

            double turnSpeed = (sOdom->tkP * tP) + (sOdom->tkI * tI) + (sOdom->tkD * tD);
            
            mP = sOdom->moveDist;
            mI += sOdom->moveDist;
            mD = sOdom->moveDist - prevMoveDist;
            prevMoveDist = sOdom->moveDist;

            double moveSpeed = (sOdom->mkP *  mP) + (sOdom->mkI * mI) + (sOdom->mkD * mD);

            double speedSum = moveSpeed + fabs(turnSpeed);
            double maxMoveSpeed = moveSpeed * (127 / speedSum);
            double maxTurnSpeed = turnSpeed * (127 / speedSum);
            if (moveSpeed > maxMoveSpeed) {
                moveSpeed = maxMoveSpeed;
            }
            if (fabs(turnSpeed) > fabs(maxTurnSpeed)) {
                turnSpeed = maxTurnSpeed;
            }
            
            sRobot->arcade(moveSpeed * direction, turnSpeed);
            if (sOdom->n % 2 == 0) { printf("\ncurrentPos = (%d, %d, %d), move = %d, moveDist = %d, moveSpeed = %d, turnErr = %d, turnSpeed = %d", (int)sOdom->currentPos.x, (int)sOdom->currentPos.y, (int)sOdom->currentPos.h, sOdom->moves, (int)sOdom->moveDist, (int)(moveSpeed * direction), (int)sOdom->turnErr, (int)turnSpeed); }
        }
        else {
            sRobot->stopDrive();
        }

        pros::delay(20);
    }
}

// void Odometry::tmoveTo(void* params) {
//     pros::delay(20);
//     double turn;
//     Point movePoint;

//     double mP = 0, mI = 0, mD = 0;
//     double prevMoveDist;

//     double tP = 0, tI = 0, tD = 0;
//     double prevTurnErr;

//     while(true) {
//         turn = sOdom->currentPos.h;
//         if (fabs(turn) > 360.0) {
//             turn = std::fmod(turn, 360.0);
//         }
//         movePoint = sOdom->targetPos - sOdom->currentPos;
//         movePoint.h = sOdom->currentPos.angleTo(sOdom->targetPos);
        
//         sOdom->moveDist = sOdom->currentPos.distanceTo(sOdom->targetPos);
//         sOdom->turnErr = sOdom->targetPos.h.load() - turn;

//         sOdom->n += 1;
//         if (sOdom->n == 1) {
//             prevMoveDist = sOdom->moveDist;
//             prevTurnErr = sOdom->turnErr;
//             mI = 0;
//             tI = 0;
//         }

//         if (fabs(sOdom->turnErr) > sOdom->tacc || sOdom->moveDist > sOdom->macc) {
//             tP = sOdom->turnErr;
//             tI += sOdom->turnErr;
//             tD = sOdom->turnErr - prevTurnErr;
//             prevTurnErr = sOdom->turnErr;

//             double turnSpeed = (sOdom->tkP * tP) + (sOdom->tkI * tI) + (sOdom->tkD * tD);
            
//             mP = sOdom->moveDist;
//             mI += sOdom->moveDist;
//             mD = sOdom->moveDist - prevMoveDist;
//             prevMoveDist = sOdom->moveDist;

//             double moveSpeed = (sOdom->mkP *  mP) + (sOdom->mkI * mI) + (sOdom->mkD * mD);

//             double speedSum = moveSpeed + fabs(turnSpeed);
//             double maxMoveSpeed = moveSpeed * (127 / speedSum);
//             double maxTurnSpeed = turnSpeed * (127 / speedSum);
//             if (moveSpeed > maxMoveSpeed) {
//                 moveSpeed = maxMoveSpeed;
//             }
//             if (fabs(turnSpeed) > fabs(maxTurnSpeed)) {
//                 turnSpeed = maxTurnSpeed;
//             }
            
//             sRobot->arcade(moveSpeed, turnSpeed);
//             printf("\nmoveDist = %d, moveSpeed = %d, turnErr = %d, turnSpeed = %d", (int)sOdom->moveDist, (int)moveSpeed, (int)sOdom->turnErr, (int)turnSpeed);
//         }
//         else {
//             sOdom->n = 0;
//             mI = 0;
//             tI = 0;
//             sRobot->stopDrive();
//         }

//         pros::delay(20);
//     }
// }



void Odometry::setTarget(double setx, double sety, std::initializer_list<double> mPID, double setmacc, std::initializer_list<double> tPID, double settacc) {
    waitUntilStop();
    moves++;
    n = 0;
    targetPos.x = setx;
    targetPos.y = sety;
    targetPos.h = currentPos.angleTo(targetPos);
    mkP = mPID.begin()[0];
    mkI = mPID.begin()[1];
    mkD = mPID.begin()[2];
    macc = setmacc;
    tkP = tPID.begin()[0];
    tkI = tPID.begin()[1];
    tkD = tPID.begin()[2];
    tacc = settacc;
}

void Odometry::setTarget(double setTurn, std::initializer_list<double> tPID, double settacc) {
    waitUntilStop();
    moves++;
    n = 0;
    targetPos.x = currentPos.x.load();
    targetPos.y = currentPos.y.load();
    targetPos.h = setTurn;
    macc = 100.0;
    tkP = tPID.begin()[0];
    tkI = tPID.begin()[1];
    tkD = tPID.begin()[2];
    tacc = settacc;
}

void Odometry::setTargetNow(double setx, double sety, std::initializer_list<double> mPID, double setmacc, std::initializer_list<double> tPID, double settacc) {
    moves++;
    n = 0;
    targetPos.x = setx;
    targetPos.y = sety;
    targetPos.h = currentPos.angleTo(targetPos);
    mkP = mPID.begin()[0];
    mkI = mPID.begin()[1];
    mkD = mPID.begin()[2];
    macc = setmacc;
    tkP = tPID.begin()[0];
    tkI = tPID.begin()[1];
    tkD = tPID.begin()[2];
    tacc = settacc;
}

void Odometry::setTargetNow(double setTurn, std::initializer_list<double> tPID, double settacc) {
    moves++;
    n = 0;
    targetPos.x = currentPos.x.load();
    targetPos.y = currentPos.y.load();
    targetPos.h = setTurn;
    macc = 100.0;
    tkP = tPID.begin()[0];
    tkI = tPID.begin()[1];
    tkD = tPID.begin()[2];
    tacc = settacc;
}

void Odometry::waitUntilStop() {
    if (moves == 0) {
        return;
    }
    std::deque<double> distances;
    distances.clear();
    std::deque<double> turns;
    turns.clear();
    double time = 0;
    while(distances.size() < 25 && turns.size() < 25) {
        distances.push_front(moveDist);
        turns.push_front(turnErr);
        pros::delay(20);
        time += 20;
    }
    while(moveDist > macc || fabs(turnErr) > tacc) {
        // if (fabs(sRobot->getMotor("BackLeft")->get_actual_velocity()) < 10.0 && fabs(sRobot->getMotor("FrontRight")->get_actual_velocity()) < 10.0) {
        //     // pros::c::controller_rumble(pros::E_CONTROLLER_MASTER, ".");
        //     break;
        // }
        if (fabs(distances.back() - distances.front()) < 15.0 && fabs(turns.back() - turns.front()) < 0.75) {
            // pros::c::controller_rumble(pros::E_CONTROLLER_MASTER, ".");
            printf("\nbroke");
            break;
        }
        distances.pop_back();
        distances.push_front(moveDist);
        turns.pop_back();
        turns.push_front(turnErr);
        pros::delay(20);
        time += 20;
    }
}

double Odometry::toRadians(double degrees) {
    return (degrees * (M_PI / 180.0));
}

double Odometry::toDegrees(double radians) {
    return (radians * (180.0 / M_PI));
}

double Odometry::turnToHorizontalMovement(double deltaRotation) {
    double circumference = 1050; //update this
    double distance = (deltaRotation / 360) * circumference;
    return distance;
}