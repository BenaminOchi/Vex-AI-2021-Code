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

  void buttonRightArrowPressed();
  void buttonLeftArrowPressed();

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

  void displayOurTeamBallColor() {
    hwController.Screen.clearScreen();
    hwController.Screen.setCursor(1, 1);
    hwController.Screen.newLine();
    if (sOurTeamBallColor == JetsonData::BALL_RED) {
      hwController.Screen.print("  Team : RED");
    }
    else {
      hwController.Screen.print("  Team : BLUE");
    }
    hwController.Screen.newLine();
  }

  void buttonRightArrowPressed() {
    if (hwCompetition.isEnabled() == true) {
      return;
    }
    displayOurTeamBallColor();
  }

  void toggleOurTeamBallColor() {
    if (sOurTeamBallColor == JetsonData::BALL_RED) {
      sOurTeamBallColor = JetsonData::BALL_BLUE;
    }
    else {
      sOurTeamBallColor = JetsonData::BALL_RED;
    }
  }

  void buttonLeftArrowPressed() {
    if (hwCompetition.isEnabled() == true) {
      return;
    }
    toggleOurTeamBallColor();
    displayOurTeamBallColor();
 }

  void initialize() {
    hwController.ButtonLeft.pressed(buttonLeftArrowPressed);
    hwController.ButtonRight.pressed(buttonRightArrowPressed);
    displayOurTeamBallColor();
  }

}
