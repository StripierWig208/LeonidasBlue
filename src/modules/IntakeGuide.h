#pragma once
#include "vex.h"

using namespace vex;



enum IntakeGuideState {
  GUIDE_UP,
  GUIDE_DOWN
};

class IntakeGuide {
private:
  IntakeGuideState state;
  int stallCount = 0;
  motor guide;

public:
  IntakeGuide();

  void update();
  void setState(IntakeGuideState state);
  IntakeGuideState getState();
};