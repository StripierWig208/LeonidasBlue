// #include "Odometry.h"
// #include <cstring>
// #include <cstdio>

// Odometry::Odometry()
//     : serial(nullptr), x(0), y(0), heading(0), bufIdx(0) {
//   memset(buf, 0, sizeof(buf));
// }

// void Odometry::init(vex::triport::port &rxPort, vex::triport::port &txPort) {
//   serial = new vex::serialPort(rxPort, txPort, 9600);
// }

// void Odometry::parseLine(char *line) {
//   if (line[0] != '$') return;
//   double nx, ny, nh;
//   if (sscanf(line + 1, "%lf,%lf,%lf", &nx, &ny, &nh) == 3) {
//     x       = nx;
//     y       = ny;
//     heading = nh;
//   }
// }

// void Odometry::update() {
//   if (!serial) return;
//   while (serial->available() > 0) {
//     char c = (char)serial->read();
//     if (c == '\n') {
//       buf[bufIdx] = '\0';
//       parseLine(buf);
//       bufIdx = 0;
//     } else if (bufIdx < 63) {
//       buf[bufIdx++] = c;
//     }
//   }
// }

// void Odometry::reset() {
//   x = 0;
//   y = 0;
//   heading = 0;
// }

// double Odometry::getX()       { return x; }
// double Odometry::getY()       { return y; }
// double Odometry::getHeading() { return heading; }
