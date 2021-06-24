#include "vex.h"
#include "JetsonData.h"
#include "MysteryGang/RobotConfig.h"


namespace JetsonData {

  enum JetsonCommClassIdType {
    JC_UNDEFINED              =  0,
    JC_BALL_RED_GENERIC       = 10,
    JC_BALL_RED_OUT_OF_GOAL   = 11,
    JC_BALL_RED_IN_GOAL       = 12,
    JC_BALL_BLUE_GENERIC      = 20,
    JC_BALL_BLUE_OUT_OF_GOAL  = 21,
    JC_BALL_BLUE_IN_GOAL      = 22,
    JC_GOAL_GENERIC           = 30,
    JC_GOAL_WITH_OPENINGS     = 31,   // Atleast 1 spot open in the goal
    JC_GOAL_RED_TO_DESCORE    = 32,   // More red than blue in the tower
    JC_GOAL_BLUE_TO_DESCORE   = 33    // More blue than red in the tower
  };

  static int sLastJetsonClassId = JC_UNDEFINED;  // JetsonCommClassIdType

  
  void getBoxData(ClassIdType classId, bool* hasTarget, int* x, int* y, float* widthI, float* heightI, float* depthI) {
    static MAP_RECORD  sLocalMap;
    bool hasValidTarget = false;
    fifo_object_box* curBox;

    jetson_comms.get_data(&sLocalMap);   // Get last map data

    for (int cnt=0; cnt<6; cnt++) {
      if (cnt < sLocalMap.boxnum) {
        curBox = &sLocalMap.boxobj[cnt];
        if ((isBallRed(curBox->classID) && isBallRed(classId)) ||
            (isBallBlue(curBox->classID) && isBallBlue(classId)) ||
            (isGoal(curBox->classID) && isGoal(classId))) {
          hasValidTarget = true;
          *x          = curBox->x;
          *y          = curBox->y;
          *widthI     = curBox->width / 25.4;
          *heightI    = curBox->height / 25.4;
          *depthI     = curBox->depth / 25.4;
          sLastJetsonClassId = curBox->classID;
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
      sLastJetsonClassId = JC_UNDEFINED;
    }
    *hasTarget = hasValidTarget;
  }

  void getBoxDataBallRedOutOfGoal(ClassIdType classId, bool* hasTarget, int* x, int* y, float* widthI, float* heightI, float* depthI) {
    static MAP_RECORD  sLocalMap;
    bool hasValidTarget = false;
    fifo_object_box* curBox;

    jetson_comms.get_data(&sLocalMap);   // Get last map data

    for (int cnt=0; cnt<6; cnt++) {
      if (cnt < sLocalMap.boxnum) {
        curBox = &sLocalMap.boxobj[cnt];
        if ((curBox->classID == JC_BALL_RED_OUT_OF_GOAL) || (curBox->classID == JC_BALL_RED_GENERIC)) {
          hasValidTarget = true;
          *x          = curBox->x;
          *y          = curBox->y;
          *widthI     = curBox->width / 25.4;
          *heightI    = curBox->height / 25.4;
          *depthI     = curBox->depth / 25.4;
          sLastJetsonClassId = curBox->classID;
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
    sLastJetsonClassId = JC_UNDEFINED;
  }

  int getLastJetsonClassId() {
    return sLastJetsonClassId;
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
