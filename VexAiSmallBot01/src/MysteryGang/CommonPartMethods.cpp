#include "MysteryGang/CommonPartMethods.h"
#include "MysteryGang/RobotConfig.h"


namespace Cpm {
  static bool sLimitSwitchPressed = false;
  static bool sBumperSwitchPressed = false;

  void stopAllMotors() {
    stopWheels();
    stopAllIntakes();
  }
  
  void stopWheels() {
    hwMotorWheelFrontLeft.stop();
    hwMotorWheelFrontRight.stop();
    hwMotorWheelBackLeft.stop();
    hwMotorWheelBackRight.stop();
  }

  void moveRobotBackward(unsigned int numInches) {
    // Move forward
    int numMillisecs = numInches *  50;
    int leftMotorSpeed;
    int rightMotorSpeed;

    leftMotorSpeed = 40;
    rightMotorSpeed = 40;
    hwMotorWheelFrontLeft.spin(vex::directionType::rev, leftMotorSpeed, vex::velocityUnits::pct); 
    hwMotorWheelBackLeft.spin(vex::directionType::rev, rightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::rev, leftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::rev, rightMotorSpeed, vex::velocityUnits::pct);
    vex::task::sleep(numMillisecs);
    
    stopWheels();
  }

  void moveRobotForward(unsigned int numInches) {
    // Move forward
    int numMillisecs = numInches *  50;
    int motorSpeed = 40;

    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct); 
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    vex::task::sleep(numMillisecs);
    
    stopWheels();
  }

  void startWheels() {
    // Move forward
    int motorSpeed = 40;

    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct); 
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
  }

  void turnRobotLeft(unsigned int numDegrees) {
    // Turn left
    //hwMotorWheelFrontLeft.resetPosition();
    //hwMotorWheelBackLeft.resetPosition();

    double numMillisecs = numDegrees * 29.63;
    int leftMotorSpeed = 10;
    int rightMotorSpeed = 10;
    hwMotorWheelFrontLeft.spin(vex::directionType::rev, leftMotorSpeed, vex::velocityUnits::pct); 
    hwMotorWheelBackLeft.spin(vex::directionType::rev, leftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, rightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, rightMotorSpeed, vex::velocityUnits::pct);
    vex::task::sleep(numMillisecs);

    //while ((hwMotorWheelFrontLeft.position(degrees) < rotationsInDegrees) || (hwMotorWheelBackLeft.position(degrees) < rotationsInDegrees)) {
    //}
    stopWheels();
  }

  void turnRobotRight(unsigned int numDegrees) {
    // Turn right
    double numMillisecs = numDegrees * 29.63;
    int leftMotorSpeed = 10;
    int rightMotorSpeed = 10;
    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, leftMotorSpeed, vex::velocityUnits::pct); 
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, leftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::rev, rightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::rev, rightMotorSpeed, vex::velocityUnits::pct);
    vex::task::sleep(numMillisecs);

    stopWheels();
  }

  void startAllIntakes() {
    int motorSpeed = 150;
    hwMotorIntakeLeft.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeRight.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeLifter.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
    hwMotorPusher.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
  }

  void stopAllIntakes() {
    hwMotorIntakeLeft.stop();
    hwMotorIntakeRight.stop();
    hwMotorIntakeLifter.stop();
    hwMotorPusher.stop();
  }

  void startBottomIntakes() {
    int motorSpeed = 150;
    hwMotorIntakeLeft.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeRight.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
  }

  void stopBottomIntakes() {
    hwMotorIntakeLeft.stop();
    hwMotorIntakeRight.stop();
  }

  void startMiddleIntake() {
    int motorSpeed = 150;
    hwMotorPusher.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);    
  }

  void stopMiddleIntake() {
    hwMotorPusher.stop();
  }

  void startTopIntakes() {
    int motorSpeed = 150;
    hwMotorIntakeLifter.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);    
    hwMotorPusher.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
  }

  void stopTopIntakes() {
    hwMotorIntakeLifter.stop();
    hwMotorPusher.stop();
  }

  void startAllIntakesReverse() {
    int motorSpeed = 150;
    hwMotorIntakeLeft.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeRight.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeLifter.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorPusher.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
  }

  void startBottomIntakesReverse() {
    int motorSpeed = 150;
    hwMotorIntakeLeft.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeRight.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
  }

  void startMiddleIntakeReverse() {
    int motorSpeed = 150;
    hwMotorPusher.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);    
  }

  void startTopIntakesReverse() {
    int motorSpeed = 150;
    hwMotorIntakeLifter.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);    
    hwMotorPusher.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
  }

  bool wasLimitSwitchPressed() {
    return sLimitSwitchPressed;
  }

  void setLimitSwitchPressed() {
    sLimitSwitchPressed = true;
  }

  void clearLimitSwitchPressed() {
    sLimitSwitchPressed = false;
  }

  bool wasBumperSwitchPressed() {
    return sBumperSwitchPressed;
  }

  void setBumperSwitchPressed() {
    sBumperSwitchPressed = true;
  }

  void clearBumperSwitchPressed() {
    sBumperSwitchPressed = false;
  }

  void coastWheels() {
    hwMotorWheelFrontLeft.setStopping(vex::brakeType::coast);
    hwMotorWheelBackLeft.setStopping(vex::brakeType::coast);
    hwMotorWheelFrontRight.setStopping(vex::brakeType::coast);
    hwMotorWheelBackRight.setStopping(vex::brakeType::coast);
  }
}
