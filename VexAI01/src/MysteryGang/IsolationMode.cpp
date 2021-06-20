#include "MysteryGang/CommonPartMethods.h"
#include "MysteryGang/IsolationMode.h"
#include "MysteryGang/RobotConfig.h"

extern void setJetsonDisplay(int lr);
extern void getBoxData(bool* hasTarget, int* classId, int* x, int* y, float* widthI, float* heightI, float* depthI);

static const char* sBuildDate = __DATE__;
static const char* sBuildTime = __TIME__;
static const char* sVersion = "0001";

static bool sHasTarget       = false;
static int   sCurClassId     = 0;
static int   sCurX           = 0;
static int   sCurY           = 0;
static float sCurWidthI      = 0.0;
static float sCurHeightI     = 0.0;
static float sCurDepthI      = 0.0;

static int sLeftMotorSpeed   = 0;
static int sRightMotorSpeed  = 0;

static bool sIsTaskSuspended = false;
static bool sIsTracking = true;

namespace IsolationMode {

  void enterMain() {
    thread tIsol(IsolationMode::controlTask);
  }

  const char* getBuildDate() {
    return sBuildDate;
  }

  const char* getBuildTime() {
	return sBuildTime;
  }

  const char* getVersion() {
	return sVersion;
  }

  void setWheelSpeeds() {
    const unsigned int centerX = 320;
    const int MaxSpeed = 7;  //TBD: 150;
    const int HalfSpeed = 4;  //TBD: 75;
    int targetDistance = sCurDepthI;
    int leftRight = 3;  // Default to: pointing at target
    static bool sIsTargetInRange = false;

    if (sHasTarget == false) {
      leftRight = 0;    // no valid target
      if (sIsTargetInRange == false) {
        Cpm::coastWheels();
      }
      else {
        LimitSwitchD.pressed(Cpm::setLimitSwitchPressed);
        BumperC.pressed(Cpm::setBumperSwitchPressed);
        if (Cpm::wasLimitSwitchPressed() == true) {
          Cpm::stopAllMotors();
          sIsTracking = false;
        }
        if (Cpm::wasBumperSwitchPressed() == true) {
          Cpm::stopAllMotors();
          sIsTracking = false;
        }
      }
    }
    //else if (sCurX <= 130) {
    else if (sCurX <= (centerX - 100 * 2)) {
      sLeftMotorSpeed = MaxSpeed * -1;
      sRightMotorSpeed = MaxSpeed;
      leftRight = 1;    // Left
    }
    //else if (sCurX >= 190) {
    else if (sCurX >= (centerX + 100 * 2)) {
      sLeftMotorSpeed = MaxSpeed;
      sRightMotorSpeed = MaxSpeed * -1;
      leftRight = 2;    // Right
    }
    //else if (sCurX <= 155) {
    else if (sCurX <= (centerX - 60 * 2)) {
      sLeftMotorSpeed = HalfSpeed *  -1;
      sLeftMotorSpeed = HalfSpeed;
      leftRight = 1;    // Left
    }
    //else if (sCurX >= 165) {
    else if (sCurX >= (centerX + 60 * 2)) {
      sLeftMotorSpeed = HalfSpeed;
      sRightMotorSpeed = HalfSpeed * -1;
      leftRight = 2;    // Right
    }
    else {  // We are pretty much pointing at the target
      // Here the robot is pointed at the target, move forward to target
      if (targetDistance >= 30) {
        sLeftMotorSpeed = 40;
        sRightMotorSpeed = 40;
      }
      else if (targetDistance >= 16) {
        sLeftMotorSpeed = 30;
        sRightMotorSpeed = 30;
      }
      //else if (targetDistance >= 1) {
      else {
        sLeftMotorSpeed = 15;
        sRightMotorSpeed = 15;
        Cpm::startBottomIntakes();
        Cpm::startMiddleIntake();
        sIsTargetInRange = true;
      }
    }
    setJetsonDisplay(leftRight);
  }

  //-----------------------
  // Isolation mode event loop
  //-----------------------
  int controlTask() {

    Cpm::stopAllMotors();

    Cpm::moveRobotForward(24);
    vex::task::sleep(200);
    Cpm::turnRobotLeft(135);

    // Enter the main control loop
    while (true) {
      if (sIsTracking == true) {
        // Get latest info on the target
        getBoxData(&sHasTarget, &sCurClassId, &sCurX, &sCurY, &sCurWidthI, &sCurHeightI, &sCurDepthI);

        setWheelSpeeds();

        if (sIsTracking == true) {
          // Move wheels based on trackloop positions
          hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
          hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
          hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
          hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
        }
      }

      if (sIsTaskSuspended == true) {
        break;
      }
    }
    return 0;
  }

  void suspendTask() {
    sIsTaskSuspended = true;
  }

  void testCpm() {
    Cpm::stopAllMotors();

    Cpm::moveRobotForward(24);
    vex::task::sleep(200);
    Cpm::turnRobotLeft(135);
    vex::task::sleep(200);
    Cpm::moveRobotForward(500);
    vex::task::sleep(200);
    Cpm::moveRobotBackward(500);
    vex::task::sleep(200);
    Cpm::turnRobotLeft(2000);
    vex::task::sleep(200);
    Cpm::turnRobotRight(2000);
    vex::task::sleep(200);
    Cpm::startAllIntakes();
    vex::task::sleep(500);
    Cpm::stopAllIntakes();
    vex::task::sleep(1000);
    Cpm::startBottomIntakes();
    vex::task::sleep(500);
    Cpm::stopBottomIntakes();
    vex::task::sleep(1000);
    Cpm::startMiddleIntake();
    vex::task::sleep(500);
    Cpm::stopMiddleIntake();
    vex::task::sleep(1000);
    Cpm::startTopIntakes();
    vex::task::sleep(500);
    Cpm::stopTopIntakes();
  }
}
