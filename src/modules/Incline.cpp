#include "Incline.h"

Incline::Incline()
    : inclineLeft(PORT13, ratio18_1, false),
      inclineRight(PORT12, ratio18_1, true) {
  state = MID;
  homing = false;
  inclineLeft.setStopping(hold);
  inclineRight.setStopping(hold);
}

void Incline::home() {
  homing = true;
  Brain.Screen.printAt(10, 10, "Homing incline...");

  inclineLeft.spin(reverse, 200, rpm);
  inclineRight.spin(reverse, 200, rpm);

  wait(100, msec); // let movement begin

  // Stall detection: if position stops changing, we've hit the hard stop
  double prevPos = inclineLeft.position(deg);
  int stableCount = 0;

  while (stableCount < 4) {
    wait(30, msec);
    double currPos = inclineLeft.position(deg);
    if (fabs(currPos - prevPos) < 2.0) {
      stableCount++;
    } else {
      stableCount = 0;
    }
    prevPos = currPos;
  }

  inclineLeft.stop(hold);
  inclineRight.stop(hold);

  inclineLeft.setPosition(0, deg);
  inclineRight.setPosition(0, deg);
  state = MID;
  homing = false;

  Brain.Screen.printAt(10, 10, "Incline homed.   ");
}

void Incline::update() {
  if (homing) return;
  double angle;
  if (state == MID) {
    angle = 0;
  } else {
    angle = 363;
  }

  inclineLeft.spinToPosition(angle, degrees, 200, velocityUnits::rpm, false);
  inclineRight.spinToPosition(angle, degrees, 200, velocityUnits::rpm, false);

  Brain.Screen.printAt(10, 50, "INC L:%.0f R:%.0f -> %.0f",
    inclineLeft.position(deg), inclineRight.position(deg), angle);
}

PozitieIncline Incline::getState(){
  return state;
}

void Incline::setState(PozitieIncline state){
  this->state = state;
}
