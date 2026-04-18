#pragma once
#include "vex.h"

using namespace vex;

enum PozitieIncline{
  MID,
  HIGH
};

class Incline{
private:
  PozitieIncline state;
  bool homing;

  motor inclineLeft;
  motor inclineRight;

public:
  Incline();

  void update();
  void home();
  PozitieIncline getState();
  void setState(PozitieIncline state);
};
