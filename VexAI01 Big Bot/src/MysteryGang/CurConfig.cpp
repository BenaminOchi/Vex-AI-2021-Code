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
  const char* sOverallVersion   = "0001";



  void displayConfigBrain() {
    hwBrain.Screen.clearScreen();
    hwBrain.Screen.setCursor(1,1);
    // Diisplay overall information (build date and time, version, etc.)
    //    OV : Overall Version
    hwBrain.Screen.print("OVERALL     : %s : %s [%s]", sOverallVersion, sOverallBuildDate, sOverallBuildTime);
    hwBrain.Screen.newLine();

    hwBrain.Screen.print("ISOLATION   : %s : %s [%s]", IsolationMode::getVersion(),
      IsolationMode::getBuildDate(), IsolationMode::getBuildTime());
    hwBrain.Screen.newLine();

    hwBrain.Screen.print("INTERACTIVE : %s : %s [%s]", InteractiveMode::getVersion(),
      InteractiveMode::getBuildDate(), InteractiveMode::getBuildTime());
    hwBrain.Screen.newLine();
  }

  void initialize() {
    // TBD - Finish
  }
}
