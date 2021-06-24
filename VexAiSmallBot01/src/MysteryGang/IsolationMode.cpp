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
    static int   sCurX           = 0;
    static int   sCurY           = 0;
    static float sCurWidthI      = 0.0;
    static float sCurHeightI     = 0.0;
    static float sCurDepthI      = 0.0;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    int dbgLeftRight = 3;  // Default to: pointing at target
    JetsonData::ClassIdType opponentBall = CurConfig::getOpponentTeamBallColor();

    // Get latest info on the target
    // TBD: Need to use red/blue appropriately based on team color
    getBoxDataBallBlueInsideGoal(opponentBall, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

    if (hasTarget) {
      sCurX          = curX;
      sCurY          = curY;
      sCurWidthI     = curWidthI;
      sCurHeightI    = curHeightI;
      sCurDepthI     = curDepthI;
    }
    else {
      dbgLeftRight = 0;    // no valid target
      // Use last position for everything except depth
      curX           = sCurX;
      curY           = sCurY;
      curWidthI      = sCurWidthI;
      curHeightI     = sCurHeightI;
      curDepthI      = 1.0;
    }

    if (curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForMaxSpeed)) {
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
      if (hasTarget) {
        // Only do this if we are operating on valid data
        sCurState = nextState;
      }
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
    static int   sCurX           = 0;
    static int   sCurY           = 0;
    static float sCurWidthI      = 0.0;
    static float sCurHeightI     = 0.0;
    static float sCurDepthI      = 0.0;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    int dbgLeftRight = 3;  // Default to: pointing at target

    // Get latest info on the target
    getBoxData(JetsonData::GOAL, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

    if (hasTarget) {
      sCurX          = curX;
      sCurY          = curY;
      sCurWidthI     = curWidthI;
      sCurHeightI    = curHeightI;
      sCurDepthI     = curDepthI;
    }
    else {
      dbgLeftRight = 0;    // no valid target
      // Use last position for everything except depth
      curX           = sCurX;
      curY           = sCurY;
      curWidthI      = sCurWidthI;
      curHeightI     = sCurHeightI;
      curDepthI      = 1.0;
    }

    if (curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForMaxSpeed)) {
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
      if (hasTarget) {
        // Only do this if we are operating on valid data
        sCurState = nextState;
      }
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
    Cpm::moveRobotForward(24);
    vex::task::sleep(200);
    Cpm::turnRobotRight(45);
    sCurState = STATE_TOWER_ONE_TARGET_GET_CENTERED;
  }

  void performStateTowerOneTargetGetCentered() {
    centerOnTarget(STATE_TOWER_ONE_TARGET_TRACK_TO_IT);
  }

  void performStateTowerOneTargetTrackToIt() {
    static int   sCurX           = 0;
    static int   sCurY           = 0;
    static float sCurWidthI      = 0.0;
    static float sCurHeightI     = 0.0;
    static float sCurDepthI      = 0.0;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    int dbgLeftRight = 3;  // Default to: pointing at target
    JetsonData::ClassIdType opponentBall = CurConfig::getOurTeamBallColor();

    // Get latest info on the target
    // TBD: Need to use red/blue appropriately based on team color
    getBoxDataBallBlueInsideGoal(opponentBall, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

    if (hasTarget) {
      sCurX          = curX;
      sCurY          = curY;
      sCurWidthI     = curWidthI;
      sCurHeightI    = curHeightI;
      sCurDepthI     = curDepthI;
    }
    else {
      dbgLeftRight = 0;    // no valid target
      // Use last position for everything except depth
      curX           = sCurX;
      curY           = sCurY;
      curWidthI      = sCurWidthI;
      curHeightI     = sCurHeightI;
      curDepthI      = 1.0;
    }

    if ((curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForLineOfSightSpeed)) || (curX >= (JetsonData::CenterX + JetsonData::CenterAdjustForLineOfSightSpeed))) {
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
          //sCurState = STATE_TOWER_ONE_TARGET_IN_RANGE;
          sCurState = STATE_DONE;   // TBD: Debug
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
      Cpm::stopAllMotors();
      sCurState = STATE_TOWER_ONE_BACKUP_AND_OUTAKE;
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

  void performStateTowerOneGoalGetCentered() {
    centerOnGoal(STATE_TOWER_ONE_GOAL_TRACK_TO_IT);
  }

  void performStateTowerOneGoalTrackToIt() {
    static int   sCurX           = 0;
    static int   sCurY           = 0;
    static float sCurWidthI      = 0.0;
    static float sCurHeightI     = 0.0;
    static float sCurDepthI      = 0.0;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    int dbgLeftRight = 3;  // Default to: pointing at target

    // Get latest info on the target
    getBoxData(JetsonData::GOAL, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

    if (hasTarget) {
      sCurX          = curX;
      sCurY          = curY;
      sCurWidthI     = curWidthI;
      sCurHeightI    = curHeightI;
      sCurDepthI     = curDepthI;
    }
    else {
      dbgLeftRight = 0;    // no valid target
      // Use last position for everything except depth
      curX           = sCurX;
      curY           = sCurY;
      curWidthI      = sCurWidthI;
      curHeightI     = sCurHeightI;
      curDepthI      = 1.0;
    }

    if ((curX <= (JetsonData::CenterX - JetsonData::CenterAdjustForLineOfSightSpeed)) || (curX >= (JetsonData::CenterX + JetsonData::CenterAdjustForLineOfSightSpeed))) {
      // Target is no longer centered, go back and center it
      Cpm::stopAllWheels();
      vex::task::sleep(100);
      sCurState = STATE_TOWER_ONE_GOAL_GET_CENTERED;
    }
    else {
      // Here the robot is pointed at the target, move forward to target
      if (curDepthI >= 30) {
        sLeftMotorSpeed = 30;
        sRightMotorSpeed = 30;
      }
      else if (curDepthI >= 16) {
        sLeftMotorSpeed = 20;
        sRightMotorSpeed = 20;
      }
      else {
        sLeftMotorSpeed = 20;
        sRightMotorSpeed = 20;
        if (hasTarget) {
          // Only do this if we are operating on valid data
          Cpm::startBottomIntakes();
          sCurState = STATE_TOWER_ONE_GOAL_IN_RANGE;
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

  void performStateTowerOneGoalInRange() {
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

    if ((Cpm::wasBumperSwitchPressed() == true) || (Cpm::wasLimitSwitchPressed() == true)) {
      // We are at the tower, shoot the balls into the goal, and hang on to the one opponent ball
      Cpm::disableLimitSwitch();
      Cpm::disableBumperSwitch();
      Cpm::stopAllWheels();
      Cpm::startAllIntakes();
      vex::task::sleep(1000);
      Cpm::stopBottomIntakes();
      vex::task::sleep(3000);
      Cpm::stopAllMotors();
      sCurState = STATE_TOWER_ONE_BACKUP_TURN_OUTAKE;
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

  void performStateTowerOneBackupAndOutake() {
    Cpm::startTopIntakesReverse();
    vex::task::sleep(500);
    Cpm::stopTopIntakes();
    Cpm::moveRobotBackward(24);
    vex::task::sleep(1000);
    sCurState = STATE_TOWER_ONE_GOAL_GET_CENTERED;
  }

  void performStateTowerOneBackupTurnOutake() {
    Cpm::moveRobotBackward(24);
    vex::task::sleep(10);
    Cpm::turnRobotRight(45);
    Cpm::startAllIntakesReverse();
    vex::task::sleep(1000);
    Cpm::stopAllIntakes();
    Cpm::turnRobotLeft(180);
    sCurState = STATE_DONE;
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
    case STATE_TOWER_ONE_BACKUP_AND_OUTAKE     :  performStateTowerOneBackupAndOutake();     break;
    case STATE_TOWER_ONE_GOAL_GET_CENTERED     :  performStateTowerOneGoalGetCentered();     break;
    case STATE_TOWER_ONE_GOAL_TRACK_TO_IT      :  performStateTowerOneGoalTrackToIt();       break;
    case STATE_TOWER_ONE_GOAL_IN_RANGE         :  performStateTowerOneGoalInRange();         break;
    case STATE_TOWER_ONE_BACKUP_TURN_OUTAKE    :  performStateTowerOneBackupTurnOutake();    break;
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
