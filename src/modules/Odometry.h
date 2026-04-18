// #pragma once
// #include "vex.h"

// using namespace vex;

// class Odometry {
// public:
//   Odometry();
//   void init(vex::triport::port &rxPort, vex::triport::port &txPort);
//   void update();
//   void reset();

//   double getX();       // mm
//   double getY();       // mm
//   double getHeading(); // degrees

// private:
//   vex::serialPort *serial;
//   double x, y, heading;
//   char   buf[64];
//   int    bufIdx;

//   void parseLine(char *line);
// };
