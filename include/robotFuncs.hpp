#pragma once

enum class mLiftStates {down = 1010, low = 1110, lowmid = 1300, mid = 2216, high = 2626};

namespace robotFuncs {
    void drive(void* params);
    void nothing();
    void testmove();
    void mLiftPID(void* params);
    void move_mLift();
    void manualmove_mLift();
    void manualhold_mLift();
    void toggle_pLift();
    void toggleConveyor();
    void clamp();
    void holdClamp();
    void resetRotations();

    void auton_clamp(int time);
    void auton_declamp(int time);
    void auton_mLift(mLiftStates liftState);
    void auton_conveyor(int time);
}