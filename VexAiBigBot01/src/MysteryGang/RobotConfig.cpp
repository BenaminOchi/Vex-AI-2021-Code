#include "MysteryGang/RobotConfig.h"

// VEXcode device constructors
vex::brain       hwBrain;
vex::bumper      hwBumper               = vex::bumper(hwBrain.ThreeWirePort.A);
vex::competition hwCompetition;
vex::controller  hwController           = vex::controller(primary);
vex::limit       hwLimit                = vex::limit(hwBrain.ThreeWirePort.C);
vex::motor       hwMotorWheelFrontLeft  = vex::motor(PORT11, ratio18_1, false);
vex::motor       hwMotorWheelFrontRight = vex::motor(PORT13, ratio18_1, true);
vex::motor       hwMotorWheelBackLeft   = vex::motor(PORT12, ratio18_1, false);
vex::motor       hwMotorWheelBackRight  = vex::motor(PORT14, ratio18_1, true);
vex::motor       hwMotorIntakeLeft      = vex::motor(PORT16, ratio18_1, false);
vex::motor       hwMotorIntakeRight     = vex::motor(PORT17, ratio18_1, true);
vex::motor       hwMotorIntakeShooter   = vex::motor(PORT18, ratio18_1, true);
vex::motor       hwMotorIntakeMiddle    = vex::motor(PORT19, ratio18_1, true);
