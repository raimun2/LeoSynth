#pragma once
#include <Arduino.h>
#include "LeoSynthPins.h"

// LeoSynth Hardware Abstraction Layer (HAL)
// Handles all inputs (buttons, switch, pots, audio-in) and outputs (LEDs, audio-out)

class LeoSynth {
public:
  // Switch states
  enum SwitchState : uint8_t { SW_POS0 = 0, SW_POS1 = 1, SW_POS2 = 2 };

  // Initialise hardware
  static void begin();

  // Update input states each loop
  static void update();

  // ===== INPUTS =====
  // Button: true while held
  static bool button(uint8_t index);

  // Button rising edge: true only once on press
  static bool buttonEdge(uint8_t index);

  // Logical PAD mapped to hardware layout
  static bool pad(uint8_t index);
  static bool padEdge(uint8_t index);

  // Logical CTRL buttons (top row controls)
  static bool ctrl(uint8_t index);
  static bool ctrlEdge(uint8_t index);

  // 3-way switch position
  static SwitchState triSwitch();

  // Potentiometers (raw analog 0-1023)
  static uint16_t potRaw(uint8_t index);

  // Potentiometers (smoothed)
  static uint16_t potSmooth(uint8_t index);

  // Audio/CV input
  static uint16_t audioIn(); // Reads A5

  // ===== OUTPUTS =====
  // Clear all LEDs
  static void ledClear();

  // Set Charlieplex LED by id (0-3)
  static void ledSet(uint8_t id);

  // Flash LED briefly (blocking)
  static void ledFlash(uint8_t id, uint16_t ms);

  // Play simple beep
  static void beep(uint16_t freq, uint16_t ms);

  // Stop audio
  static void audioOff();

private:
  // Internal scans
  static void scanButtons_();
  static void scanSwitch_();
  static void scanPots_();

  // Charlieplex LED helper
  static void setLEDPair_(uint8_t id);

  // ===== STATE =====
  static uint8_t btnState_[USYN_NUM_BTNS];
  static uint8_t btnPrev_[USYN_NUM_BTNS];
  static uint32_t btnTs_[USYN_NUM_BTNS];

  static uint16_t potsRaw_[USYN_NUM_POTS];
  static uint16_t potsSm_[USYN_NUM_POTS];

  static SwitchState swState_;
  static uint32_t beepUntilMs_;
};
