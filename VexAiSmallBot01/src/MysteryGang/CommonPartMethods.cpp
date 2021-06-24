#include "MysteryGang/CommonPartMethods.h"
#include "MysteryGang/RobotConfig.h"


namespace Cpm {
  static bool sLimitSwitchPressed = false;
  static bool sBumperSwitchPressed = false;

  void stopAllMotors() {
    stopAllWheels();
    stopAllIntakes();
  }
  
  void stopAllWheels() {
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
    
    stopAllWheels();
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
    
    stopAllWheels();
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
    const unsigned int speedMultiplier = 2;
    //hwMotorWheelFrontLeft.resetPosition();
    //hwMotorWheelBackLeft.resetPosition();

    double numMillisecs = numDegrees * 23.43 / speedMultiplier;
    int leftMotorSpeed = 10 * speedMultiplier;
    int rightMotorSpeed = 10 * speedMultiplier;
    hwMotorWheelFrontLeft.spin(vex::directionType::rev, leftMotorSpeed, vex::velocityUnits::pct); 
    hwMotorWheelBackLeft.spin(vex::directionType::rev, leftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, rightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, rightMotorSpeed, vex::velocityUnits::pct);
    vex::task::sleep(numMillisecs);

    //while ((hwMotorWheelFrontLeft.position(degrees) < rotationsInDegrees) || (hwMotorWheelBackLeft.position(degrees) < rotationsInDegrees)) {
    //}
    stopAllWheels();
  }

  void turnRobotRight(unsigned int numDegrees) {
    const unsigned int speedMultiplier = 2;
    double numMillisecs = numDegrees * 23.43 / speedMultiplier;
    int leftMotorSpeed = 10 * speedMultiplier;
    int rightMotorSpeed = 10 * speedMultiplier;
    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, leftMotorSpeed, vex::velocityUnits::pct); 
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, leftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::rev, rightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::rev, rightMotorSpeed, vex::velocityUnits::pct);
    vex::task::sleep(numMillisecs);

    stopAllWheels();
  }

  void startAllIntakes() {
    int motorSpeed = 150;
    hwMotorIntakeLeft.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeRight.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeShooter.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeMiddle.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
  }

  void stopAllIntakes() {
    hwMotorIntakeLeft.stop();
    hwMotorIntakeRight.stop();
    hwMotorIntakeShooter.stop();
    hwMotorIntakeMiddle.stop();
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
    hwMotorIntakeMiddle.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);    
  }

  void stopMiddleIntake() {
    hwMotorIntakeMiddle.stop();
  }

  void startTopIntakes() {
    int motorSpeed = 150;
    hwMotorIntakeShooter.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);    
    hwMotorIntakeMiddle.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
  }

  void stopTopIntakes() {
    hwMotorIntakeShooter.stop();
    hwMotorIntakeMiddle.stop();
  }

  void startAllIntakesReverse() {
    int motorSpeed = 150;
    hwMotorIntakeLeft.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeRight.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeShooter.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeMiddle.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
  }

  void startBottomIntakesReverse() {
    int motorSpeed = 150;
    hwMotorIntakeLeft.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeRight.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
  }

  void startMiddleIntakeReverse() {
    int motorSpeed = 150;
    hwMotorIntakeMiddle.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);    
  }

  void startTopIntakesReverse() {
    int motorSpeed = 150;
    hwMotorIntakeShooter.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);    
    hwMotorIntakeMiddle.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
  }

  bool wasLimitSwitchPressed() {
    return sLimitSwitchPressed;
  }

  void setLimitSwitchPressed() {
    sLimitSwitchPressed = true;
  }

  void dummyLimitSwitchPressed() {
    // Do nothing
  }

  void disableLimitSwitch() {
    hwLimit.pressed(Cpm::dummyLimitSwitchPressed);
    sLimitSwitchPressed = false;
  }

  void enableLimitSwitch() {
    hwLimit.pressed(Cpm::setLimitSwitchPressed);
  }

  bool wasBumperSwitchPressed() {
    return sBumperSwitchPressed;
  }

  void setBumperSwitchPressed() {
    sBumperSwitchPressed = true;
  }

  void dummyBumperSwitchPressed() {
    // Do nothing
  }

  void disableBumperSwitch() {
    hwBumper.pressed(Cpm::dummyBumperSwitchPressed);
    sBumperSwitchPressed = false;
  }

  void enableBumperSwitch() {
    hwBumper.pressed(Cpm::setBumperSwitchPressed);
  }

  void coastAllWheels() {
    hwMotorWheelFrontLeft.setStopping(vex::brakeType::coast);
    hwMotorWheelBackLeft.setStopping(vex::brakeType::coast);
    hwMotorWheelFrontRight.setStopping(vex::brakeType::coast);
    hwMotorWheelBackRight.setStopping(vex::brakeType::coast);
  }

  void stopWithCoast(int numMillisecs) {
    coastAllWheels();
    vex::task::sleep(numMillisecs);
    stopAllWheels();
  }
}
