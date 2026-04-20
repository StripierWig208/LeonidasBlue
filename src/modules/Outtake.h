#pragma once
#include "vex.h"

using namespace vex;

enum OuttakeState{
  CLOSE,
  OPEN,
  STUCK 
};

class Outtake{
  private:
  OuttakeState state;
  motor door;

  public:
  distance senzor1;
  distance senzor2;
  Outtake();
  void update();
  void home();
  OuttakeState getState();
  void setState(OuttakeState state);
  bool hasObject1();
  bool hasObject2();
};