#ifndef CommonPartMethods_h
#define CommonPartMethods_h

namespace Cpm {
  void moveRobotBackward(unsigned int numInches);

  // Move the robot forwards for the specified amount of time
  void moveRobotForward(unsigned int numInches);

  void startWheels();

  // Stop all of the robot's motors
  void stopAllMotors();

  // Stop the robot's wheels
  void stopWheels();
  
  // Turn the robot to the left for the specified motor rotations
  // Rotations in degrees must be negative
  void turnRobotLeft(unsigned int numDegrees);

  // Turn the robot to the right for the specified motor rotations
  // Rotations in degrees must be positive
  void turnRobotRight(unsigned int numDegrees);

  void startAllIntakes();

  void stopAllIntakes();

  void startBottomIntakes();

  void stopBottomIntakes();

  void startMiddleIntake();

  void stopMiddleIntake();

  void startTopIntakes();

  void stopTopIntakes();

  void startAllIntakesReverse();

  void startBottomIntakesReverse();

  void startMiddleIntakeReverse();

  void startTopIntakesReverse();

  bool wasLimitSwitchPressed();

  void setLimitSwitchPressed();

  void clearLimitSwitchPressed();

  bool wasBumperSwitchPressed();

  void setBumperSwitchPressed();

  void clearBumperSwitchPressed();

  void coastWheels();

  void stopWithCoast();
}
#endif
