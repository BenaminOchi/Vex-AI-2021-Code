#include "JetsonData.h"
#include "MysteryGang/CommonPartMethods.h"
#include "MysteryGang/CurConfig.h"
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
    STATE_TRACK_FIRST_TARGET_IN_RANGE,
    STATE_BACK_UP_AND_OUTAKE,
    STATE_TRACK_FIRST_GOAL,
    STATE_TRACK_FIRST_GOAL_IN_RANGE,
    STATE_BACK_UP_AND_TURN,
    STATE_TRACK_SECOND_GOAL,
    STATE_TRACK_SECOND_GOAL_IN_RANGE,
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
    vex::task::sleep(1000);
  }

  void performStateBackUpAndTurn() {
    Cpm::moveRobotBackward(24);
    vex::task::sleep(10);
    Cpm::turnRobotRight(45);
    Cpm::startAllIntakesReverse();
    vex::task::sleep(1000);
    Cpm::stopAllIntakes();
    Cpm::turnRobotLeft(135);
  }

  void performStateInit() {
    Cpm::moveRobotForward(24);
    vex::task::sleep(200);
    Cpm::turnRobotLeft(135);
    //Cpm::turnRobotRight(135);
  }

  void performStateTrackFirstGoal() {
    const int MaxSpeed   = 7;
    const int HalfSpeed  = 4;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    int dbgLeftRight = 3;  // Default to: pointing at target
    static bool sRanOnce = false;

    if (sRanOnce == false) {
      //hwBrain.Screen.print("STATE_TRACK_FIRST_GOAL");
      //hwBrain.Screen.newLine();
      //Cpm::clearLimitSwitchPressed();
      //Cpm::clearBumperSwitchPressed();
      sRanOnce = true;
    }

    // Get latest info on the target
    getBoxData(JetsonData::GOAL, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

    if (hasTarget == false) {
      dbgLeftRight = 0;    // no valid target
      Cpm::coastWheels();
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
    else {
      // Here the robot is pointed at the target, move forward to target
      if (curDepthI >= 30) {
        sLeftMotorSpeed = 40;
        sRightMotorSpeed = 40;
      }
      else if (curDepthI >= 8) {
        sLeftMotorSpeed = 30;
        sRightMotorSpeed = 30;
      }
      else {
        sLeftMotorSpeed = 20;   // TBD
        sRightMotorSpeed = 20;  // TBD
        Cpm::startBottomIntakes();
        sCurState = STATE_TRACK_FIRST_GOAL_IN_RANGE;
      }
    }
    updateJetsonDisplayLr(dbgLeftRight);

    // Move wheels based on trackloop positions
    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
  }

  void performStateTrackFirstGoalInRange() {
    static bool sMoveForward = true;
    static bool sRanOnce = false;

    if (sRanOnce == false) {
      //hwBrain.Screen.print("STATE_TRACK_FIRST_GOAL_IN_RANGE");
      //hwBrain.Screen.newLine();
      //Cpm::clearLimitSwitchPressed();
      Cpm::clearBumperSwitchPressed();
      sRanOnce = true;
    }

    if (sMoveForward == true) {
      //hwLimit.pressed(Cpm::setLimitSwitchPressed);    // TBD - Use Cpm
      hwBumper.pressed(Cpm::setBumperSwitchPressed);  // TBD - Use Cpm
      sMoveForward = false;
    }

    if (Cpm::wasBumperSwitchPressed() == true) {
      Cpm::stopWheels();
      Cpm::startAllIntakes();
      vex::task::sleep(1000);
      vex::task::sleep(3000);
      Cpm::stopAllMotors();
      sCurState = STATE_BACK_UP_AND_TURN;
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

  void performStateTrackSecondGoal() {
    const int MaxSpeed   = 7;
    const int HalfSpeed  = 4;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    int dbgLeftRight = 3;  // Default to: pointing at target
    static bool sRanOnce = false;

    if (sRanOnce == false) {
      //hwBrain.Screen.print("STATE_TRACK_FIRST_GOAL");
      //hwBrain.Screen.newLine();
      //Cpm::clearLimitSwitchPressed();
      //Cpm::clearBumperSwitchPressed();
      sRanOnce = true;
    }

    // Get latest info on the target
    getBoxData(JetsonData::GOAL, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

    if (hasTarget == false) {
      dbgLeftRight = 0;    // no valid target
      Cpm::coastWheels();
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
    else {
      // Here the robot is pointed at the target, move forward to target
      if (curDepthI >= 30) {
        sLeftMotorSpeed = 40;
        sRightMotorSpeed = 40;
      }
      else if (curDepthI >= 8) {
        sLeftMotorSpeed = 30;
        sRightMotorSpeed = 30;
      }
      else {
        sLeftMotorSpeed = 20;   // TBD
        sRightMotorSpeed = 20;  // TBD
        Cpm::startBottomIntakes();
        sCurState = STATE_TRACK_SECOND_GOAL_IN_RANGE;
      }
    }
    updateJetsonDisplayLr(dbgLeftRight);

    // Move wheels based on trackloop positions
    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
  }

  void performStateTrackSecondGoalInRange() {
    static bool sMoveForward = true;
    static bool sRanOnce = false;

    if (sRanOnce == false) {
      //hwBrain.Screen.print("STATE_TRACK_FIRST_GOAL_IN_RANGE");
      //hwBrain.Screen.newLine();
      //Cpm::clearLimitSwitchPressed();
      Cpm::clearBumperSwitchPressed();
      sRanOnce = true;
    }

    if (sMoveForward == true) {
      //hwLimit.pressed(Cpm::setLimitSwitchPressed);    // TBD - Use Cpm
      hwBumper.pressed(Cpm::setBumperSwitchPressed);  // TBD - Use Cpm
      sMoveForward = false;
    }

    if (Cpm::wasBumperSwitchPressed() == true) {
      Cpm::stopWheels();
      Cpm::startAllIntakes();
      vex::task::sleep(5000);
      Cpm::stopAllMotors();
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

  void performStateTrackFirstTarget() {
    const int MaxSpeed   = 7;
    const int HalfSpeed  = 4;
    bool  hasTarget      = false;
    int   curX           = 0;
    int   curY           = 0;
    float curWidthI      = 0.0;
    float curHeightI     = 0.0;
    float curDepthI      = 0.0;
    int dbgLeftRight = 3;  // Default to: pointing at target
    JetsonData::ClassIdType ourBall = CurConfig::getOurTeamBallColor();

    // Get latest info on the target
    getBoxData(ourBall, &hasTarget, &curX, &curY, &curWidthI, &curHeightI, &curDepthI);

    if (hasTarget == false) {
      dbgLeftRight = 0;    // no valid target
      Cpm::coastWheels();
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
        sLeftMotorSpeed = 20;
        sRightMotorSpeed = 20;
        Cpm::startBottomIntakes();
        Cpm::startMiddleIntake();
        sCurState = STATE_TRACK_FIRST_TARGET_IN_RANGE;
      }
    }
    updateJetsonDisplayLr(dbgLeftRight);

    // Move wheels based on trackloop positions
    hwMotorWheelFrontLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelFrontRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackLeft.spin(vex::directionType::fwd, sLeftMotorSpeed, vex::velocityUnits::pct);
    hwMotorWheelBackRight.spin(vex::directionType::fwd, sRightMotorSpeed, vex::velocityUnits::pct);
  }

  void performStateTrackFirstTargetInRange() {
    static bool sMoveForward = true;
    static bool sRanOnce = false;

    if (sRanOnce == false) {
      //hwBrain.Screen.print("STATE_TRACK_FIRST_TARGET_IN_RANGE");
      //hwBrain.Screen.newLine();
      Cpm::clearLimitSwitchPressed();
      Cpm::clearBumperSwitchPressed();
      sRanOnce = true;
    }

    if (sMoveForward == true) {
      hwLimit.pressed(Cpm::setLimitSwitchPressed);    // TBD - Use Cpm
      hwBumper.pressed(Cpm::setBumperSwitchPressed);  // TBD - Use Cpm
      sMoveForward = false;
    }

    if ((Cpm::wasLimitSwitchPressed() == true) || (Cpm::wasBumperSwitchPressed() == true)) {
      Cpm::stopAllMotors();
      sCurState = STATE_BACK_UP_AND_OUTAKE;
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

  void runStateMachine() {
    switch (sCurState) {
    case STATE_INIT :
      performStateInit();
      sCurState = STATE_TRACK_FIRST_TARGET;
      break;
    case STATE_TRACK_FIRST_TARGET :
      performStateTrackFirstTarget();
      break;
    case STATE_TRACK_FIRST_TARGET_IN_RANGE :
      performStateTrackFirstTargetInRange();
      break;
    case STATE_BACK_UP_AND_OUTAKE :
      performStateBackUpAndOutake();
      sCurState = STATE_TRACK_FIRST_GOAL;
      break;
    case STATE_TRACK_FIRST_GOAL :
      performStateTrackFirstGoal();
      break;
    case STATE_TRACK_FIRST_GOAL_IN_RANGE :
      performStateTrackFirstGoalInRange();
      break;
    case STATE_BACK_UP_AND_TURN :
      performStateBackUpAndTurn();
      sCurState = STATE_TRACK_SECOND_GOAL;
    case STATE_TRACK_SECOND_GOAL :
      performStateTrackSecondGoal();
    case STATE_TRACK_SECOND_GOAL_IN_RANGE:
      performStateTrackSecondGoalInRange();
      break;
    case STATE_DONE :
      Cpm::clearLimitSwitchPressed();
      Cpm::clearBumperSwitchPressed();
      Cpm::stopAllMotors();
      break;
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
