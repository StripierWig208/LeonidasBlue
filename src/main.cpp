#include "vex.h"
#include "modules/Drivetrain.h"
#include "modules/Intake.h"
#include "modules/Incline.h"
#include "modules/Outtake.h"
#include "modules/IntakeGuide.h"
#include <algorithm>

using namespace vex;

extern vex::analog_in imuPos, imuNeg;

competition Competition;
controller con = controller(primary);

Drivetrain dt;
Intake intake;
Incline incline;
Outtake outtake;
IntakeGuide guide;

bool intakeForwardToggle = false;
bool intakeReverseToggle = false;
bool  doorToggle = false;
bool  doorStuck = false;
bool  doorStuckUsed = false;
bool turboToggle = false;
bool Guide = false;

OuttakeState prevOuttakeState = CLOSE;
int inclineMidTimer = 0;
int xCycleState = 0;
int yCycleState = 0;



void toggleIntakeForward() {
  xCycleState = 0;
  yCycleState = 0;
  intakeForwardToggle = !intakeForwardToggle;
  if (intakeForwardToggle) {
    intakeReverseToggle = false;
  }
}

void toggleIntakeReverse() {
  intakeReverseToggle = !intakeReverseToggle;
  if (intakeReverseToggle) {
    intakeForwardToggle = false;
  }
}

void toggleDoor() {
  doorToggle = !doorToggle;
}
void toggleDoorStuck() {
  if (doorStuckUsed) return;
  doorStuck = true;
  doorStuckUsed = true;
  doorToggle = false;
}
void toggleTurbo() {
  turboToggle = !turboToggle;
}

void toggleGuide()
{
  Guide=!Guide;
}

void cycleX() {
  if (yCycleState > 0) yCycleState = 0; // cancel Y when X starts
  if (xCycleState == 0) {
    xCycleState = 1;
  } else if (xCycleState == 1) {
    xCycleState = 2;
  } else {
    xCycleState = 1;
    intakeForwardToggle = false;
    doorToggle = false;
  }
}

bool rotateBot (double angle)
{
  motorPowers pow = {0, 0};
  bool ret = dt.setHeading (angle, pow);
  dt.setPower(pow);
  return ret;
}

void rotateToAngle(double angle, int timeoutMs = 500) {
  int elapsed = 0;
  while (!rotateBot(angle) && elapsed < timeoutMs) {
    incline.update();
    intake.update();
    outtake.update();
    guide.update();
    wait(10, msec);
    elapsed += 10;
  }
  dt.setPower({0, 0});
}

void cycleY() {
  if (xCycleState > 0) return;
  if (yCycleState == 0) {
    yCycleState = 1;                          // 1st press: incline down
  } else if (yCycleState == 1) {
    yCycleState = 2;                          // 2nd press: intake + open door
  } else {
    yCycleState = 1;                          // 3rd press: close, stay down
    intakeForwardToggle = false;
    doorToggle = false;
  }
}

double batterVoltage = 0;
void driveDistance(double mm, double heading = 0) {
  dt.resetXEncoder();
  bool done = false;
  dt.startForwardMotion(mm);
  while (!done) {
    motorPowers power = {0, 0};
    dt.setHeading(heading, power);
    done = dt.AutonomousDrive(power);
    dt.setPower(power);
    incline.update();
    intake.update();
    outtake.update();
    guide.update();
    wait(10, msec);
  }
  dt.setPower({0, 0});
}

