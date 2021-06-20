#include "Odometry.hpp"

Odometry* Odometry::oInstance = NULL;

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
    macc = 100;
    tkP = 0;
    tkI = 0;
    tkD = 0;
    tacc = 5;
    moves = 0;
}

Odometry* Odometry::Instance() {
    if (!oInstance) {
        oInstance = new Odometry();
    }
    return oInstance;
}

void Odometry::FPS(void* params) {
    double previousHeading = sOdom->currentPos.h;
    while(true) {
        sOdom->currentPos.h = sRobot->getInertial("Inertial")->get_rotation();
        double heading = sOdom->currentPos.h;

        double deltaRotation = heading - previousHeading;
        double verticalMovement = ((sRobot->getEncoder("Left")->get_value() + sRobot->getEncoder("Right")->get_value()) / 2);
        double badHorizontalMovement = sOdom->turnToHorizontalMovement(deltaRotation);
        double horizontalMovement = (sRobot->getEncoder("Middle")->get_value() + badHorizontalMovement);

        Point deltaPoint;
        deltaPoint.x = (verticalMovement * sin(sOdom->toRadians(heading))) + (horizontalMovement * cos(sOdom->toRadians(heading)));
        deltaPoint.y = (verticalMovement * cos(sOdom->toRadians(heading))) - (horizontalMovement * sin(sOdom->toRadians(heading)));

        sOdom->currentPos += deltaPoint;

        pros::lcd::set_text(1, "X: " + std::to_string(sOdom->currentPos.x));
		pros::lcd::set_text(2, "Y: " + std::to_string(sOdom->currentPos.y));
        pros::lcd::set_text(3, "Inertial: " + std::to_string(sOdom->currentPos.h));
        
        sRobot->getEncoder("Left")->reset();
        sRobot->getEncoder("Middle")->reset();
        sRobot->getEncoder("Right")->reset();
        previousHeading = heading;

        pros::delay(20);
    }
}

void Odometry::moveTo(void* params) {
    pros::delay(20);
    double turn;
    Point movePoint;
    Point relativePoint;
    double relativesum;

    double mP = 0, mI = 0, mD = 0;
    double prevMoveDist;

    double tP = 0, tI = 0, tD = 0;
    double prevTurnErr;

    int n = 0;

    while(true) {
        // pros::lcd::set_text(4, std::to_string(sOdom->moveDist));
        turn = sOdom->currentPos.h;
        if (fabs(turn) > 360.0) {
            turn = std::fmod(turn, 360.0);
        }
        movePoint = sOdom->targetPos - sOdom->currentPos;
        movePoint.h = sOdom->currentPos.angleTo(sOdom->targetPos);
        
        sOdom->moveDist = sOdom->currentPos.distanceTo(sOdom->targetPos);
        sOdom->turnErr = sOdom->targetPos.h - sOdom->currentPos.h;
        relativePoint.h = movePoint.h - turn;
        relativePoint.x = sOdom->moveDist * sin(sOdom->toRadians(relativePoint.h));
        relativePoint.y = sOdom->moveDist * cos(sOdom->toRadians(relativePoint.h));
        relativesum = fabs(relativePoint.x) + fabs(relativePoint.y);

        n += 1;
        if (n == 1) {
            prevMoveDist = sOdom->moveDist;
            prevTurnErr = sOdom->turnErr;
        }
        

        if(sOdom->moveDist > sOdom->macc || sOdom->turnErr > sOdom->tacc || sOdom->turnErr < (-1 * sOdom->tacc)) {
            mP = sOdom->moveDist;
            mI += sOdom->moveDist;
            mD = sOdom->moveDist - prevMoveDist;
            prevMoveDist = sOdom->moveDist;

            double moveSpeed = (sOdom->mkP *  mP) + (sOdom->mkI * mI) + (sOdom->mkD * mD);

            tP = sOdom->turnErr;
            tI += sOdom->turnErr;
            tD = sOdom->turnErr - prevTurnErr;
            prevTurnErr = sOdom->turnErr;

            double turnSpeed = (sOdom->tkP * tP) + (sOdom->tkI * tI) + (sOdom->tkD * tD);

            double speedSum = moveSpeed + fabs(turnSpeed);
            double maxMoveSpeed = moveSpeed * (127 / speedSum);
            double maxTurnSpeed = turnSpeed * (127 / speedSum);
            if (moveSpeed > maxMoveSpeed) {
                moveSpeed = maxMoveSpeed;
            }
            if (fabs(turnSpeed) > fabs(maxTurnSpeed)) {
                turnSpeed = maxTurnSpeed;
            }

            *(sRobot->getMotor("BackLeft")) = ((relativePoint.y - relativePoint.x) * (moveSpeed / relativesum) + turnSpeed);
            *(sRobot->getMotor("BackRight")) = ((relativePoint.y + relativePoint.x) * (moveSpeed / relativesum) - turnSpeed);
            *(sRobot->getMotor("FrontLeft")) = ((relativePoint.y + relativePoint.x) * (moveSpeed / relativesum) + turnSpeed);
            *(sRobot->getMotor("FrontRight")) = ((relativePoint.y - relativePoint.x) * (moveSpeed / relativesum) - turnSpeed);
            printf("\nmoveDist = %d, moveSpeed = %d, turnErr = %d, turnSpeed = %d", (int)sOdom->moveDist, (int)moveSpeed, (int)sOdom->turnErr, (int)turnSpeed);
        }
        else {
            n = 0;
            mI = 0;
            tI = 0;
            sOdom->stopDrive();
        }

        pros::delay(20);
    }
}

