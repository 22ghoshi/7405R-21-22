#pragma once
#include "main.h"

#include "Robot.hpp"
#include "Odometry.hpp"

#include <map>
#include <memory>
#include <string>

class Thread {
    private:
    Thread();

    static std::map<std::string, std::unique_ptr<pros::Task>> tasks;

    public:
    static void startTask(std::string name, void (*func)(void*), void* params = nullptr);
	static void pauseTask(std::string name);
	static void resumeTask(std::string name);
	static void killTask(std::string name);
	static bool isRunning(std::string name);
	static bool existsTask(std::string name);
};