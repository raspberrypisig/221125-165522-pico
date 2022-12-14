
#include "dispatcher.h"
#include "DMD3.h"
#include "customglcdfonts.h"
#include "debug.h"
#include "timeout.h"

#if defined(ARDUINO_ARCH_RP2040)
/*
  #define PIN_DATA1 3
  #define PIN_DATA2 4
  #define PIN_DATA3 5
  #define PIN_DATA4 2
*/

#define PIN_DATA1 3
#define PIN_DATA2 2
#define PIN_DATA3 4
#define PIN_DATA4 5

#define PIN_CLK 12
#define PIN_LATCH 11
#define PIN_NOE 10
#elif defined(ESP8266)
#define PIN_DATA1 D6
#define PIN_DATA2 D2
#define PIN_DATA3 D1
#define PIN_DATA4 D8

#define PIN_CLK D7
#define PIN_LATCH D0
#define PIN_NOE D5
#endif
// TX = 0
// RX = 1

#define PANELS_WIDTH 6
#define PANELS_HEIGHT 2

Dispatcher *runner;

DMD3* determinePanelSize() {
  // 512KB block device, starting 1MB inside the flash
  FlashIAPBlockDevice bd(XIP_BASE + 1024 * 1024, 1024 * 512);
  mbed::TDBStore eeprom(&bd);
  eeprom.init();
  mbed::KVStore::info_t info;
  int ram_buffer[24];
  int width;
  int height;
  const char* keystorename = "panelsize";

  if (eeprom.get_info(keystorename, &info) != MBED_ERROR_ITEM_NOT_FOUND) {
    eeprom.get(keystorename, ram_buffer, info.size);
    width = ram_buffer[0];
    height = ram_buffer[1];
    return new DMD3(width, height);
  }
  return new DMD3(PANELS_WIDTH, PANELS_HEIGHT);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200); //Rx
  delay(1000);

  //Enable debugging project-wide
  DEBUG = true;
  //DEBUG = false;

  // Enable UPDATE LOOP time period for each command system-wide
  TIMEOUT = 2000;

  DMD3 *display = determinePanelSize();
  //display.begin(PIN_CLK, PIN_LATCH, PIN_NOE, PIN_DATA1, PIN_DATA2, PIN_DATA3, PIN_DATA4);
  display->begin(PIN_CLK, PIN_LATCH, PIN_NOE, PIN_DATA1, PIN_DATA2, PIN_DATA3, PIN_DATA4);
  //display.setBrightness(0.05);
  //display.setFont(ArialFont16x16);
  //display.setFont(SpaceInvadersFont16x16);
  //display.setFont(VerdanaFont16x16);
  display->setFont(VerdanaFont16x16);

  //runner = new Dispatcher(&display);
  runner = new Dispatcher(display);
  //runner->dispatch('1');
  runner->dispatch('2');
}

void loop() {
  runner->handleSerialInput(Serial);
  runner->handleSerialInput(Serial1);
}
