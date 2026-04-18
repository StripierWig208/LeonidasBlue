#pragma once
#include "vex.h"
#include "PIDController.h"
#include "MotionProfile.h"
#define DEGREES 0
#define RADIANS 1
#define PI 3.141592653589

struct motorPowers{
  double left, right;
};

using namespace vex;
const static double ERROR_ON_X = 5; // in mm
const static double ERROR_ON_HEADING = 7; // in deg



class Drivetrain {
private:

  PIDController headingController, forwardController;
  double voltage = 0;
  bool FollowingLine = false;

  double speedMultiplier = 1.0;

  double currentLeftVoltage = 0.0;
  double currentRightVoltage = 0.0;

  double resetedHeading = 0;

public:
  motor leftFront;
  motor leftBack;
  motor rightFront;
  motor rightBack;
  motor_group leftDrive;
  motor_group rightDrive;
  MotionProfile forwardProfile;

  Drivetrain();

  void update(controller con);
  void setSpeedMultiplier(double multiplier);
  double getDistanceOnX();
  void stopDrive();
  void setPower(motorPowers);
  void resetHeading(double = 0);

  double degToTurnRevolutions(double angleDeg);
  double getRobotYaw(int type); // 0 - deg, 1 - rads
  double getXVelocity();

  bool setHeading(double heading, motorPowers&); // in deg
  bool AutonomousDrive(motorPowers&);
  void startForwardMotion(double distanceMM);

  void resetDriveRotations();
  void resetXEncoder();

};