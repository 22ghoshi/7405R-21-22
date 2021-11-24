#pragma once

namespace robotFuncs {
    void drive(void* params);
    void resetMotors();
    void nothing();
    void testmove();
    void mLiftPID(void* params);
    void move_mLift();
    void toggle_pLift();
    void toggleConveyor();
    void clamp();
    void holdClamp();
}