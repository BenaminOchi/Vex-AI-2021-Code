#include "JetsonData.h"
#include "MysteryGang/CommonPartMethods.h"
#include "MysteryGang/CurConfig.h"
#include "MysteryGang/IsolationMode.h"
#include "MysteryGang/RobotConfig.h"

extern void updateJetsonDisplayLr(int lr);

static const char* sBuildDate = __DATE__;
static const char* sBuildTime = __TIME__;
static const char* sVersion = "0003";


namespace IsolationMode {

  enum StateMachineType {
    STATE_UNUSED,
    STATE_INIT,
    STATE_TOWER_ONE_TARGET_ENTRY,
    STATE_TOWER_ONE_TARGET_GET_CENTERED,
    STATE_TOWER_ONE_TARGET_TRACK_TO_IT,
    STATE_TOWER_ONE_TARGET_IN_RANGE,
    STATE_TOWER_ONE_BACKUP_AND_OUTAKE,
    STATE_TOWER_ONE_GOAL_GET_CENTERED,
    STATE_TOWER_ONE_GOAL_TRACK_TO_IT,
    STATE_TOWER_ONE_GOAL_IN_RANGE,
    STATE_TOWER_ONE_BACKUP_TURN_OUTAKE,
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

  void printStateMachineDebug() {
    static StateMachineType sLastDisplayedState = STATE_INIT;
    int lastClassId = JetsonData::getLastJetsonClassId();
  
    if (sLastDisplayedState != sCurState) {
      hwBrain.Screen.print("  SM-%d  CI-%d", sCurState, lastClassId);
      hwBrain.Screen.newLine();
      sLastDisplayedState = sCurState;
    }
  }

  void printDebug(const char* dbgStr) {
    hwBrain.Screen.print(dbgStr);
    hwBrain.Screen.newLine();
  }




  //-----------------------------------------------------------------------------
  //--- Common routines
  //-----------------------------------------------------------------------------
  void centerOnTarget(StateMachineType nextState) {
    const int MaxSpeed   = 7;
    const int HalfSpeed  = 4;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    int dbgLeftRight = 3;  // Default to: pointing at target

    // Get latest info on the target
    JetsonData::getBoxDataOpponentTeamBall(JetsonData::BRTGT_DONT_CARE, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

    if (hasTarget == false) {
      dbgLeftRight = 0;    // no valid target
      Cpm::coastAllWheels();
    }
    else if (curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForMaxSpeed)) {
      sLeftMotorSpeed = MaxSpeed * -1;
      sRightMotorSpeed = MaxSpeed;
      dbgLeftRight = 1;    // Left
    }
    else if (curX >= (JetsonData::CenterX + JetsonData::CenterAdjustForMaxSpeed)) {
      sLeftMotorSpeed = MaxSpeed;
      sRightMotorSpeed = MaxSpeed * -1;
      dbgLeftRight = 2;    // Right
    }
    else if (curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForHalfSpeed)) {
      sLeftMotorSpeed = HalfSpeed *  -1;
      sLeftMotorSpeed = HalfSpeed;
      dbgLeftRight = 1;    // Left
    }
    else if (curX >= (JetsonData::CenterX + JetsonData::CenterAdjustForHalfSpeed)) {
      sLeftMotorSpeed = HalfSpeed;
      sRightMotorSpeed = HalfSpeed * -1;
      dbgLeftRight = 2;    // Right
    }
    else if (curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForLineOfSightSpeed)) {
      sLeftMotorSpeed = 2 *  -1;
      sLeftMotorSpeed = 2;
      dbgLeftRight = 1;    // Left
    }
    else if (curX >= (JetsonData::CenterX + JetsonData::CenterAdjustForLineOfSightSpeed)) {
      sLeftMotorSpeed = 2;
      sRightMotorSpeed = 2 * -1;
      dbgLeftRight = 2;    // Right
    }
    else {  // Target is centered in the view
      sCurState = nextState;
    }
    updateJetsonDisplayLr(dbgLeftRight);

