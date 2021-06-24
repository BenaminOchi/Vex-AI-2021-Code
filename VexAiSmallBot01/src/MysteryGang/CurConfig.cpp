#include "MysteryGang/CurConfig.h"
#include "MysteryGang/ManualMode.h"
#include "MysteryGang/InteractiveMode.h"
#include "MysteryGang/IsolationMode.h"
#include "MysteryGang/RobotConfig.h"


namespace CurConfig {
  //-----------------------
  // Local Variables
  //-----------------------
  const char* sOverallBuildDate = __DATE__;
  const char* sOverallBuildTime = __TIME__;
  const char* sOverallVersion   = "0002";

  static JetsonData::ClassIdType sOurTeamBallColor = JetsonData::BALL_RED;


  void displayConfigBrain() {
    hwBrain.Screen.clearScreen();
    hwBrain.Screen.setCursor(1,1);

    hwBrain.Screen.print("OVERALL     : %s : %s [%s]", sOverallVersion, sOverallBuildDate, sOverallBuildTime);
    hwBrain.Screen.newLine();
    hwBrain.Screen.print("ISOLATION   : %s : %s [%s]", IsolationMode::getVersion(), IsolationMode::getBuildDate(), IsolationMode::getBuildTime());
    hwBrain.Screen.newLine();
    hwBrain.Screen.print("INTERACTIVE : %s : %s [%s]", InteractiveMode::getVersion(), InteractiveMode::getBuildDate(), InteractiveMode::getBuildTime());
    hwBrain.Screen.newLine();
  }

  JetsonData::ClassIdType getOpponentTeamBallColor() {
    if (sOurTeamBallColor == JetsonData::BALL_RED) {
      return JetsonData::BALL_BLUE;
    }
    else {
      return JetsonData::BALL_RED;
    }
  }
  JetsonData::ClassIdType getOurTeamBallColor() {
    return sOurTeamBallColor;
  }

  void initialize() {
    // TBD - Finish
  }
}
