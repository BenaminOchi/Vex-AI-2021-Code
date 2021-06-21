#ifndef JetsonData_h
#define JetsonData_h

namespace JetsonData {
  enum ClassIdType {
    BALL_UNDEFINED = 0,
    BALL_RED = 10,
    BALL_BLUE = 20,
    GOAL = 30
  };

  void getBoxData(ClassIdType classId, bool* hasTarget, int* x, int* y, float* widthI, float* heightI, float* depthI);
}

#endif