    // Move wheels based on trackloop positions
    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
  }

  void centerOnGoal(StateMachineType nextState) {
    const int MaxSpeed   = 7;
    const int HalfSpeed  = 4;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    int dbgLeftRight = 3;  // Default to: pointing at target

    // Get latest info on the target
    JetsonData::getBoxDataGoal(JetsonData::GOAL, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

    if (hasTarget == false) {
      dbgLeftRight = 0;    // no valid target
      Cpm::coastAllWheels();
    }
    else if (curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForMaxSpeed)) {
      sLeftMotorSpeed = MaxSpeed * -1;
      sRightMotorSpeed = MaxSpeed;
      dbgLeftRight = 1;    // Left
    }
    else if (curX >= (JetsonData::CenterX + JetsonData::CenterAdjustForMaxSpeed)) {
      sLeftMotorSpeed = MaxSpeed;
      sRightMotorSpeed = MaxSpeed * -1;
      dbgLeftRight = 2;    // Right
    }
    else if (curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForHalfSpeed)) {
      sLeftMotorSpeed = HalfSpeed *  -1;
      sLeftMotorSpeed = HalfSpeed;
      dbgLeftRight = 1;    // Left
    }
    else if (curX >= (JetsonData::CenterX + JetsonData::CenterAdjustForHalfSpeed)) {
      sLeftMotorSpeed = HalfSpeed;
      sRightMotorSpeed = HalfSpeed * -1;
      dbgLeftRight = 2;    // Right
    }
    else if (curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForLineOfSightSpeed)) {
      sLeftMotorSpeed = 2 *  -1;
      sLeftMotorSpeed = 2;
      dbgLeftRight = 1;    // Left
    }
    else if (curX >= (JetsonData::CenterX + JetsonData::CenterAdjustForLineOfSightSpeed)) {
      sLeftMotorSpeed = 2;
      sRightMotorSpeed = 2 * -1;
      dbgLeftRight = 2;    // Right
    }
    else {  // Target is centered in the view
      sCurState = nextState;
    }
    updateJetsonDisplayLr(dbgLeftRight);

    // Move wheels based on trackloop positions
    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
  }

  //-----------------------------------------------------------------------------
  //--- State Machine Init
  //-----------------------------------------------------------------------------
  void performStateInit() {
    // Nothing to do here for now
    sCurState = STATE_TOWER_ONE_TARGET_ENTRY;
  }




  //-----------------------------------------------------------------------------
  //--- Tower One
  //-----------------------------------------------------------------------------
  void performStateTowerOneTargetEntry() {
    Cpm::moveRobotForward(60);
    vex::task::sleep(200);
    Cpm::turnRobotRight(80);
    sCurState = STATE_TOWER_ONE_TARGET_GET_CENTERED;
  }

  void performStateTowerOneTargetGetCentered() {
    centerOnTarget(STATE_TOWER_ONE_TARGET_TRACK_TO_IT);
  }

  void performStateTowerOneTargetTrackToIt() {
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    int dbgLeftRight = 3;  // Default to: pointing at target

    // Get latest info on the target
    JetsonData::getBoxDataOpponentTeamBall(JetsonData::BRTGT_DONT_CARE, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

    if (hasTarget == false) {
      dbgLeftRight = 0;    // no valid target
      Cpm::coastAllWheels();
    }
    else if ((curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForLineOfSightSpeed)) || (curX >= (JetsonData::CenterX + JetsonData::CenterAdjustForLineOfSightSpeed))) {
      // Target is no longer centered, go back and center it
      Cpm::stopAllWheels();
      vex::task::sleep(100);
      sCurState = STATE_TOWER_ONE_TARGET_GET_CENTERED;
    }
    else {  // Here the robot is pointed at the target, move forward to target
      if (curDepthI >= 30) {
        sLeftMotorSpeed = 40;
        sRightMotorSpeed = 40;
      }
      else if (curDepthI >= 16) {
        sLeftMotorSpeed = 30;
        sRightMotorSpeed = 30;
      }
      else {
        sLeftMotorSpeed = 20;
        sRightMotorSpeed = 20;
        if (hasTarget) {
          // Only do this if we are operating on valid data
          Cpm::startBottomIntakes();
          sCurState = STATE_TOWER_ONE_TARGET_IN_RANGE;
        }
      }
    }
    updateJetsonDisplayLr(dbgLeftRight);

    // Move wheels based on trackloop positions
    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
  }

  void performStateTowerOneTargetInRange() {
    static bool sMoveForward = true;
    static bool sRanOnce = false;

    if (sRanOnce == false) {
      Cpm::disableLimitSwitch();
      Cpm::disableBumperSwitch();
      sRanOnce = true;
    }

    if (sMoveForward == true) {
      Cpm::enableLimitSwitch();
      Cpm::enableBumperSwitch();
      sMoveForward = false;
    }

    if ((Cpm::wasLimitSwitchPressed() == true) || (Cpm::wasBumperSwitchPressed() == true)) {
      // First ball was acquired
      Cpm::moveRobotBackward(3);
      Cpm::stopAllWheels();
      vex::task::sleep(3000);
      Cpm::stopAllIntakes();
      sCurState = STATE_DONE;
    }
    else {
      if (sMoveForward == true) {
        // Move wheels based on trackloop positions
        hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
        hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
        hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
        hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
      }
    }
  }



  //-----------------------------------------------------------------------------
  //--- Done with Isolation
  //-----------------------------------------------------------------------------
  void performStateDone() {
    Cpm::disableLimitSwitch();
    Cpm::disableBumperSwitch();
    Cpm::stopAllMotors();
  }



  //-----------------------------------------------------------------------------
  //--- State Machine
  //-----------------------------------------------------------------------------
  void runStateMachine() {
    printStateMachineDebug();

    switch (sCurState) {
    case STATE_INIT                            :  performStateInit();                        break;
    case STATE_TOWER_ONE_TARGET_ENTRY          :  performStateTowerOneTargetEntry();         break;
    case STATE_TOWER_ONE_TARGET_GET_CENTERED   :  performStateTowerOneTargetGetCentered();   break;
    case STATE_TOWER_ONE_TARGET_TRACK_TO_IT    :  performStateTowerOneTargetTrackToIt();     break;
    case STATE_TOWER_ONE_TARGET_IN_RANGE       :  performStateTowerOneTargetInRange();       break;
    case STATE_DONE                            :  performStateDone();                        break;
    default                                    :                                             break;
    }
  }


  //-----------------------------------------------------------------------------
  //--- Isolation mode event loop
  //-----------------------------------------------------------------------------
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
