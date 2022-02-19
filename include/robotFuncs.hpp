#pragma once

enum class liftStates {down = 0, low = 500, lowmid = 850, mid = 1000, high = 3350};

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

    void auton_mogo(int ultrasonicLimit, double yLimit);
    void auton_clamp(int time);
    void auton_declamp(int time);
    void autonLift(liftStates setLiftState);
    void auton_conveyor(int time);

    void controllerPrint(void* params);
}