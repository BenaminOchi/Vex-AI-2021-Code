#include "MysteryGang/CommonPartMethods.h"
#include "MysteryGang/RobotConfig.h"

namespace Cpm {
  // Set cube pull in speeds
  static int sIntakeRightMotorSpeed = 0;
  static int sIntakeLeftMotorSpeed = 0;

  void intakeCubes(unsigned int numMillisecs) {
    sIntakeLeftMotorSpeed = 150;
    sIntakeRightMotorSpeed = sIntakeLeftMotorSpeed;
    hwMotorIntakeLeft.spin(vex::directionType::rev, sIntakeLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeRight.spin(vex::directionType::fwd, sIntakeRightMotorSpeed, vex::velocityUnits::pct);
    vex::task::sleep(numMillisecs);

    stopCubeTreads();
  }

  void movePusherBackward(int rotationsInDegrees) {
    // Move pusher backward
    hwEncoderPusher.setPosition(0, degrees);
    hwMotorPusher.spin(reverse);
    while (hwEncoderPusher.position(degrees) > rotationsInDegrees) {
    } 
    stopPusher();
  }

  void movePusherForward(int rotationsInDegrees) {
    // Move pusher forward
    hwEncoderPusher.setPosition(0, degrees);
    hwMotorPusher.spin(forward);
    while (hwEncoderPusher.position(degrees) < rotationsInDegrees) {
    } 
    stopPusher();
  }

  void moveRobotBackward(int rotationsInDegrees) {
    // Move backward
    hwEncoderWheels.setPosition(0, degrees);
    hwMotorWheelBackLeft.spin(reverse);
    hwMotorWheelBackRight.spin(reverse);
    hwMotorWheelFrontLeft.spin(reverse);
    hwMotorWheelFrontRight.spin(reverse);
    while (hwEncoderWheels.position(degrees) > rotationsInDegrees) {
    }
    stopWheels();
  }

  void moveRobotBackwardAndIntake(int rotationsInDegrees) {
    // Move backward and intake cubes
    hwEncoderWheels.setPosition(0, degrees);
    hwMotorWheelBackLeft.spin(reverse);
    hwMotorWheelBackRight.spin(reverse);
    hwMotorWheelFrontLeft.spin(reverse);
    hwMotorWheelFrontRight.spin(reverse);
    hwMotorIntakeLeft.spin(forward);
    hwMotorIntakeRight.spin(forward);
    while (hwEncoderWheels.position(degrees) > rotationsInDegrees) {
    }
    stopWheels();
    stopCubeTreads();
  }


  void moveRobotBackwardAndOutake(unsigned int numMillisecs) {
    int leftMotorSpeed;
    int rightMotorSpeed;

    leftMotorSpeed = -50;
    rightMotorSpeed = -50;
    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, leftMotorSpeed, vex::velocityUnits::pct); 
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, rightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, leftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, rightMotorSpeed, vex::velocityUnits::pct);

    sIntakeLeftMotorSpeed = -150;
    sIntakeRightMotorSpeed = sIntakeLeftMotorSpeed;
    hwMotorIntakeLeft.spin(vex::directionType::fwd, sIntakeLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorIntakeRight.spin(vex::directionType::fwd, sIntakeRightMotorSpeed, vex::velocityUnits::pct);
    vex::task::sleep(numMillisecs);

    stopWheels();
    stopCubeTreads();
  }

  void moveRobotForwardEncoders(int rotationsInDegrees) {
    // Move forward
    hwEncoderWheels.setPosition(0, degrees);
    hwMotorWheelBackLeft.spin(forward);
    hwMotorWheelBackRight.spin(forward);
    hwMotorWheelFrontLeft.spin(forward);
    hwMotorWheelFrontRight.spin(forward);
    while (hwEncoderWheels.position(degrees) < rotationsInDegrees) {
    }

    stopWheels();
  }

  void moveRobotForwardAndIntake(int rotationsInDegrees) {
    // Move forward and intake cubes
    hwEncoderWheels.setPosition(0, degrees);
    hwMotorWheelBackLeft.spin(forward);
    hwMotorWheelBackRight.spin(forward);
    hwMotorWheelFrontLeft.spin(forward);
    hwMotorWheelFrontRight.spin(forward);
    hwMotorIntakeLeft.spin(forward);
    hwMotorIntakeRight.spin(forward);
    while (hwEncoderWheels.position(degrees) < rotationsInDegrees) {
    }
    stopWheels();
    stopCubeTreads();
  }

  void outakeCubes(int rotationsInDegrees) {
    hwMotorIntakeLeft.setVelocity(-150, percent);
    hwMotorIntakeRight.setVelocity(-150, percent);
    hwMotorIntakeLeft.rotateFor(rotationsInDegrees, rotationUnits::deg, false);
    hwMotorIntakeRight.rotateFor(rotationsInDegrees, rotationUnits::deg);

    stopCubeTreads();
  }

  void setPusherSpeed(int pusherSpeedPercent) {
    hwMotorPusher.setVelocity(pusherSpeedPercent, percent);
  }

  void setWheelAndIntakeSpeeds(int wheelSpeedPercent, int intakeSpeedPercent) {
    hwMotorWheelBackLeft.setVelocity(wheelSpeedPercent, percent);
    hwMotorWheelBackRight.setVelocity(wheelSpeedPercent, percent);
    hwMotorWheelFrontLeft.setVelocity(wheelSpeedPercent, percent);
    hwMotorWheelFrontRight.setVelocity(wheelSpeedPercent, percent);
    hwMotorIntakeLeft.setVelocity(intakeSpeedPercent, percent);
    hwMotorIntakeRight.setVelocity(intakeSpeedPercent, percent);
  }

  void stopAllMotors() {
    stopWheels();
    stopAllIntakes();
  }
  
  void stopCubeTreads() {
    hwMotorIntakeLeft.stop();
    hwMotorIntakeRight.stop();
  }

  void stopPusher() {
    hwMotorPusher.stop();
  }

  void stopWheels() {
    hwMotorWheelFrontLeft.stop();
    hwMotorWheelFrontRight.stop();
    hwMotorWheelBackLeft.stop();
    hwMotorWheelBackRight.stop();
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
    double numMillisecs = numDegrees * 30;
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

  static bool sLimitSwitchPressed = false;
  bool wasLimitSwitchPressed() {
    return sLimitSwitchPressed;
  }

  void setLimitSwitchPressed() {
    sLimitSwitchPressed = true;
  }

  void clearLimitSwitchPressed() {
    sLimitSwitchPressed = false;
  }

  static bool sBumperSwitchPressed = false;
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
