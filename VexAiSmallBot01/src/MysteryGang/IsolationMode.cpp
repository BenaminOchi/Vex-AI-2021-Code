#include "JetsonData.h"
#include "MysteryGang/CommonPartMethods.h"
#include "MysteryGang/IsolationMode.h"
#include "MysteryGang/RobotConfig.h"

extern void updateJetsonDisplayLr(int lr);

static const char* sBuildDate = __DATE__;
static const char* sBuildTime = __TIME__;
static const char* sVersion = "0002";


namespace IsolationMode {

  enum StateMachineType {
    STATE_UNUSED,
    STATE_INIT,
    STATE_TRACK_FIRST_TARGET,
    STATE_BACK_UP_AND_OUTAKE,
    STATE_TRACK_FIRST_GOAL,
    STATE_DONE
  };

  StateMachineType sCurState = STATE_INIT;

  static int sLeftMotorSpeed   = 0;
  static int sRightMotorSpeed  = 0;

  static bool sIsTaskSuspended = false;


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

  void performStateBackUpAndOutake() {
    Cpm::startTopIntakesReverse();
    vex::task::sleep(500);
    Cpm::stopTopIntakes();
    Cpm::moveRobotBackward(24);
  }

  void performStateInit() {
    Cpm::moveRobotForward(24);
    vex::task::sleep(200);
    //Cpm::turnRobotLeft(135);
    Cpm::turnRobotRight(135);
  }

