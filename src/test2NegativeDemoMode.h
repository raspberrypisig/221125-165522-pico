#pragma once

#include "command.h"

struct DemoCommand {
  int currentCommand = 0;
};

class TestIdleCommand : public Command
{
  private:
    unsigned long timeStart = 0UL;
    char text[100];
        

  public:
    void parseArgs(std::vector<std::string> args) override {
    // Serial.println(args[0].c_str());
      strcpy(text, args.at(0).c_str());
    }

    void execute(DMD3 *canvas)  override {
      if ( millis() - timeStart > 2000) { 
        clearSerialMonitor();
        canvas->clear();

        printCanvas(canvas);
        canvas->update();
        timeStart = millis();
      }
    }
};
