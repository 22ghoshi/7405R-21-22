#pragma once

enum class liftStates {down = 1050, downlow = 1180, low = 1250, lowmid = 1400, mid = 1400, midhigh = 1700, high = 3350};

namespace robotFuncs {
    void drive(void* params);
    void nothing();
    void testmove();
    void liftPID(void* params);
    void moveLift();
    void manualmoveLift();
    void manualholdLift();
    void conveyor(void* params);
    void conveyorIn();
    void conveyorOut();
    void toggleFrontClamp();
    void toggleBackClamp();
    void toggleTilter();

    void autonLift(liftStates setLiftState);
    void auton_conveyor(int time);

    void controllerPrint(void* params);
}