#include "./IntakeGuide.h"
using namespace vex;

IntakeGuide::IntakeGuide()
  : guide(PORT1, ratio18_1, false) {
  state = GUIDE_UP;
  guide.setStopping(hold);
  guide.setPosition(0, deg);
}

void IntakeGuide::update() {
  switch (state) {
    case GUIDE_DOWN:
      guide.spinToPosition(-430, degrees, 200, velocityUnits::rpm, false);
      break;
    case GUIDE_UP:
      guide.spinToPosition(0, degrees, 200, velocityUnits::rpm, false);
      break;
  }
}

void IntakeGuide::setState(IntakeGuideState state) {
  this->state = state;
}

IntakeGuideState IntakeGuide::getState() {
  return state;
}