void autonomous(void) {
  incline.home();
  driveDistance(103);
  rotateToAngle(154);
  incline.setState(HIGH);
  guide.setState(GUIDE_DOWN);
  intake.setState(INTAKE);
  driveDistance(-30, 130);
  dt.forwardProfile.decceleration = 1000;
  for (int i = 0; i < 100; i++) {
    incline.update();
    guide.update();
    intake.update();
    outtake.update();
    wait(3, msec);
  }
  dt.forwardProfile.decceleration = 150;
  driveDistance(73, 127);
  timer t;
  // t.reset();
  // while (t.time(sec) < 6 && (outtake.hasObject2() && t.time(sec)>1) ){
  //     incline.update(); intake.update(); outtake.update(); guide.update();
  // }


  // for (int i = 0; i < 1; i++) {
  //   for (int j = 0; j < 100; j++) {
  //     incline.update(); intake.update(); outtake.update(); guide.update();
  //     wait(8, msec);
  //   }
  //   driveDistance(-45, 130);
  //   guide.setState(GUIDE_DOWN);
  //   driveDistance(57, 125);
  // }//de schimbat dupa ce merge fata spate
  driveDistance(-88, 137);
  driveDistance(-20, 130);
  outtake.setState(OPEN);
  driveDistance(-37, 128);
  
  // while (t.time(sec) < 6 && outtake.hasObject1()) {
  //   incline.update(); intake.update(); outtake.update(); guide.update();
  // }
  outtake.setState(CLOSE);
  driveDistance(50, dt.getRobotYaw(DEGREES));
  driveDistance(85, 130);

  // while (t.time(sec) < 6 && (outtake.hasObject2() && t.time(sec)>1)) {
  //     incline.update(); intake.update(); outtake.update(); guide.update();
  //   }

  // for (int i = 0; i < 1; i++) {
  //   for (int j = 0; j < 100; j++) {
  //     incline.update(); intake.update(); outtake.update(); guide.update();
  //     wait(8, msec);
  //   }
  //   guide.setState(GUIDE_DOWN);
  //   driveDistance(-45, 130);
  //   driveDistance(57, 125);
  // }//de schimbat dupa ce merge fata spate

  driveDistance(-88, 137);
  driveDistance(-20, 130);
  outtake.setState(OPEN);
  driveDistance(-37, 128);
  while (t.time(sec) < 6 && outtake.hasObject1()) {
    incline.update(); intake.update(); outtake.update(); guide.update();
  }
  outtake.setState(CLOSE);
  driveDistance(30, dt.getRobotYaw(DEGREES));
  guide.setState(GUIDE_UP);
  driveDistance(40, 135);


  // // dt.resetDriveRotations();
  // outtake.setState(CLOSE);
  // driveDistance(60);
  
  // for (int i = 0; i < 300; i++) {
  //   incline.update(); intake.update(); outtake.update(); guide.update();
  //   wait(8, msec);
  // }
  // for (int i = 0; i < 250; i++) {
  //   intake.setState(REVERSE);
  //   incline.update(); intake.update(); outtake.update(); guide.update();
  //   wait(2, msec);
  //   intake.setState(INTAKE);
  //   incline.update(); intake.update(); outtake.update(); guide.update();
  //   wait(3, msec);
  // }
  // outtake.setState(CLOSE);
  // driveDistance(50, 130);
  // rotateToAngle(-120);
  // guide.setState(GUIDE_UP);
  // for (int i = 0; i < 50; i++) {
  //   guide.update();
  //   wait(10, msec);
  // }
  // driveDistance(70, -120);
  // rotateToAngle(-30);
  // driveDistance(50, -30);

}

aivision::colordesc COL1(1, 242, 90, 90, 10, 0.20);

bool saw = false;
double deltaTime = 0;

void usercontrol(void) {

  while (true) {
    // a1.takeSnapshot(aivision::ALL_COLORS);
    // if(a1.objectCount > 0){
    //   Brain.Screen.printAt(10, 30, "id: %d", a1.objects[0].id);
    //   saw = true;
    // } else {
    //   Brain.Screen.printAt(10, 30, "No  objs");
    //   saw = false;
    // }
    if (doorStuck) {
      outtake.setState(STUCK);
      if (outtake.getState() == CLOSE) doorStuck = false;
    } else if (doorToggle) {
      outtake.setState(OPEN);
    } else {
      outtake.setState(CLOSE);
    }

    if (prevOuttakeState == OPEN && outtake.getState() == CLOSE) {
      inclineMidTimer = 20;
    }

    if (outtake.getState() == OPEN) {
      intake.setState(INTAKE);
      guide.setState(GUIDE_DOWN);
    } else {
      guide.setState(GUIDE_UP);

      if (intakeForwardToggle || saw) {
        intake.setState(INTAKE);
        incline.setState(HIGH);
      } else if (intakeReverseToggle) {
        intake.setState(REVERSE);
        incline.setState(MID);
      } else {
        intake.setState(STOP);
      }
    }

    // X cycle: press 1 = incline HIGH, press 2 = intake + open door, press 3 = close (stays HIGH)
    if (xCycleState >= 1) {
      incline.setState(HIGH);
      inclineMidTimer = 0;
    }
    if (xCycleState == 2) {
      intake.setState(INTAKE);
      outtake.setState(OPEN);
    }

    // Y cycle (blocked while X active): 1=incline down, 2=intake+door open, 3=close (stays down)
    if (yCycleState >= 1) {
      incline.setState(MID);
      inclineMidTimer = 0;
    }
    if (yCycleState == 2) {
      intake.setState(INTAKE);
      outtake.setState(OPEN);
    }

    if (inclineMidTimer > 0) {
      incline.setState(MID);
      inclineMidTimer--;
    }

    if (turboToggle) {
      dt.setSpeedMultiplier(1.3);
    } else {
      dt.setSpeedMultiplier(0.95);
    }

    if(Guide)
      guide.setState(GUIDE_DOWN);
      else guide.setState(GUIDE_UP);  
    incline.update();
    dt.update(con);
    intake.update();
    outtake.update();
    guide.update();

    prevOuttakeState = outtake.getState();

    Brain.Screen.printAt(3, 200, "yaw: %.1f deg  A:%d B:%d",
      dt.getRobotYaw(DEGREES),
      imuPos.value(vex::analogUnits::mV),
      imuNeg.value(vex::analogUnits::mV));

    wait(20, msec);
  }
}

