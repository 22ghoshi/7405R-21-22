#pragma once

enum class liftStates {down = 1090, downlow = 1180, low = 1200, lowmid = 1325, mid = 2075, midhigh = 1700, high = 2600, veryhigh = 2700};

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