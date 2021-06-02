#include "MysteryGang/CommonPartMethods.h"
#include "MysteryGang/ManualMode.h"
#include "MysteryGang/RobotConfig.h"
#include "Isolation.h"

extern void setJetsonDisplay(int lr);
extern void getBoxData(bool* hasTarget, int* classId, int* x, int* y, float* widthI, float* heightI, float* depthI);

static bool sHasTarget   = false;
static int   sCurClassId = 0;
static int   sCurX       = 0;
static int   sCurY       = 0;
static float sCurWidthI  = 0.0;
static float sCurHeightI = 0.0;
static float sCurDepthI  = 0.0;

static int sLeftMotorSpeed = 0;
static int sRightMotorSpeed = 0;


namespace IsolationMode {

  void setWheelSpeeds() {
    const int MaxSpeed = 15;  //TBD: 150;
    const int HalfSpeed = 7;  //TBD: 75;
    int targetDistance = sCurDepthI;
    int leftRight = 3;  // Default to: pointing at target

    if (sHasTarget == false) {
      sLeftMotorSpeed = 0;
      sRightMotorSpeed = 0;
      leftRight = 0;    // no valid target
    }
    else if (sCurX <= 130) {
      sLeftMotorSpeed = MaxSpeed;
      sRightMotorSpeed = MaxSpeed * -1;
      leftRight = 1;    // Left
    }
    else if (sCurX >= 190) {
      sLeftMotorSpeed = MaxSpeed * -1;
      sRightMotorSpeed = MaxSpeed;
      leftRight = 2;    // Right
    }
    else if (sCurX <= 155) {
      sLeftMotorSpeed = HalfSpeed;
      sLeftMotorSpeed = HalfSpeed * -1;
      leftRight = 1;    // Left
    }
    else if (sCurX >= 165) {
      sLeftMotorSpeed = HalfSpeed * -1;
      sRightMotorSpeed = HalfSpeed;
      leftRight = 2;    // Right
    }
    else {  // We are pretty much pointing at the target
      // Here the robot is pointed at the target, move forward to target
      if (targetDistance >= 30) {
        sLeftMotorSpeed = MaxSpeed;
        sRightMotorSpeed = MaxSpeed;
      }
      else if (targetDistance >= 16) {
        sLeftMotorSpeed = HalfSpeed;
        sRightMotorSpeed = HalfSpeed;
      }
      else {
        sLeftMotorSpeed = 0;
        sRightMotorSpeed = 0;
      }
    }
    setJetsonDisplay(leftRight);
  }

  //-----------------------
  // Isolation mode event loop
  //-----------------------
  int isolationModeTask() {
    
    Cpm::stopAllMotors();

    // Enter the main control loop
    while (true) {
      // Get latest info on the target
      getBoxData(&sHasTarget, &sCurClassId, &sCurX, &sCurY, &sCurWidthI, &sCurHeightI, &sCurDepthI);

      setWheelSpeeds();

      // Move wheels based on trackloop positions
      hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct); 
      hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
      hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
      hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);

      // Short delay just in case we want to display something on the controller
      vex::task::sleep(1);
    }
  }
}
