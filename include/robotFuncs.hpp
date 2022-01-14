#pragma once

enum class liftStates {down = 1010, low = 1110, lowmid = 1300, mid = 2216, high = 2626};

namespace robotFuncs {
    void drive(void* params);
    void nothing();
    void testmove();
    void liftPID(void* params);
    void moveLift();
    void manualmoveLift();
    void manualholdLift();
    // void toggleConveyor();
    void toggleFrontClamp();
    void toggleBackClamp();
    void toggleTilter();

    void auton_clamp(int time);
    void auton_declamp(int time);
    void autonLift(liftStates setLiftState);
    // void auton_conveyor(int time);
}