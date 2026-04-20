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
      if (abs(guide.velocity(velocityUnits::rpm)) < 5 &&
          abs(guide.position(degrees) - (-400)) > 10) {
        stallCount++;
      } else {
        stallCount = 0;
      }
      if (stallCount > 5) {
        guide.stop(hold);
        guide.setPosition(-400, degrees);
        stallCount = 0;
        break;
      }
      guide.spinToPosition(-400, degrees, 200, velocityUnits::rpm, false);
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
