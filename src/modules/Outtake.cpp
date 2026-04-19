#include "Outtake.h"


// motor shooter  = motor(PORT10, ratio36_1, true);
// motor outtake = motor(PORT11, ratio36_1, true);
Outtake::Outtake() : door(PORT11,ratio18_1,true){
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
        door.spinToPosition(96,deg, 200, rpm,false);
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