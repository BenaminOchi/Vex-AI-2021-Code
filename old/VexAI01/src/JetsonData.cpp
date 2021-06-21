#include "vex.h"
#include "MysteryGang/RobotConfig.h"

// TBD - Use enum
const unsigned int BALL_UNDEFINED = 0;
const unsigned int BALL_RED = 10;
const unsigned int BALL_BLUE = 20;
const unsigned int GOAL = 30;

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

void getBoxData(bool* hasTarget, int* classId, int* x, int* y, float* widthI, float* heightI, float* depthI) {
  static MAP_RECORD  localMap;
  bool hasValidTarget = false;

  jetson_comms.get_data(&localMap);   // Get last map data

  for (int i=0;i<4;i++) {
    if (i < localMap.boxnum) {
      if (isBallRed(localMap.boxobj[i].classID)) {    //TBD: Update to support blue balls
        hasValidTarget = true;
        *classId    = localMap.boxobj[i].classID;
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
    *classId    = 1;
    *x          = 0;
    *y          = 0;
    *widthI     = 0.0;
    *heightI    = 0.0;
    *depthI     = 0.0;
    *hasTarget  = false;
  }
}
