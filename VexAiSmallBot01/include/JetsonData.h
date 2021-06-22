#ifndef JetsonData_h
#define JetsonData_h

namespace JetsonData {
  enum ClassIdType {
    BALL_UNDEFINED = 0,
    BALL_RED = 10,
    BALL_BLUE = 20,
    GOAL = 30
  };

  const unsigned int CenterX = 320;
  const unsigned int CenterAdjustForMaxSpeed  = 100*2;
  const unsigned int CenterAdjustForHalfSpeed = 60*2;

  void getBoxData(ClassIdType classId, bool* hasTarget, int* x, int* y, float* widthI, float* heightI, float* depthI);
  bool isBallBlue(int classId);
  bool isBallRed(int classId);
  bool isGoal(int classId);
}

#endif
