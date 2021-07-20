#include "Sensor.hpp"

std::map<std::string, std::unique_ptr<Sensor>> Sensor::sensors;
std::map<std::string, std::unique_ptr<pros::ADIButton>> Sensor::button;
std::map<std::string, std::unique_ptr<pros::ADILineSensor>> Sensor::line;
std::map<std::string, std::unique_ptr<pros::ADIPotentiometer>> Sensor::potentiometer;
std::map<std::string, std::unique_ptr<pros::ADIUltrasonic>> Sensor::ultrasonic;
std::map<std::string, std::unique_ptr<pros::ADIEncoder>> Sensor::encoder;
std::map<std::string, std::unique_ptr<pros::Rotation>> Sensor::rotation;
std::map<std::string, std::unique_ptr<pros::Imu>> Sensor::inertial;
std::map<std::string, std::unique_ptr<pros::Vision>> Sensor::vision;

Sensor::Sensor() {}

Sensor::Sensor(std::string sensorName, sensorClass sensorType, std::uint8_t sensorPort, bool reversed) {
    if ((sensorType == sensorClass::inertial || sensorType == sensorClass::vision || sensorType == sensorClass::rotation) && (sensorPort < 1 || sensorPort > 21)) {
        throw std::invalid_argument("inertial/vision port 1 - 21 only");
    }
    else if (sensorPort < 1 || sensorPort > 8) {
        throw std::invalid_argument("triport 1 - 8 only");
    }
    else if ((sensorType == sensorClass::ultrasonic || sensorType == sensorClass::encoder) && (sensorPort % 2 != 1)) {
        throw std::invalid_argument("ultrasonic/encoder start odd port only");
    }

    this->name = sensorName;
    this->type = sensorType;
    this->port = sensorPort;
    this->reverse = reversed;

    switch(type) {
        case sensorClass::button:
            button[name] = std::make_unique<pros::ADIButton>(port);
            break;
        case sensorClass::line:
            line[name] = std::make_unique<pros::ADILineSensor>(port);
            break;
        case sensorClass::potentiometer:
            potentiometer[name] = std::make_unique<pros::ADIPotentiometer>(port);
            break;
        case sensorClass::ultrasonic:
            ultrasonic[name] = std::make_unique<pros::ADIUltrasonic>(port, port + 1);
            break;
        case sensorClass::encoder:
           encoder[name] = std::make_unique<pros::ADIEncoder>(port, port + 1, reverse);
           break;
        case sensorClass::rotation:
            rotation[name] = std::make_unique<pros::Rotation>(port);
            break;
        case sensorClass::inertial:
            inertial[name] = std::make_unique<pros::Imu>(port);
            break;
        case sensorClass::vision:
            vision[name] = std::make_unique<pros::Vision>(port);
            break;
    }
}

Sensor::Sensor(std::string sensorName, sensorClass sensorType, std::pair<std::uint8_t, std::uint8_t> sensorPorts, bool reversed) {
    if (sensorPorts.second < 1 || sensorPorts.second > 8) {
        throw std::invalid_argument("expander triport 1 - 8 only");
    }
    else if ((sensorType == sensorClass::ultrasonic || sensorType == sensorClass::encoder) && (sensorPorts.second % 2 != 1)) {
        throw std::invalid_argument("ultrasonic/encoder start odd port only");
    }
    
    this->name = sensorName;
    this->type = sensorType;
    this->port = sensorPorts.second;
    this->reverse = reversed;

    switch(type) {
        case sensorClass::button:
            button[name] = std::make_unique<pros::ADIButton>(std::make_pair(sensorPorts.first, port));
            break;
        case sensorClass::line:
            line[name] = std::make_unique<pros::ADILineSensor>(std::make_pair(sensorPorts.first, port));
            break;
        case sensorClass::potentiometer:
            potentiometer[name] = std::make_unique<pros::ADIAnalogIn>(std::make_pair(sensorPorts.first, port));
            break;
        case sensorClass::ultrasonic:
            ultrasonic[name] = std::make_unique<pros::ADIUltrasonic>(std::make_tuple(sensorPorts.first, port, port + 1));
            break;
        case sensorClass::encoder:
           encoder[name] = std::make_unique<pros::ADIEncoder>(std::make_tuple(sensorPorts.first, port, port + 1), reverse);
           break;
        default:
            throw std::invalid_argument("not supported for expander");
            break;
    }
}

std::string Sensor::getName() {
    return this->name;
}

sensorClass Sensor::getType() {
    return this->type;
}

pros::ADIButton* Sensor::getButton(std::string name) {
    return Sensor::button.at(name).get();
}
pros::ADILineSensor* Sensor::getLine(std::string name) {
	return Sensor::line.at(name).get();
}
pros::ADIPotentiometer* Sensor::getPotentiometer(std::string name) {
	return Sensor::potentiometer.at(name).get();
}
pros::ADIUltrasonic* Sensor::getUltrasonic(std::string name) {
	return Sensor::ultrasonic.at(name).get();
}
pros::ADIEncoder* Sensor::getEncoder(std::string name) {
	return Sensor::encoder.at(name).get();
}
pros::Rotation* Sensor::getRotation(std::string name) {
    return Sensor::rotation.at(name).get();
}
pros::Imu* Sensor::getInertial(std::string name) {
	return Sensor::inertial.at(name).get();
}
pros::Vision* Sensor::getVision(std::string name) {
	return Sensor::vision.at(name).get();
}