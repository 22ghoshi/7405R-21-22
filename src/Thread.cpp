#include "Thread.hpp"

Thread* Thread::pInstance = NULL;

Thread::Thread() {};

Thread* Thread::Instance() {
    if (!pInstance) {
        pInstance = new Thread();
    }
    return pInstance;
}

void Thread::startTask(std::string name, void (*func)(void*), void* params) {
	if (!existsTask(name)) {
		tasks.insert(std::pair<std::string, std::unique_ptr<pros::Task>>(name, std::move(std::make_unique<pros::Task>(func, params, name.c_str()))));
	}
}

void Thread::pauseTask(std::string name) {
	tasks.at(name)->suspend();
}

void Thread::resumeTask(std::string name) {
	tasks.at(name)->resume();
}

void Thread::killTask(std::string name) {
	if (existsTask(name)) {
		tasks.at(name)->remove();
		tasks.erase(name);
	}
}

bool Thread::isRunning(std::string name) {
	return tasks.at(name)->get_state() == pros::E_TASK_STATE_RUNNING;
}

bool Thread::existsTask(std::string name) {
	return tasks.find(name) != tasks.end();
}