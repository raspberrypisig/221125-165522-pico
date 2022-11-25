#pragma once

#include "command.h"

struct DemoCommand {
  int currentCommand = 0;
};

class TestDemoCommand : public Command
{
  private:
    unsigned long timeStart = 0UL;
    int currentDemoIndex = 0;
    std::vector<int> demos{'I','1','2','3'};

  public:
    void parseArgs(std::vector<std::string> args) override {
    }

    int demoCommmand() override {
      return demos.at(currentDemoIndex);
    }

    void execute(DMD3 *canvas)  override {
      if ( millis() - timeStart > 30000) {
        currentDemoIndex = (currentDemoIndex + 1) % demos.size();
        if (currentDemoIndex == 0) currentDemoIndex++;
        timeStart = millis();
      }
    }
};
