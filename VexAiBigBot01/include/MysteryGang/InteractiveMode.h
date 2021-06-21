#ifndef _InteractiveMode_H
#define _InteractiveMode_H


namespace InteractiveMode {
  void enterMain();
  const char* getBuildDate();
  const char* getBuildTime();
  const char* getVersion();

  int controlTask();
  void setWheelSpeeds();
}

#endif
