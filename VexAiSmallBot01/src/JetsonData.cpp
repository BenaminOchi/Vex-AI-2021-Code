#include "vex.h"
#include "JetsonData.h"
#include "MysteryGang/RobotConfig.h"


namespace JetsonData {

  bool isBallRed(int classId) {
    if ((classId >= BALL_RED) && (classId < BALL_BLUE)) {
      return true;
    }
    else {
      return false;
    }
  }

  bool isBallBlue(int classId) {
    if ((classId >= BALL_BLUE) && (classId < GOAL)) {
      return true;
    }
    else {
      return false;
    }
  }

  void getBoxData(ClassIdType classId, bool* hasTarget, int* x, int* y, float* widthI, float* heightI, float* depthI) {
    static MAP_RECORD  localMap;
    bool hasValidTarget = false;

    jetson_comms.get_data(&localMap);   // Get last map data

    for (int i=0;i<4;i++) {
      if (i < localMap.boxnum) {
        if (localMap.boxobj[i].classID == classId) {
          hasValidTarget = true;
          *x          = localMap.boxobj[i].x;
          *y          = localMap.boxobj[i].y;
          *widthI     = localMap.boxobj[i].width / 25.4;
          *heightI    = localMap.boxobj[i].height / 25.4;
          *depthI     = localMap.boxobj[i].depth / 25.4;
          *hasTarget  = true;
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
      *hasTarget  = false;
    }
  }
}
