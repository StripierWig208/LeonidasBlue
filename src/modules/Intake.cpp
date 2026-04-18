#include "Intake.h"
#include "Incline.h"

//intake guide servo port 13
Intake::Intake() : intakeInside(PORT14, ratio18_1, false), intakeOutside(PORT15, ratio18_1, true), intakeOpposite(PORT20, ratio18_1, true),intakeOppositeOutside(PORT10, ratio18_1, false){
  this->state = STOP;
}
void Intake::update(){
  switch(state){
    case STOP:{

     // inclineLeft.spinToPosition(30, degrees, 100, velocityUnits::rpm);
      //inclineRight.spinToPosition(-30, degrees, 100, velocityUnits::rpm);
      intakeOutside.spin(forward,0,rpm);
      intakeInside.spin(forward,0,rpm);
      intakeOpposite.spin(forward,0,rpm);
      intakeOppositeOutside.spin(forward,0,rpm);

      break;
    }
    case INTAKE:{
     // inclineLeft.spinToPosition(80, degrees, 100, velocityUnits::rpm);
      //inclineRight.spinToPosition(-80, degrees, 100, velocityUnits::rpm);
      intakeOutside.spin(forward,200,rpm);
      intakeInside.spin(forward,200,rpm);
      intakeOpposite.spin(forward,200,rpm);
      intakeOppositeOutside.spin(forward,200,rpm);

      break;
    }
    case REVERSE:{
     // inclineLeft.spinToPosition(80, degrees, 100, velocityUnits::rpm);
     // inclineRight.spinToPosition(-80, degrees, 100, velocityUnits::rpm);
      intakeOutside.spin(forward,-200,rpm);
      intakeInside.spin(forward,-200,rpm);
      intakeOpposite.spin(forward,-200,rpm);
      intakeOppositeOutside.spin(forward,-200,rpm);

      break;
    }
  }
}

Viteza Intake::getState(){
  return state;
}
void Intake::setState(Viteza state){
  this->state = state;
}