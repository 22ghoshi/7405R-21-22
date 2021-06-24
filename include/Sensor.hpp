#pragma once
#include "main.h"

#include <map>
#include <memory>
#include <string>
#include <stdexcept>

enum class sensorClass { button, line, potentiometer, ultrasonic, encoder, inertial, vision };

class Sensor {
    private:
    static std::map<std::string, std::unique_ptr<pros::ADIButton>> button;
    static std::map<std::string, std::unique_ptr<pros::ADILineSensor>> line;
    static std::map<std::string, std::unique_ptr<pros::ADIPotentiometer>> potentiometer;
    static std::map<std::string, std::unique_ptr<pros::ADIUltrasonic>> ultrasonic;
    static std::map<std::string, std::unique_ptr<pros::ADIEncoder>> encoder;
    static std::map<std::string, std::unique_ptr<pros::Imu>> inertial;
    static std::map<std::string, std::unique_ptr<pros::Vision>> vision;
    std::string name;
    sensorClass type;
    std::uint8_t port;
    bool reverse;

    public:
    static std::map<std::string, std::unique_ptr<Sensor>> sensors;
    
    Sensor();
    Sensor(std::string sensorName, sensorClass sensorType, std::uint8_t sensorPort, bool reversed = false);
    Sensor(std::string sensorName, sensorClass sensorType, std::pair<std::uint8_t, std::uint8_t> sensorPorts, bool reversed = false); //expander constructor
    std::string getName();
    sensorClass getType();
    static pros::ADIButton* getButton(std::string name);
    static pros::ADILineSensor* getLine(std::string name);
    static pros::ADIPotentiometer* getPotentiometer(std::string name);
    static pros::ADIUltrasonic* getUltrasonic(std::string name);
    static pros::ADIEncoder* getEncoder(std::string name);
    static pros::Imu* getInertial(std::string name);
    static pros::Vision* getVision(std::string name);
};