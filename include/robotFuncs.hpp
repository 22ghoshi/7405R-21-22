#pragma once

enum class liftStates {down = 675, low = 1050, lowmid = 800, mid = 1980, high = 2035};

namespace robotFuncs {
    void drive(void* params);
    void nothing();
    void testmove();
    void liftPID(void* params);
    void moveLift();
    void manualmoveLift();
    void manualholdLift();
    void toggleConveyor();
    void toggleFrontClamp();
    void toggleBackClamp();
    void toggleTilter();

    void auton_clamp(int time);
    void auton_declamp(int time);
    void autonLift(liftStates setLiftState);
    void auton_conveyor(int time);

    void controllerPrint(void* params);
}