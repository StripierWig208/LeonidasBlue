#pragma once

class PIDController {
private:
  double kP, kI, kD;
  double integral = 0.0;
  double prevError = 0.0;

public:
  PIDController(double kP, double kI, double kD)
      : kP(kP), kI(kI), kD(kD) {}

  double update(double error, double velo = 0, bool usevelo = false) {
    integral += error;
    double derivative = error - prevError;
    if(usevelo) derivative = velo;
    prevError = error;
    return kP * error + kI * integral + kD * derivative;
  }

  void reset() {
    integral = 0.0;
    prevError = 0.0;
  }
};