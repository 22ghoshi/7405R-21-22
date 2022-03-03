#pragma once
#include "main.h"

#include <map>
#include <memory>
#include <string>
#include <stdexcept>

enum class sensorClass { button, line, potentiometer, ultrasonic, encoder, rotation, inertial, vision, distance };

class Sensor {
    private:
    std::string name;
    sensorClass type;
    std::uint8_t port;
    bool reverse;

    public:
    static std::map<std::string, std::unique_ptr<pros::ADIButton>> button;
    static std::map<std::string, std::unique_ptr<pros::ADILineSensor>> line;
    static std::map<std::string, std::unique_ptr<pros::ADIPotentiometer>> potentiometer;
    static std::map<std::string, std::unique_ptr<pros::ADIUltrasonic>> ultrasonic;
    static std::map<std::string, std::unique_ptr<pros::ADIEncoder>> encoder;
    static std::map<std::string, std::unique_ptr<pros::Rotation>> rotation;
    static std::map<std::string, std::unique_ptr<pros::Imu>> inertial;
    static std::map<std::string, std::unique_ptr<pros::Vision>> vision;
    static std::map<std::string, std::unique_ptr<pros::Distance>> distance;
    
    Sensor();
    Sensor(std::string sensorName, sensorClass sensorType, std::uint8_t sensorPort, bool reversed = false);
    Sensor(std::string sensorName, sensorClass sensorType, std::pair<std::uint8_t, std::uint8_t> sensorPorts, bool reversed = false); //expander constructor
    std::string getName();
    sensorClass getType();
};