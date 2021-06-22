#ifndef CurConfig_h
#define CurConfig_h

#include "JetsonData.h"


namespace CurConfig {

  //-----------------------
  // Function Prototypes
  //-----------------------

  // Displays the current version and configuration to the brain screen
  void displayConfigBrain();

  JetsonData::ClassIdType getOurTeamBallColor();

  // Initialize the configuration
  void initialize();
}

#endif
