#pragma once
#include "vex.h"

using namespace vex;

enum Viteza{
  STOP,
  INTAKE,
  REVERSE
};

class Intake{
  private:
  Viteza state;
  motor intakeInside,intakeOutside,intakeOpposite,intakeOppositeOutside;
  public:
  Intake();
  void update();
  Viteza getState();
  void setState(Viteza state);
};