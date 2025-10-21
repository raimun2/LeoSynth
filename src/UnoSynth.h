#pragma once
#include <Arduino.h>
#include "hardware_layout.h"
#include "utils.h"

// Public API — patches only see these functions
namespace UnoSynth {
  // lifecycle
  void begin();
  void update();                 // refresh LEDs, internal timers (call each loop)

  // inputs
  bool getKey(uint8_t i);        // 0..7
  bool getFn(uint8_t i);         // 0..3
  int  getPot(uint8_t i);        // 0..3 (0..1023)
  int  getPage();                // 0..2 (3-position switch)

  // LEDs (4 total, indexed 0..3)
  void setLED(uint8_t id, bool on);

  // simple audio (tone-based)
  void playFreq(uint16_t hz);
  void stopAudio();
}
