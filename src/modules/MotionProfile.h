#pragma once
#include "vex.h"

class MotionProfile{
    double acceleration = 0;
    double decceleration = 0;
    double maxVelocity = 0;
    double maxUsedVelocity = 0;
    double position = 0;
    double startPos = 0;
    double t0 = 0, t1 = 0, t2 = 0;
    vex::timer time_;
    int sgn = 1;
public:
    MotionProfile(double Acc, double Decc, double MaxVel);
    void startMotion(double targetMotion);
    void setStartPos(double pos);
    void update();
    double getPosition();
    // double getVelocity(); 
    bool motionDone();

};