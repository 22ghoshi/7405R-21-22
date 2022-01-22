#pragma once
#include "main.h"

#include "Robot.hpp"

#include <atomic>

#define sOdom Odometry::Instance()

struct Point {
    std::atomic<double> x;
    std::atomic<double> y;
    std::atomic<double> h;

    Point() {
        this->x = 0;
        this->y = 0;
        this->h = 0;
    }
    Point(double x, double y) {
        this->x = x;
        this->y = y;
        this->h = 0;
    }
    Point(const Point& point) {
        this->x = point.x.load();
        this->y = point.y.load();
        this->h = point.h.load();
    }
    void operator=(const Point& equal) {
        this->x = equal.x.load();
        this->y = equal.y.load();
        this->h = equal.h.load();
    };
    void operator+=(const Point& add) {
        this->x = this->x + add.x;
        this->y = this->y + add.y;
    };
    Point operator-(const Point& sub) {
        Point res(this->x - sub.x, this->y - sub.y);
        return res;
    };
    double distanceTo(const Point& to) {
        double dist;
        dist = sqrt(((to.x - x) * (to.x - x)) + ((to.y - y) * (to.y - y)));
        return dist;
    };
    
    //returns angle relative to y axis in degrees from -180 to 180
    double angleTo(const Point& to) { 
        double angle;
        double tox, toy;
        tox = to.x - this->x;
        toy = to.y - this->y;
        angle = ((atan2(tox, toy)) * (180.0 / M_PI));
        return angle;
    };
};

class Odometry {
    private:
    Odometry();
    static Odometry* pInstance;
    double mkP;
    double mkI;
    double mkD;
    double macc;
    double tkP;
    double tkI;
    double tkD;
    double tacc;
    int n;

    bool turning;

    public:
    Point currentPos;
    Point targetPos;
    double moveDist;
    double turnErr;
    int moves;
    
    static Odometry* Instance();
    static void FPS(void* params);
    // static void oldmoveTo(void* params);
    static void moveTo(void* params);
    // static void tmoveTo(void* params);
    void setTarget(double setx, double sety, std::initializer_list<double> mPID = {0.25, 0.0, 0.2}, double setmacc = 20, std::initializer_list<double> tPID = {1.5, 0.0, 0.4}, double settacc = 2.0);
    void setTarget(double setTurn, std::initializer_list<double> tPID = {1.5, 0.0, 0.4}, double settacc = 2.0);
    void setTargetNow(double setx, double sety, std::initializer_list<double> mPID = {0.25, 0.0, 0.2}, double setmacc = 20, std::initializer_list<double> tPID = {1.5, 0.0, 0.4}, double settacc = 2.0);
    void setTargetNow(double setTurn, std::initializer_list<double> tPID = {1.5, 0.0, 0.4}, double settacc = 2.0);
    void waitUntilStop();
    double toRadians(double degrees);
    double toDegrees(double radians);
    double turnToHorizontalMovement(double deltaRotation);
};