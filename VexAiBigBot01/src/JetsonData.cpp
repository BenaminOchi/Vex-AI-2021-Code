#include "vex.h"
#include "JetsonData.h"
#include "MysteryGang/RobotConfig.h"


namespace JetsonData {

  void getBoxData(ClassIdType classId, bool* hasTarget, int* x, int* y, float* widthI, float* heightI, float* depthI) {
    static MAP_RECORD  sLocalMap;
    bool hasValidTarget = false;

    jetson_comms.get_data(&sLocalMap);   // Get last map data

    for (int i=0;i<4;i++) {
      if (i < sLocalMap.boxnum) {
        if (sLocalMap.boxobj[i].classID == classId) {
          hasValidTarget = true;
          *x          = sLocalMap.boxobj[i].x;
          *y          = sLocalMap.boxobj[i].y;
          *widthI     = sLocalMap.boxobj[i].width / 25.4;
          *heightI    = sLocalMap.boxobj[i].height / 25.4;
          *depthI     = sLocalMap.boxobj[i].depth / 25.4;
          break;
        }
      }
      else {
      }
    }

    if (hasValidTarget == false) {
      *x          = 0;
      *y          = 0;
      *widthI     = 0.0;
      *heightI    = 0.0;
      *depthI     = 0.0;
    }
    *hasTarget = hasValidTarget;
  }

  bool isBallBlue(int classId) {
    if ((classId >= BALL_BLUE) && (classId < GOAL)) {
      return true;
    }
    else {
      return false;
    }
  }

  bool isBallRed(int classId) {
    if ((classId >= BALL_RED) && (classId < BALL_BLUE)) {
      return true;
    }
    else {
      return false;
    }
  }

  bool isGoal(int classId) {
    if (classId >= GOAL) {
      return true;
    }
    else {
      return false;
    }
  }
}