  void performStateTrackFirstGoal() {
    const unsigned int centerX = 320;
    const int MaxSpeed = 7;
    const int HalfSpeed = 4;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    static bool sIsTracking = true;
    static bool sIsTargetInRange = false;
    int dbgLeftRight = 3;  // Default to: pointing at target

    if (sIsTracking == true) {
      // Get latest info on the target
      getBoxData(JetsonData::GOAL, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

      if (hasTarget == false) {
    	dbgLeftRight = 0;    // no valid target
        if (sIsTargetInRange == false) {
          Cpm::coastWheels();
        }
        else {
          hwLimit.pressed(Cpm::setLimitSwitchPressed);    // TBD - Use Cpm
          hwBumper.pressed(Cpm::setBumperSwitchPressed);  // TBD - Use Cpm
          if (Cpm::wasLimitSwitchPressed() == true) {
            Cpm::stopAllMotors();
            Cpm::stopWheels();
            Cpm::stopAllIntakes();
            //vex::task::sleep(3000);
            //Cpm::clearLimitSwitchPressed();
            //vex::task::sleep(3000);
            //Cpm::startTopIntakesReverse();
            //vex::task::sleep(500);
            //Cpm::stopTopIntakes();
            sIsTracking = false;
            sCurState = STATE_DONE;
          }
          if (Cpm::wasBumperSwitchPressed() == true) {
            Cpm::stopAllMotors();
            sIsTracking = false;
          }
        }
      }
      else if (curX <= (centerX - 100 * 2)) {
        sLeftMotorSpeed = MaxSpeed * -1;
        sRightMotorSpeed = MaxSpeed;
        dbgLeftRight = 1;    // Left
      }
      else if (curX >= (centerX + 100 * 2)) {
        sLeftMotorSpeed = MaxSpeed;
        sRightMotorSpeed = MaxSpeed * -1;
        dbgLeftRight = 2;    // Right
      }
      else if (curX <= (centerX - 60 * 2)) {
        sLeftMotorSpeed = HalfSpeed *  -1;
        sLeftMotorSpeed = HalfSpeed;
        dbgLeftRight = 1;    // Left
      }
      else if (curX >= (centerX + 60 * 2)) {
        sLeftMotorSpeed = HalfSpeed;
        sRightMotorSpeed = HalfSpeed * -1;
        dbgLeftRight = 2;    // Right
      }
      else {
        // Here the robot is pointed at the target, move forward to target
        if (curDepthI >= 30) {
          sLeftMotorSpeed = 40;
          sRightMotorSpeed = 40;
        }
        else if (curDepthI >= 16) {
          sLeftMotorSpeed = 30;
          sRightMotorSpeed = 30;
        }
        else {
          sLeftMotorSpeed = 15;
          sRightMotorSpeed = 15;
          Cpm::startBottomIntakes();
          Cpm::startMiddleIntake();
          sIsTargetInRange = true;
        }
      }
      updateJetsonDisplayLr(dbgLeftRight);

      if (sIsTracking == true) {
        // Move wheels based on trackloop positions
        hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
        hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
        hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
        hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
      }
    }
  }

  void performStateTrackFirstTarget() {
    const unsigned int centerX = 320;
    const int MaxSpeed = 7;
    const int HalfSpeed = 4;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    static bool sIsTracking = true;
    static bool sIsTargetInRange = false;
    int dbgLeftRight = 3;  // Default to: pointing at target
    JetsonData::ClassIdType ourBall = JetsonData::BALL_RED;   // TBD: Need to use red/blue ball based on team

    if (sIsTracking == true) {
      // Get latest info on the target
      getBoxData(ourBall, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

      if (sIsTargetInRange == true) {  // We are within striking (acquiring) range of the target, but we can't see it
        hwLimit.pressed(Cpm::setLimitSwitchPressed);    // TBD - Use Cpm
        hwBumper.pressed(Cpm::setBumperSwitchPressed);  // TBD - Use Cpm
        if (Cpm::wasLimitSwitchPressed() == true) {
          Cpm::stopAllMotors();
          sIsTracking = false;
          sCurState = STATE_BACK_UP_AND_OUTAKE;
        }
        if (Cpm::wasBumperSwitchPressed() == true) {
          Cpm::stopAllMotors();
          sIsTracking = false;
        }
      }
      else {
      if (hasTarget == false) {
    	  dbgLeftRight = 0;    // no valid target
        Cpm::coastWheels();
      }
      else if (curX <= (centerX - 100 * 2)) {
        sLeftMotorSpeed = MaxSpeed * -1;
        sRightMotorSpeed = MaxSpeed;
        dbgLeftRight = 1;    // Left
      }
      else if (curX >= (centerX + 100 * 2)) {
        sLeftMotorSpeed = MaxSpeed;
        sRightMotorSpeed = MaxSpeed * -1;
        dbgLeftRight = 2;    // Right
      }
      else if (curX <= (centerX - 60 * 2)) {
        sLeftMotorSpeed = HalfSpeed *  -1;
        sLeftMotorSpeed = HalfSpeed;
        dbgLeftRight = 1;    // Left
      }
      else if (curX >= (centerX + 60 * 2)) {
        sLeftMotorSpeed = HalfSpeed;
        sRightMotorSpeed = HalfSpeed * -1;
        dbgLeftRight = 2;    // Right
      }
      else {
        // Here the robot is pointed at the target, move forward to target
        if (curDepthI >= 30) {
          sLeftMotorSpeed = 40;
          sRightMotorSpeed = 40;
        }
        else if (curDepthI >= 16) {
          sLeftMotorSpeed = 30;
          sRightMotorSpeed = 30;
        }
        else {
          sLeftMotorSpeed = 15;
          sRightMotorSpeed = 15;
          Cpm::startBottomIntakes();
          Cpm::startMiddleIntake();
          sIsTargetInRange = true;
        }
      }
      }
      updateJetsonDisplayLr(dbgLeftRight);

      if (sIsTracking == true) {
        // Move wheels based on trackloop positions
        hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
        hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
        hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
        hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
      }
    }
  }

  void runStateMachine() {
    switch (sCurState) {
    case STATE_INIT :
      performStateInit();
      sCurState = STATE_TRACK_FIRST_TARGET;
      break;
    case STATE_TRACK_FIRST_TARGET :
      Cpm::clearLimitSwitchPressed();
      performStateTrackFirstTarget();
      break;
    case STATE_BACK_UP_AND_OUTAKE :
      performStateBackUpAndOutake();
      sCurState = STATE_TRACK_FIRST_GOAL;
      break;
    case STATE_TRACK_FIRST_GOAL :
      performStateTrackFirstGoal();
      sCurState = STATE_DONE;
    default :
      break;
    }
  }

  //-----------------------
  // Isolation mode event loop
  //-----------------------
  int controlTask() {

    Cpm::stopAllMotors();

    // Enter the main control loop
    while (true) {
      runStateMachine();
      if (sIsTaskSuspended == true) {
        break;
      }
      vex::task::sleep(1);  // Allow detection of competition switch, etc.
    }
    return 0;
  }

  void suspendTask() {
    sIsTaskSuspended = true;
  }

}
