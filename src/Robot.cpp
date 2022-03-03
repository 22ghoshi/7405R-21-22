#include "Robot.hpp"

Robot* Robot::pInstance = NULL;

Robot::Robot() {
	//dead ports 2 4 5 6 10 12 14 19
	motors["BackLeft"] = std::make_shared<Motor>("BackLeft", motorGearset::GS200, 17, true);
	motors["BackRight"] = std::make_shared<Motor>("BackRight", motorGearset::GS200, 1);
	motors["MidLeft"] = std::make_shared<Motor>("MidLeft", motorGearset::GS200, 3);
	motors["MidRight"] = std::make_shared<Motor>("MidRight", motorGearset::GS200, 13, true);
	motors["FrontLeft"] = std::make_shared<Motor>("FrontLeft", motorGearset::GS200, 15, true);
	motors["FrontRight"] = std::make_shared<Motor>("FrontRight", motorGearset::GS200, 7);
	makeMotorGroup("LeftDrive", {"BackLeft", "MidLeft", "FrontLeft"});
	makeMotorGroup("RightDrive", {"BackRight", "MidRight", "FrontRight"});
	makeMotorGroup("Drive", {"BackLeft", "BackRight", "MidLeft", "MidRight", "FrontLeft", "FrontRight"});
	motors["Conveyor"] = std::make_shared<Motor>("Conveyor", motorGearset::GS600, 21, true);
	motors["Lift"] = std::make_shared<Motor>("Lift", motorGearset::GS100, 11, true);
	
	pistons["FrontClamp"] = std::make_unique<Piston>("FrontClamp", 1);
	pistons["BackClamp"] = std:: make_unique<Piston>("BackClamp", 4);
	pistons["Tilter"] = std::make_unique<Piston>("Tilter", 2);
	
	sensors["Inertial"] = std::make_unique<Sensor>("Inertial", sensorClass::inertial, 20);
	sensors["Left Encoder"] = std::make_unique<Sensor>("Left", sensorClass::encoder, 7);
	sensors["Right Encoder"] = std::make_unique<Sensor>("Right", sensorClass::encoder, 5);
	sensors["Lift Potentiometer"] = std::make_unique<Sensor>("Lift", sensorClass::potentiometer, std::make_pair(16, 8));
	sensors["Front Ultrasonic"] = std::make_unique<Sensor>("Front", sensorClass::ultrasonic, std::make_pair(16, 1));
	sensors["Back Distance"] = std::make_unique<Sensor>("Back", sensorClass::distance, 9);

	getInertial("Inertial")->reset();
	pros::delay(3000);
}

Robot* Robot::Instance() {
    if (!pInstance) {
        pInstance = new Robot();
    }
    return pInstance;
}

void Robot::mecanum(int power, int strafe, int turn) {
	*(getMotor("BackLeft")) = power - strafe + turn;
	*(getMotor("BackRight")) = power + strafe - turn;
	*(getMotor("FrontLeft")) = power + strafe + turn;
	*(getMotor("FrontRight")) = power - strafe - turn;
	
}

void Robot::arcade(int power, int turn) {
	*(getMotorGroup("LeftDrive")) = power + turn;
	*(getMotorGroup("RightDrive")) = power - turn;
}

void Robot::tank(int left, int right) {
	*(getMotorGroup("LeftDrive")) = left;
	*(getMotorGroup("RightDrive")) = right;
}

void Robot::move(double dist, std::initializer_list<double> mPID, double macc, std::initializer_list<double> tPID, double tacc) {
	double mkP = mPID.begin()[0];
    double mkI = mPID.begin()[1];
    double mkD = mPID.begin()[2];

    double tkP = tPID.begin()[0];
    double tkI = tPID.begin()[1];
    double tkD = tPID.begin()[2];

	double mP = 0, mI = 0, mD = 0;
	double moveDist, prevMoveDist, moveSpeed;
	double tP = 0, tI = 0, tD = 0;
	double turnErr, prevTurnErr, turnSpeed;

	moveDist = dist - sRobot->getMotorGroup("Drive")->getEncoders();
	turnErr = -(sRobot->getInertial("Inertial")->get_rotation());
	prevMoveDist = moveDist;
	prevTurnErr = turnErr;

	while (moveDist > macc || fabs(turnErr) > tacc) {
		moveDist = dist - sRobot->getMotorGroup("Drive")->getEncoders();
		turnErr = -(sRobot->getInertial("Inertial")->get_rotation());
		prevMoveDist = moveDist;
		prevTurnErr = turnErr;

		mP = moveDist;
		mI += moveDist;
		mD = moveDist - prevMoveDist;
		prevMoveDist = moveDist;

		moveSpeed = (mkP *  mP) + (mkI * mI) + (mkD * mD);

		tP = turnErr;
		tI += turnErr;
		tD = turnErr - prevTurnErr;
		
		turnSpeed = (tkP * tP) + (tkI * tI) + (tkD * tD);

		double speedSum = moveSpeed + fabs(turnSpeed);
        double maxMoveSpeed = moveSpeed * (127 / speedSum);
        double maxTurnSpeed = turnSpeed * (127 / speedSum);
		if (moveSpeed > maxMoveSpeed) {
            moveSpeed = maxMoveSpeed;
        }
        if (fabs(turnSpeed) > fabs(maxTurnSpeed)) {
            turnSpeed = maxTurnSpeed;
        }

        sRobot->arcade(moveSpeed, turnSpeed);
	}

}

void Robot::stopDrive() {
    getMotorGroup("Drive")->stop(brakeType::brake);
}

void Robot::makeMotorGroup(std::string name, std::vector<std::string> motorNames) {
	std::vector<std::shared_ptr<Motor>> motorGroupMotors;
	for (auto const& motorName : motorNames) {
		motorGroupMotors.push_back(motors[motorName]);
	}
	motorGroups[name] = std::make_unique<MotorGroup>(name, motorGroupMotors);
}

pros::Motor* Robot::getMotor(std::string name) {
	return motors.at(name).get()->getMotor();
}

MotorGroup* Robot::getMotorGroup(std::string name) {
	return motorGroups.at(name).get();
}

pros::ADIDigitalOut* Robot::getPiston(std::string name) {
	return pistons.at(name).get()->getPiston();
}

pros::ADIButton* Robot::getButton(std::string name) {
    return Sensor::button.at(name).get();
}
pros::ADILineSensor* Robot::getLine(std::string name) {
	return Sensor::line.at(name).get();
}
pros::ADIPotentiometer* Robot::getPotentiometer(std::string name) {
	return Sensor::potentiometer.at(name).get();
}
pros::ADIUltrasonic* Robot::getUltrasonic(std::string name) {
	return Sensor::ultrasonic.at(name).get();
}
pros::ADIEncoder* Robot::getEncoder(std::string name) {
	return Sensor::encoder.at(name).get();
}
pros::Rotation* Robot::getRotation(std::string name) {
    return Sensor::rotation.at(name).get();
}
pros::Imu* Robot::getInertial(std::string name) {
	return Sensor::inertial.at(name).get();
}
pros::Vision* Robot::getVision(std::string name) {
	return Sensor::vision.at(name).get();
}

pros::Distance* Robot::getDistance(std::string name) {
	return Sensor::distance.at(name).get();
}