void odotest(){
  while(true){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(3, 20, "X: %lf", dt.getDistanceOnX());
    wait(10, msec);
  }
}

void backAndforth(){
  while(true){
    bool done = false;
    //dt.forwardProfile.setStartPos(dt.getDistanceOnX());
    dt.startForwardMotion(-200);

    while(!done){
      motorPowers power = {0, 0};
      
      dt.setHeading(0, power);
      done = dt.AutonomousDrive(power);
      dt.setPower(power);
      Brain.Screen.printAt(3, 20, "X: %lf", dt.getDistanceOnX());
      Brain.Screen.printAt(3, 40, "velo: %lf", dt.getXVelocity());
    }
    dt.setPower({0, 0});
    wait(1, sec);
    dt.resetXEncoder();
    done = false;
    //dt.forwardProfile.setStartPos(dt.getDistanceOnX());
    dt.startForwardMotion(200);
    while(!done){
      motorPowers power = {0, 0};
      dt.setHeading(0, power);
      done = dt.AutonomousDrive(power);
      dt.setPower(power);
      Brain.Screen.printAt(3, 20, "X: %lf", dt.getDistanceOnX());
      Brain.Screen.printAt(3, 40, "velo: %lf", dt.getXVelocity());

      // Brain.Screen.printAt(3, 40, "pow: %lf", power.left);
    }
    dt.setPower({0, 0});
    wait(1, sec);
    dt.resetXEncoder();
  }
}

void testImu(){
  // while(1){
  //   Brain.Screen.clearScreen();
  //   Brain.Screen.printAt(3, 60, "pos: %lf", ((double)imuPos.value(vex::analogUnits::mV) - 147.0) / (3142.0 - 147.0) * 180.0);
  //   Brain.Screen.printAt(3, 40, "nog: %lf", ((double)imuNeg.value(vex::analogUnits::mV) - 103.0) / (3092.0 - 103.0));
  //   Brain.Screen.printAt(3, 20, "yaw: %lfdeg", dt.getRobotYaw(DEGREES));
  //   wait(5, msec);
  // }
  while(1)
  {
    Brain.Screen.clearScreen();
    imuPos.value(analogUnits::mV);
    imuNeg.value(analogUnits::mV);
    Brain.Screen.printAt(3, 60, "pos: %d", (imuPos.value(vex::analogUnits::mV)));
    Brain.Screen.printAt(3, 40, "neg: %d", (imuNeg.value(vex::analogUnits::mV)));
    Brain.Screen.printAt(3, 20, "yaw: %lfdeg", dt.getRobotYaw(DEGREES));
    wait (3,msec);
  }
  
}



void testAMP(){
  motor test(PORT12);
  MotionProfile profile(100, 100, 200);
  int sgn = 1;
  while(true){
    profile.setStartPos(0.0);
    profile.startMotion(360.0 * sgn);
    while(!profile.motionDone()){
      //test.setPosition(profile.getPosition(), rotationUnits::deg);
      Brain.Screen.printAt(4, 20, "pos: %lf", profile.getPosition());
      profile.update();
    }
    Brain.Screen.clearScreen();
    wait(500, msec);
    sgn *= -1;
  }
}

void testHeading(){
  double targetPos = 135;
  while(true){
    motorPowers power = {0, 0};
    dt.setHeading(targetPos, power);
    // while(!dt.setHeading(targetPos, power)){
      dt.setPower(power);
      power = {0, 0};
    Brain.Screen.printAt(3, 10, "yaw: %lf", dt.getRobotYaw(DEGREES));
    // }
    // dt.resetHeading(0);
    // wait(500, msec);
  }
}

int main() {
  vexcodeInit();
  
  con.ButtonA.pressed(toggleIntakeForward);
  con.ButtonB.pressed(toggleIntakeReverse);
  con.ButtonUp.pressed(toggleDoor);
  con.ButtonDown.pressed(toggleGuide);
  con.ButtonR1.pressed(toggleTurbo);
  con.ButtonLeft.pressed(toggleGuide);
  con.ButtonX.pressed(cycleX);
  con.ButtonY.pressed(cycleY);

  //Competition.autonomous(autonomous);
  Competition.drivercontrol(autonomous);

  // while (true) {
  //   Brain.Screen.clearScreen();
  //   Brain.Screen.printAt(3, 20, "yaw: %.1f deg", dt.getRobotYaw(DEGREES));
  //   Brain.Screen.printAt(3, 40, "C(pos): %d mV", imuPos.value(vex::analogUnits::mV));
  //   Brain.Screen.printAt(3, 60, "D(neg): %d mV", imuNeg.value(vex::analogUnits::mV));
  //   wait(50, msec);
  // }
}