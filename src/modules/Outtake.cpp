#include "Outtake.h"


// motor shooter  = motor(PORT10, ratio36_1, true);
// motor outtake = motor(PORT11, ratio36_1, true);
Outtake::Outtake() : door(PORT11,ratio18_1,true), senzor1(PORT8), senzor2(PORT7) {
  state = CLOSE;
}
void Outtake::home() {
  door.setPosition(0, deg);
  state = CLOSE;
  door.spinToPosition(0, deg, 200, rpm, true);
}

void Outtake::update(){
    switch(state){
      case CLOSE:{
        door.spinToPosition(0,deg, 200, rpm,false);
        break;
      }
      case OPEN:{
        door.spinToPosition(120,deg, 200, rpm,false);
        break;
      }
      case STUCK:{
        door.spinToPosition(-60, deg, 200, rpm, false);
        if (door.position(deg) == 160) {
          door.setPosition(0, deg);
          state = CLOSE;
        }
        break;
      }
    }
  }
  OuttakeState Outtake::getState(){
    return state;
  }
  void Outtake::setState(OuttakeState state){
    this->state = state;
  }
  bool Outtake::hasObject1(){
    return senzor1.objectDistance(distanceUnits::mm)<100;
  }

  double lastDist = 400;
  const double LPF = 0.8;
  bool Outtake::hasObject2()
  {
    double a=senzor2.objectDistance(distanceUnits::mm);
      lastDist = a * (1-LPF) + lastDist * LPF;
      return lastDist<200;
  }
