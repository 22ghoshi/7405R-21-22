#pragma once

enum class mLiftStates {down = 1010, low = 1110, lowmid = 1500, mid = 2216, high = 2626};

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
    void auton_clamp();
    void auton_declamp();
    void auton_mLift(mLiftStates liftState);
}