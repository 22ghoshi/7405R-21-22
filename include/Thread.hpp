#pragma once
#include "main.h"

#include "Robot.hpp"
#include "Odometry.hpp"

#include <map>
#include <memory>
#include <string>

#define sThread Thread::Instance()

class Thread {
    private:
    Thread();

    std::map<std::string, std::unique_ptr<pros::Task>> tasks;

    static Thread* pInstance;

    public:
    static Thread* Instance();

    void startTask(std::string name, void (*func)(void*), void* params = nullptr);
	void pauseTask(std::string name);
	void resumeTask(std::string name);
	void killTask(std::string name);
	bool isRunning(std::string name);
	bool existsTask(std::string name);
};