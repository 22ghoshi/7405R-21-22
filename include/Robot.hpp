#pragma once
#include "main.h"

#include "Devices.hpp"
#include "Motor.hpp"

#include <map>
#include <memory>
#include <string>
#include <queue>
#include <vector>
#include <functional>

enum class motors { BackLeft, BackRight, MidLeft, MidRight, FrontLeft, FrontRight, Conveyor, Lift };
enum class motorGroups { LeftDrive, RightDrive, Drive };
enum class pistons { FrontClamp, BackClamp, Tilter };
enum class sensors { Inertial, LeftEncoder, RightEncoder, LiftPotentiometer, BackDistance, FrontDistance };

namespace Robot {   
    template <motors motor>
    pros::Motor getMotor();

    template<> inline
    pros::Motor getMotor<motors::BackLeft>() {
        return *(Devices::getMotor("Back Left", 17, motorGearset::GS200, true));
    }

    template<> inline
    pros::Motor getMotor<motors::BackRight>() {
        return *(Devices::getMotor("Back Right", 1, motorGearset::GS200));
    }

    template<> inline
    pros::Motor getMotor<motors::MidLeft>() {
        return *(Devices::getMotor("Mid Left", 3, motorGearset::GS200));
    }

    template<> inline
    pros::Motor getMotor<motors::MidRight>() {
        return *(Devices::getMotor("Mid Right", 13, motorGearset::GS200, true));
    }

    template<> inline
    pros::Motor getMotor<motors::FrontLeft>() {
        return *(Devices::getMotor("Front Left", 15, motorGearset::GS200, true));
    }

    template<> inline
    pros::Motor getMotor<motors::FrontRight>() {
        return *(Devices::getMotor("Front Right", 7, motorGearset::GS200));
    }

    template<> inline
    pros::Motor getMotor<motors::Conveyor>() {
        return *(Devices::getMotor("Conveyor", 21, motorGearset::GS600, true));
    }

    template<> inline
    pros::Motor getMotor<motors::Lift>() {
        return *(Devices::getMotor("Lift", 11, motorGearset::GS100, true));
    }

    template <motorGroups motorGroup>
    MotorGroup getMotorGroup();

    template<> inline
    MotorGroup getMotorGroup<motorGroups::LeftDrive>() {
        return MotorGroup({getMotor<motors::BackLeft>, getMotor<motors::MidLeft>, getMotor<motors::FrontLeft>});
    }

    template<> inline
    MotorGroup getMotorGroup<motorGroups::RightDrive>() {
        return MotorGroup({getMotor<motors::BackRight>, getMotor<motors::MidRight>, getMotor<motors::FrontRight>});
    }

    template<> inline
    MotorGroup getMotorGroup<motorGroups::Drive>() {
        return MotorGroup({getMotor<motors::BackLeft>, getMotor<motors::BackRight>, getMotor<motors::MidLeft>, getMotor<motors::MidRight>, getMotor<motors::FrontLeft>, getMotor<motors::FrontRight>});
    }

    template <pistons piston>
    pros::ADIDigitalOut getPiston();

    template<> inline
    pros::ADIDigitalOut getPiston<pistons::BackClamp>() {
        return *(Devices::getPiston("Back Clamp", 4));
    }

    template<> inline
    pros::ADIDigitalOut getPiston<pistons::Tilter>() {
        return *(Devices::getPiston("Tilter", 2));
    }

    template<> inline
    pros::ADIDigitalOut getPiston<pistons::FrontClamp>() {
        return *(Devices::getPiston("Front Clamp", 1));
    }

    template <sensors sensor>
    auto getSensor();

    template<> inline
    auto getSensor<sensors::Inertial>() {
        return *(Devices::getInertial("Inertial", 20));
    }

    template<> inline
    auto getSensor<sensors::LeftEncoder>() {
        return *(Devices::getEncoder("Left", 7));
    }

    template<> inline
    auto getSensor<sensors::RightEncoder>() {
        return *(Devices::getEncoder("Right", 5));
    }

    template<> inline
    auto getSensor<sensors::LiftPotentiometer>() {
        return *(Devices::getPotentiometer("Lift", 3));
    }

    template<> inline
    auto getSensor<sensors::BackDistance>() {
        return *(Devices::getDistance("Back", 9));
    }

    template<> inline
    auto getSensor<sensors::FrontDistance>() {
        return *(Devices::getDistance("Front", 10));
    }

    void mecanum(int power, int strafe, int turn);
    void arcade(int power, int turn);
    void tank(int left, int right);
    void stopDrive();
};