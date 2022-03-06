#pragma once

enum class liftStates {down = 870, downlow = 940, low = 970, lowmid = 1120, mid = 1900, midhigh = 2120, high = 2430, veryhigh = 2800};

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
    void distanceBack(double distance);

    void controllerPrint(void* params);
}