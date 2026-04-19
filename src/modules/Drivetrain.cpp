#include "Drivetrain.h"
#include <algorithm>
#include <cmath>
using namespace vex;

static double xWheelRadius = 82.5 / 2.0, yWheelRadius = 82.55 / 2.0;
static double xEncoderCPR = 8192, yEncoderCPR = 8192;
const double distanceWheels = 278 / 1e3;
const double wheelRadiusLen = 82.5 / 1e3 * 3.141592;
const double kS = 0.0;

int sgn(double val){if(val < 0) return -1; return 1;}

vex::encoder x(Brain.ThreeWirePort.E);
vex::analog_in imuPos(Brain.ThreeWirePort.C),
               imuNeg(Brain.ThreeWirePort.D);
const unsigned int minAVal = 147, minBVal = 103;
const unsigned int maxAVal = 3142 - minAVal, maxBVal = 3092 - minBVal;

Drivetrain::Drivetrain()
    : leftFront(PORT16, ratio18_1, false),
      leftBack(PORT17, ratio18_1, false),
      rightFront(PORT18, ratio18_1, true),
      rightBack(PORT19, ratio18_1, true),
      leftDrive(leftFront, leftBack),
      rightDrive(rightFront, rightBack),
      headingController(0.01, 0, 0),
      voltage{Brain.Battery.voltage(volt)},
      forwardController(0.014, 0.00000000002, 0.000001),
      forwardProfile(800, 150, 5000)//acceleratie,deceleratie,viteza maxima o sa fie nev sa le modifci
      {
  leftDrive.setStopping(hold);
  rightDrive.setStopping(hold);
  forwardProfile.setStartPos(0);
}

void Drivetrain::setSpeedMultiplier(double multiplier) {
  speedMultiplier = multiplier;
}

bool Drivetrain::AutonomousDrive(motorPowers& p){
  double distanceMM = forwardProfile.getPosition();
  forwardProfile.update();
  Brain.Screen.printAt(3, 100, "mp pos: %lf", distanceMM);
  //if(std::abs(distanceMM - getDistanceOnX()) < ERROR_ON_X) return true;
  double pow = -forwardController.update(getDistanceOnX() - distanceMM, getXVelocity(), true);
  if (pow > 0.85) pow = 0.85;
  if (pow < -0.85) pow = -0.85;
  p.left += pow;
  p.right += pow;
  if(forwardProfile.motionDone()){
    return true;
  }
  return false;
}

void Drivetrain::startForwardMotion(double d){
  forwardProfile.setStartPos(getDistanceOnX());
  forwardProfile.startMotion(d);
}

double Drivetrain::getRobotYaw(int type){
  double lval = leftBack.position(rotationUnits::rev) + leftFront.position(rotationUnits::rev);
  double rval = rightBack.position(rotationUnits::rev) + rightFront.position(rotationUnits::rev);
  lval /= 2.0; rval /= 2.0;
  lval *= distanceWheels; rval *= distanceWheels;
  double rads = ((lval - rval) / 2.0) / (distanceWheels / 2.0);
  rads -= resetedHeading;
  while(rads < -PI) rads += 2*PI;
  while(rads > PI)  rads -= 2*PI;
  if(type == RADIANS) return rads;
  return rads * 180.0 / PI;
}
void Drivetrain::resetHeading(double to){
  resetedHeading = getRobotYaw(RADIANS) + resetedHeading + to;
}

bool Drivetrain::setHeading(double heading, motorPowers& p) { // in deg
  double error = heading - getRobotYaw(DEGREES);
  if(abs(error) < ERROR_ON_HEADING) return true;
  double rotate = headingController.update(error);
  motorPowers ret = {0, 0};
  p.left += rotate;
  p.right -= rotate;
  return false;
}

void Drivetrain::setPower(motorPowers pow){
  leftDrive.spin(pow.left > 0 ? directionType::fwd : directionType::fwd, pow.left * voltage, volt);
  rightDrive.spin(pow.right > 0 ? directionType::fwd : directionType::fwd, pow.right * voltage, volt);

}

void Drivetrain::stopDrive() {
  leftDrive.stop();
  rightDrive.stop();
}

void Drivetrain::resetDriveRotations() {
  leftFront.setPosition(0, rev);
  leftBack.setPosition(0, rev);
  rightFront.setPosition(0, rev);
  rightBack.setPosition(0, rev);
}

void Drivetrain::update(controller con) {
  int fwd = con.Axis3.position(percent);
  if (abs(fwd) < 5) fwd = 0;

  int rotate = 0;
  if (con.ButtonR2.pressing()) {
    rotate = 100;
  } else if (con.ButtonL2.pressing()) {
    rotate = -100;
  }

  // TURN_STRENGTH: how much R2/L2 turn authority (0.0 = no turn, 1.0 = full turn)
  // At 0.5 with full forward + R2: left=100%, right=50% — balanced feel
  // Raise toward 1.0 for sharper turns; lower toward 0.3 for gentler steering
  const double TURN_STRENGTH = 0.75;

  auto clamp = [](int v) { return v > 100 ? 100 : v < -100 ? -100 : v; };
  int leftPower  = clamp(fwd + (int)(rotate * TURN_STRENGTH));
  int rightPower = clamp(fwd - (int)(rotate * TURN_STRENGTH));
  leftDrive.spin(forward,  leftPower  * voltage / 100.0, volt);
  rightDrive.spin(forward, rightPower * voltage / 100.0, volt);
}

double Drivetrain::getDistanceOnX(){
  return -x.value() / xEncoderCPR  * PI * xWheelRadius;
}
double Drivetrain::getXVelocity(){
  return (x.velocity(velocityUnits::rpm));
}
void Drivetrain::resetXEncoder(){
  x.resetRotation();
  x.setPosition(0, vex::rotationUnits::raw);
}