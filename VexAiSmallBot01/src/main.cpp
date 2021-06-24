#include "vex.h"
#include "MysteryGang/CommonPartMethods.h"
#include "MysteryGang/CurConfig.h"
#include "MysteryGang/InteractiveMode.h"
#include "MysteryGang/IsolationMode.h"
#include "MysteryGang/ManualMode.h" 
#include "MysteryGang/RobotConfig.h"

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----



// create instance of jetson class to receive location and other
// data from the Jetson nano
//
ai::jetson  jetson_comms;

static bool sFirstAutoFlag = true;


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          Auto_Isolation Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous isolation  */
/*  phase of a VEX AI Competition.                                           */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void auto_Isolation(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  IsolationMode::enterMain();
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                        Auto_Interaction Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous interaction*/
/*  phase of a VEX AI Competition.                                           */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


void auto_Interaction(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  InteractiveMode::enterMain();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          AutonomousMain Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void autonomousMain(void) {
  // ..........................................................................
  // The first time we enter this function we will launch our Isolation routine
  // When the field goes disabled after the isolation period this task will die
  // When the field goes enabled for the second time this task will start again
  // and we will enter the interaction period. 
  // ..........................................................................

  if (sFirstAutoFlag) {
    hwBrain.Screen.print("Enter Isolation Mode");
    hwBrain.Screen.newLine();
    auto_Isolation();
  }
  else { 
    hwBrain.Screen.print("Enter Interactive Mode");
    hwBrain.Screen.newLine();
    auto_Interaction();
  }
  sFirstAutoFlag = false;
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  Cpm::stopAllMotors();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void enterUserControl(void) {
  ManualMode::enterManualControl();
}

/*---------------------------------------------------------------------------*/
static bool sTestSwitchPressed = false;

void clearTestSwitchPressed() {
  sTestSwitchPressed = false;
}
void testSwitchPressed() {
  sTestSwitchPressed = true;
}

void testCpm() {
  /*
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
  */
  bool testLimitHw = true;
  bool testLimitCpm = true;
  bool testBumperHw = true;
  bool testBumperCpm = true;

  if (testLimitHw) {   // Test Limit Switch using hardware
    hwBrain.Screen.print("testLimitHw - Start");
    hwBrain.Screen.newLine();
    clearTestSwitchPressed();
    hwLimit.pressed(testSwitchPressed);
    while (sTestSwitchPressed == false) {
      vex::task::sleep(1);
    }
    clearTestSwitchPressed();
    hwBrain.Screen.print("testLimitHw - End");
    hwBrain.Screen.newLine();
  }

  if (testLimitCpm) {   // Test Limit Switch using CPM helpers
    hwBrain.Screen.print("testLimitCpm - Start");
    hwBrain.Screen.newLine();
    Cpm::disableLimitSwitch();
    Cpm::enableLimitSwitch();
    while (Cpm::wasLimitSwitchPressed() == false) {
      vex::task::sleep(1);
    }
    Cpm::disableLimitSwitch();
    hwBrain.Screen.print("testLimitCpm - End");
    hwBrain.Screen.newLine();
  }

  if (testBumperHw) {   // Test Bumper Switch using hardware
    hwBrain.Screen.print("testBumperHw - Start");
    hwBrain.Screen.newLine();
    clearTestSwitchPressed();
    hwBumper.pressed(testSwitchPressed);
    while (sTestSwitchPressed == false) {
      vex::task::sleep(1);
    }
    clearTestSwitchPressed();
    hwBrain.Screen.print("testBumperHw - End");
    hwBrain.Screen.newLine();
  }

  if (testBumperCpm) {   // Test Bumper Switch using CPM helpers
    hwBrain.Screen.print("testBumperCpm - Start");
    hwBrain.Screen.newLine();
    Cpm::disableBumperSwitch();
    Cpm::enableBumperSwitch();
    while (Cpm::wasBumperSwitchPressed() == false) {
      vex::task::sleep(1);
    }
    Cpm::disableBumperSwitch();
    hwBrain.Screen.print("testBumperCpm - End");
    hwBrain.Screen.newLine();
  }
}
/*---------------------------------------------------------------------------*/

//
// Main will set up the competition functions and callbacks.
//
int main() {
  int32_t loop_time = 66;     // Run at about 15Hz

  // Run the pre-autonomous function.
  pre_auton();

  CurConfig::initialize();
  CurConfig::displayConfigBrain();

  thread tDash(dashboardTask);   // Start the status update display
  tDash.setPriority(thread::threadPrioritylow);

  // Set up callbacks for autonomous and driver control periods.
  //hwCompetition.autonomous(autonomousMain);  // This is the real deal for COMPETITION
  IsolationMode::enterMain();  // TEMP for debug
  //enterUserControl();          // TEMP for debug

  //testCpm();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    // request new data    
    // NOTE: This request should only happen in a single task.    
    jetson_comms.request_map();

    // Allow other tasks to run
    this_thread::sleep_for(loop_time);
  }
}
