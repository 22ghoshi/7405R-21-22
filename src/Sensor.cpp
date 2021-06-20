#include "Sensor.hpp"

std::map<std::string, std::unique_ptr<Sensor>> Sensor::sensors;
std::map<std::string, std::unique_ptr<pros::ADIButton>> Sensor::button;
std::map<std::string, std::unique_ptr<pros::ADILineSensor>> Sensor::line;
std::map<std::string, std::unique_ptr<pros::ADIPotentiometer>> Sensor::potentiometer;
std::map<std::string, std::unique_ptr<pros::ADIUltrasonic>> Sensor::ultrasonic;
std::map<std::string, std::unique_ptr<pros::ADIEncoder>> Sensor::encoder;
std::map<std::string, std::unique_ptr<pros::Imu>> Sensor::inertial;
std::map<std::string, std::unique_ptr<pros::Vision>> Sensor::vision;

Sensor::Sensor(std::string sensorName, sensorClass sensorType, std::uint8_t sensorPort, bool reversed) {
    if ((sensorType == sensorClass::inertial || sensorType == sensorClass::vision) && (sensorPort < 1 || sensorPort > 21)) {
        throw std::invalid_argument("inertial/vision port 1 - 21 only");
    }
    else if (sensorPort < 1 || sensorPort > 8) {
        throw std::invalid_argument("triport 1 - 8 only");
    }
    else if ((sensorType == sensorClass::ultrasonic || sensorType == sensorClass::encoder) && (sensorPort % 2 != 1)) {
        throw std::invalid_argument("ultrasonic/encoder start odd port only");
    }

    name = sensorName;
    type = sensorType;
    port = sensorPort;
    reverse = reversed;

    switch(sensorType) {
        case sensorClass::button:
            button[name] = std::make_unique<pros::ADIButton>(sensorPort);
            break;
        case sensorClass::line:
            line[name] = std::make_unique<pros::ADILineSensor>(sensorPort);
            break;
        case sensorClass::potentiometer:
            potentiometer[name] = std::make_unique<pros::ADIPotentiometer>(sensorPort);
            break;
        case sensorClass::ultrasonic:
            ultrasonic[name] = std::make_unique<pros::ADIUltrasonic>(sensorPort, sensorPort + 1);
            break;
        case sensorClass::encoder:
           encoder[name] = std::make_unique<pros::ADIEncoder>(sensorPort, sensorPort + 1, reverse);
           break;
        case sensorClass::inertial:
            inertial[name] = std::make_unique<pros::Imu>(sensorPort);
            break;
        case sensorClass::vision:
            vision[name] = std::make_unique<pros::Vision>(sensorPort);
    }
}

Sensor::Sensor(std::string sensorName, sensorClass sensorType, std::pair<std::uint8_t, std::uint8_t> sensorPorts, bool reversed) {
    if (sensorPorts.second < 1 || sensorPorts.second > 8) {
        throw std::invalid_argument("expander triport 1 - 8 only");
    }
    else if ((sensorType == sensorClass::ultrasonic || sensorType == sensorClass::encoder) && (sensorPorts.second % 2 != 1)) {
        throw std::invalid_argument("ultrasonic/encoder start odd port only");
    }
    
    name = sensorName;
    type = sensorType;
    port = sensorPorts.second;
    reverse = reversed;

    switch(sensorType) {
        case sensorClass::button:
            button[name] = std::make_unique<pros::ADIButton>(std::make_pair(sensorPorts.first, sensorPorts.second));
            break;
        case sensorClass::line:
            line[name] = std::make_unique<pros::ADILineSensor>(std::make_pair(sensorPorts.first, sensorPorts.second));
            break;
        case sensorClass::potentiometer:
            potentiometer[name] = std::make_unique<pros::ADIAnalogIn>(std::make_pair(sensorPorts.first, sensorPorts.second));
            break;
        case sensorClass::ultrasonic:
            ultrasonic[name] = std::make_unique<pros::ADIUltrasonic>(std::make_tuple(sensorPorts.first, sensorPorts.second, sensorPorts.second + 1));
            break;
        case sensorClass::encoder:
           encoder[name] = std::make_unique<pros::ADIEncoder>(std::make_tuple(sensorPorts.first, sensorPorts.second, sensorPorts.second + 1), reverse);
           break;
        default:
            throw std::invalid_argument("not supported for expander");
    }
}