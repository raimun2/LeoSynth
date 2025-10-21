#pragma once
#include <Arduino.h>

// === Pin mapping (final) ===
// LEDs (charlieplex with 3 pins -> 4 LEDs)
static const uint8_t US_LED_PINS[3] = {2, 3, 4};

// Audio
static const uint8_t US_AUDIO_OUT_PIN = 5;  // tone() PWM output
static const uint8_t US_AUDIO_IN_PIN  = A5; // CV / audio in (0-5V)

// Button matrix (4 rows x 3 cols = 12 buttons)
// We split them into: first 8 = keys, last 4 = function buttons
static const uint8_t US_ROW_PINS[4] = {6, 7, 8, 9};     // outputs
static const uint8_t US_COL_PINS[3] = {10, 11, 12};     // inputs (pullup)

// 3-position switch (both pullups as requested)
static const uint8_t US_SWITCH_DIGITAL = 13; // INPUT_PULLUP
static const uint8_t US_SWITCH_ANALOG  = A0; // INPUT_PULLUP (read with analogRead)

// Pots
static const uint8_t US_POT_PINS[4] = {A1, A2, A3, A4};

// ===== Charlieplex helpers =====
inline void US_clearLEDs() {
  for (int i = 0; i < 3; i++) pinMode(US_LED_PINS[i], INPUT); // Hi-Z
}

inline void US_showLED(uint8_t id) {
  US_clearLEDs();
  switch (id) {
    case 0: // LED1 D2 -> D3
      pinMode(US_LED_PINS[0], OUTPUT); digitalWrite(US_LED_PINS[0], HIGH);
      pinMode(US_LED_PINS[1], OUTPUT); digitalWrite(US_LED_PINS[1], LOW);
      break;
    case 1: // LED2 D3 -> D2
      pinMode(US_LED_PINS[1], OUTPUT); digitalWrite(US_LED_PINS[1], HIGH);
      pinMode(US_LED_PINS[0], OUTPUT); digitalWrite(US_LED_PINS[0], LOW);
      break;
    case 2: // LED3 D3 -> D4
      pinMode(US_LED_PINS[1], OUTPUT); digitalWrite(US_LED_PINS[1], HIGH);
      pinMode(US_LED_PINS[2], OUTPUT); digitalWrite(US_LED_PINS[2], LOW);
      break;
    case 3: // LED4 D4 -> D3
      pinMode(US_LED_PINS[2], OUTPUT); digitalWrite(US_LED_PINS[2], HIGH);
      pinMode(US_LED_PINS[1], OUTPUT); digitalWrite(US_LED_PINS[1], LOW);
      break;
    default: break;
  }
}
