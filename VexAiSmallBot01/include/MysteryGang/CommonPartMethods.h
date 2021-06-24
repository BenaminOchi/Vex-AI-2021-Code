#ifndef CommonPartMethods_h
#define CommonPartMethods_h

namespace Cpm {
  void moveRobotBackward(unsigned int numInches);

  // Move the robot forwards for the specified amount of time
  void moveRobotForward(unsigned int numInches);

  void startWheels();

  // Stop all of the robot's motors
  void stopAllMotors();

  // Stop all the robot's wheels
  void stopAllWheels();
  
  // Turn the robot to the left for the specified motor rotations
  // Rotations in degrees must be negative
  void turnRobotLeft(unsigned int numDegrees);

  // Turn the robot to the right for the specified motor rotations
  // Rotations in degrees must be positive
  void turnRobotRight(unsigned int numDegrees);

  // Start all of the robot's intakes
  void startAllIntakes();

  // Stop all of the robot's intakes
  void stopAllIntakes();

  // Start all of the robot's intakes in reverse
  void startAllIntakesReverse();

  // Start the robot's bottom-most intakes
  void startBottomIntakes();

  // Stop the robot's bottom-most intakes
  void stopBottomIntakes();

  // Start the robot's bottom-most intakes in reverse
  void startBottomIntakesReverse();

  // Start the robot's middle intakes
  void startMiddleIntake();

  // Start the robot's middle intakes
  void stopMiddleIntake();

  // Start the robot's middle intakes in reverse
  void startMiddleIntakeReverse();

  // Start the robot's upper-most intakes
  void startTopIntakes();

  // Stop the robot's upper-most intakes
  void stopTopIntakes();

  // Start the robot's upper-most intakes in reverse
  void startTopIntakesReverse();

  bool wasLimitSwitchPressed();
  void setLimitSwitchPressed();
  void disableLimitSwitch();
  void enableLimitSwitch();

  bool wasBumperSwitchPressed();
  void setBumperSwitchPressed();
  void disableBumperSwitch();
  void enableBumperSwitch();

  // Coast all of the robot's wheels
  void coastAllWheels();

  // Coast for a bit of time, then stop all wheels
  void stopWithCoast();
}
#endif
