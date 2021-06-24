#pragma once
#include "main.h"

#include "Robot.hpp"

#define sOdom Odometry::Instance()

struct Point {
    double x = 0;
    double y = 0;
    double h = 0;
    void operator+=(const Point& add) {
        this->x += add.x;
        this->y += add.y; 
    };
    Point operator-(const Point& sub) {
        Point res;
        res.x = this->x - sub.x;
        res.y = this->y - sub.y;
        return res;
    };
    void operator=(const Point& equal) {
        this->x = equal.x;
        this->y = equal.y;
        this->h = equal.h;
    };
    double distanceTo(const Point& to) {
        double dist;
        dist = sqrt(((to.x - x) * (to.x - x)) + ((to.y - y) * (to.y - y)));
        return dist;
    };
    double angleTo(const Point& to) /*returns angle RELATIVE TO Y AXIS IN DEGREES from -180 to 180*/ { 
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

    public:
    Point currentPos;
    Point targetPos;
    double moveDist;
    double turnErr;
    double mkP;
    double mkI;
    double mkD;
    double macc;
    double tkP;
    double tkI;
    double tkD;
    double tacc;
    int moves;
    
    static Odometry* Instance();
    static void FPS(void* params);
    static void moveTo(void* params);
    void setTarget(double setx, double sety, double setTurn, std::initializer_list<double> mPID = {0.25, 0.0, 0.2}, double setmacc = 20, std::initializer_list<double> tPID = {1.5, 0.0, 0.4}, double settacc = 2.0);
    void setTargetNow(double setx, double sety, double setTurn, std::initializer_list<double> mPID = {0.25, 0.0, 0.2}, double setmacc = 20, std::initializer_list<double> tPID = {1.5, 0.0, 0.4}, double settacc = 2.0);
    void waitUntilStop();
    double toRadians(double degrees);
    double toDegrees(double radians);
    double turnToHorizontalMovement(double deltaRotation);
};