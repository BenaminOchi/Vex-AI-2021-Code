#include "vex.h"
#include "JetsonData.h"
#include "MysteryGang/RobotConfig.h"
#include "MysteryGang/CurConfig.h"


namespace JetsonData {

  enum JetsonCommClassIdType {
    JC_UNDEFINED              =  0,
    JC_BALL_RED_DONT_CARE     = 10,
    JC_BALL_RED_OUT_OF_GOAL   = 11,
    JC_BALL_RED_IN_GOAL       = 12,
    JC_BALL_BLUE_DONT_CARE    = 20,
    JC_BALL_BLUE_OUT_OF_GOAL  = 21,
    JC_BALL_BLUE_IN_GOAL      = 22,
    JC_GOAL_DONT_CARE         = 30,
    JC_GOAL_WITH_OPENINGS     = 31,   // Atleast 1 spot open in the goal
    JC_GOAL_RED_TO_DESCORE    = 32,   // More red than blue in the tower
    JC_GOAL_BLUE_TO_DESCORE   = 33    // More blue than red in the tower
  };

  static int sLastJetsonClassId = JC_UNDEFINED;  // JetsonCommClassIdType

  int getOffsetCenterX(int depth) {
    int offset = 0;

    // Update offset for Small Bot (based on manual centerX measurements)
    if (depth > 40)      { offset = 5; }
    else if (depth > 24) { offset = 31; }
    else if (depth > 23) { offset = 32; }
    else if (depth > 22) { offset = 34; }
    else if (depth > 21) { offset = 35; }
    else if (depth > 20) { offset = 36; }
    else if (depth > 19) { offset = 37; }
    else if (depth > 18) { offset = 39; }
    else if (depth > 17) { offset = 41; }
    else if (depth > 16) { offset = 46; }
    else                 { offset = 47; }
    return offset;
  }
  
  void getBoxDataGoal(ClassIdType classId, bool* hasTarget, int* x, int* y, float* widthI, float* heightI, float* depthI) {
    static MAP_RECORD  sLocalMap;
    bool hasValidTarget = false;
    fifo_object_box* curBox;

    jetson_comms.get_data(&sLocalMap);   // Get last map data

    for (int cnt=0; cnt<6; cnt++) {
      if (cnt < sLocalMap.boxnum) {
        curBox = &sLocalMap.boxobj[cnt];
        if (isGoal(curBox->classID) && isGoal(classId)) {
          hasValidTarget = true;
          break;
        }
      }
      else {
      }
    }

    if (hasValidTarget) {
      int offset = getOffsetCenterX(curBox->depth);
      *x          = curBox->x - offset;
      *y          = curBox->y;
      *widthI     = curBox->width / 25.4;
      *heightI    = curBox->height / 25.4;
      *depthI     = curBox->depth / 25.4;
      sLastJetsonClassId = curBox->classID;
    }
    else {
      *x          = 0;
      *y          = 0;
      *widthI     = 0.0;
      *heightI    = 0.0;
      *depthI     = 0.0;
      sLastJetsonClassId = JC_UNDEFINED;
    }
    *hasTarget = hasValidTarget;
  }

  void getBoxTeamBall(bool isOurTeam, BallRelationToGoalType ballType, bool* hasTarget, int* x, int* y, float* widthI, float* heightI, float* depthI) {
    static MAP_RECORD  sLocalMap;
    bool hasValidTarget = false;
    fifo_object_box* curBox;
    JetsonData::ClassIdType teamBallColor;
    
    if (isOurTeam) {
      teamBallColor = CurConfig::getOurTeamBallColor();
    }
    else {
      teamBallColor = CurConfig::getOpponentTeamBallColor();
    }

    jetson_comms.get_data(&sLocalMap);   // Get last map data

    for (int cnt=0; cnt<6; cnt++) {
      if (cnt < sLocalMap.boxnum) {
        curBox = &sLocalMap.boxobj[cnt];
        if (teamBallColor == JetsonData::BALL_RED) {
          if (ballType == BRTGT_INSIDE_GOAL) {
            if (curBox->classID == JC_BALL_RED_IN_GOAL) {
              hasValidTarget = true;
              break;
            }
          }
          else if (ballType == BRTGT_OUTSIDE_GOAL) {
            if (curBox->classID == JC_BALL_RED_OUT_OF_GOAL) {
              hasValidTarget = true;
              break;
            }
          }
          else if (ballType == BRTGT_DONT_CARE) {
            if (curBox->classID == JC_BALL_RED_DONT_CARE) {
              hasValidTarget = true;
              break;
            }
          }
        }
        else {    // Team Blue
          if (ballType == BRTGT_INSIDE_GOAL) {
            if (curBox->classID == JC_BALL_BLUE_IN_GOAL) {
              hasValidTarget = true;
              break;
            }
          }
          else if (ballType == BRTGT_OUTSIDE_GOAL) {
            if (curBox->classID == JC_BALL_BLUE_OUT_OF_GOAL) {
              hasValidTarget = true;
              break;
            }
          }
          else if (ballType == BRTGT_DONT_CARE) {
            if (curBox->classID == JC_BALL_BLUE_DONT_CARE) {
              hasValidTarget = true;
              break;
            }
          }
        }
      }
      else {
      }
    }

    if (hasValidTarget) {
      int offset = getOffsetCenterX(curBox->depth);
      *x          = curBox->x - offset;
      *y          = curBox->y;
      *widthI     = curBox->width / 25.4;
      *heightI    = curBox->height / 25.4;
      *depthI     = curBox->depth / 25.4;
      sLastJetsonClassId = curBox->classID;
    }
    else {
      *x          = 0;
      *y          = 0;
      *widthI     = 0.0;
      *heightI    = 0.0;
      *depthI     = 0.0;
      sLastJetsonClassId = JC_UNDEFINED;
    }
    *hasTarget = hasValidTarget;
  }

  void getBoxDataOurTeamBall(BallRelationToGoalType ballType, bool* hasTarget, int* x, int* y, float* widthI, float* heightI, float* depthI) {
    getBoxTeamBall(true, ballType, hasTarget, x, y, widthI, heightI, depthI);
  }

  void getBoxDataOpponentTeamBall(BallRelationToGoalType ballType, bool* hasTarget, int* x, int* y, float* widthI, float* heightI, float* depthI) {
    getBoxTeamBall(false, ballType, hasTarget, x, y, widthI, heightI, depthI);
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