void Odometry::setTarget(double setx, double sety, double setTurn, std::initializer_list<double> mPID, double setmacc, std::initializer_list<double> tPID, double settacc) {
    waitUntilStop();
    moves++;
    printf("\nmove number %d", moves);
    targetPos.x = setx;
    targetPos.y = sety;
    targetPos.h = setTurn;
    mkP = mPID.begin()[0];
    mkI = mPID.begin()[1];
    mkD = mPID.begin()[2];
    macc = setmacc;
    tkP = tPID.begin()[0];
    tkI = tPID.begin()[1];
    tkD = tPID.begin()[2];
    tacc = settacc;
}

void Odometry::setTargetNow(double setx, double sety, double setTurn, std::initializer_list<double> mPID, double setmacc, std::initializer_list<double> tPID, double settacc) {
    moves++;
    printf("\nmove number %d", moves);
    targetPos.x = setx;
    targetPos.y = sety;
    targetPos.h = setTurn;
    mkP = mPID.begin()[0];
    mkI = mPID.begin()[1];
    mkD = mPID.begin()[2];
    macc = setmacc;
    tkP = tPID.begin()[0];
    tkI = tPID.begin()[1];
    tkD = tPID.begin()[2];
    tacc = settacc;
}

void Odometry::waitUntilStop() {
    std::deque<double> distances;
    distances.clear();
    std::deque<double> turns;
    turns.clear();
    double time = 0;
    while(distances.size() < 25 && turns.size() < 25) {
        distances.push_front(sOdom->moveDist);
        turns.push_front(sOdom->turnErr);
        pros::delay(20);
        time += 20;
    }
    while(sOdom->moveDist > sOdom->macc || sOdom->turnErr > sOdom->tacc || sOdom->turnErr < (-1 * tacc)) {
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
        distances.push_front(sOdom->moveDist);
        turns.pop_back();
        turns.push_front(sOdom->turnErr);
        pros::delay(20);
        time += 20;
    }
}

void Odometry::stopDrive() {
    sRobot->getMotor("BackLeft")->move_velocity(0);
    sRobot->getMotor("BackRight")->move_velocity(0);
    sRobot->getMotor("FrontLeft")->move_velocity(0);
    sRobot->getMotor("FrontRight")->move_velocity(0);
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