#include "vex.h"
#include "MysteryGang/RobotConfig.h"



void getBoxData(bool* hasTarget, int* classId, int* x, int* y, float* widthI, float* heightI, float* depthI) {
  const unsigned int RED_BALL = 0;
  const unsigned int BLUE_BALL = 1;
  static MAP_RECORD  localMap;
  bool hasValidTarget = false;

  jetson_comms.get_data(&localMap);   // Get last map data

  for (int i=0;i<4;i++) {
    if (i < localMap.boxnum) {
      if (localMap.boxobj[i].classID == RED_BALL) {
        hasValidTarget = true;
        *classId    = localMap.boxobj[i].classID;
        *x          = localMap.boxobj[i].x;
        *y          = localMap.boxobj[i].y;
        *widthI     = localMap.boxobj[i].width / 25.4;
        *heightI    = localMap.boxobj[i].height / 25.4;
        *depthI     = localMap.boxobj[i].depth / 25.4;
        *hasTarget  = true;
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
