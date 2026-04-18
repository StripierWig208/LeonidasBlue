#include "MotionProfile.h"
#include <cmath>
#include "vex.h"

MotionProfile::MotionProfile(double a, double d, double mv):
    acceleration(a), decceleration(d), maxVelocity(mv), startPos{0}{};

void MotionProfile::setStartPos(double pos){ 
    startPos = pos;
}
void MotionProfile::startMotion(double pos){
    if(startPos == pos) return;
    double dist = std::abs(startPos - pos);
    if(pos > startPos) sgn = 1;
    else sgn = -1;

    //Brain.Screen.printAt(3, 60, "%d, s: %lf, end: %lf", sgn, startPos, pos);
    double atime = maxVelocity / acceleration;
    double dtime = maxVelocity / decceleration;

    if(dist <= (atime + dtime) * maxVelocity / 2.0){
        t1 = 0;
        maxUsedVelocity = std::sqrt(2 * dist / (1.0 / acceleration + 1.0 / decceleration));
        t0 = maxUsedVelocity / acceleration;
        t2 = maxUsedVelocity / decceleration;
        t1 += t0;
        t2 += t1;

    } else {
        t1 = dist / maxVelocity - acceleration / 2.0 - decceleration / 2.0;
        t0 = atime;
        t2 = dtime;
        maxUsedVelocity = maxVelocity; 
        t1 += t0;
        t2 += t1;
    }
    time_.reset();
}

void MotionProfile::update(){
    if(motionDone()) return;
    double t = time_.value();
    if(t <= t0) position = acceleration / 2.0 * t * t;
    else if(t <= t1) position = acceleration / 2.0 * t0 * t0 + maxUsedVelocity * (t - t0);
    else if(t <= t2) position = acceleration / 2.0 * t0 * t0 + maxUsedVelocity * (t - t0) - decceleration / 2.0 * (t - t1) * (t - t1);
}
double MotionProfile::getPosition(){
    return sgn * position + startPos;
}
bool MotionProfile::motionDone(){ return time_.value() >= t2;}
