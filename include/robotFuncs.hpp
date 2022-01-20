#pragma once

enum class liftStates {down = 130, low = 425, lowmid = 1500, mid = 1400, high = 1950};

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