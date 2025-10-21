#include "UnoSynth.h"

// ====== Internal state ======
static bool  s_keys[8]  = {0};
static bool  s_fns[4]   = {0};
static int   s_pots[4]  = {0};
static int   s_page     = 0;
static bool  s_leds[4]  = {0};
static unsigned long s_lastScan = 0;

// ====== Setup ======
void UnoSynth::begin() {
  // matrix
  for (int r = 0; r < 4; r++) { pinMode(US_ROW_PINS[r], OUTPUT); digitalWrite(US_ROW_PINS[r], HIGH); }
  for (int c = 0; c < 3; c++) { pinMode(US_COL_PINS[c], INPUT_PULLUP); }
  // switch (both pullups as requested)
  pinMode(US_SWITCH_DIGITAL, INPUT_PULLUP);
  pinMode(US_SWITCH_ANALOG,  INPUT_PULLUP);
  // pots
  for (int p = 0; p < 4; p++) pinMode(US_POT_PINS[p], INPUT);
  // LEDs idle (hi-Z for Charlieplexing)
  US_clearLEDs();
  // audio pin reserved (tone() manages pin mode)
}

// ====== Scan buttons (12 -> 8 keys + 4 fn) ======
static void US_scanMatrix() {
  // 4x3 => 12 buttons
  bool btn[12] = {0};
  for (int r = 0; r < 4; r++) {
    for (int i = 0; i < 4; i++) digitalWrite(US_ROW_PINS[i], HIGH);
    digitalWrite(US_ROW_PINS[r], LOW);
    for (int c = 0; c < 3; c++) {
      int idx = r * 3 + c;
      btn[idx] = (digitalRead(US_COL_PINS[c]) == LOW);
    }
  }
  // map: first 8 are keys, last 4 are fn
  for (int i = 0; i < 8; i++)  s_keys[i] = btn[i];
  for (int i = 0; i < 4; i++)  s_fns[i]  = btn[8 + i];
}

// ====== Read pots ======
static void US_readPots() {
  for (int i = 0; i < 4; i++) s_pots[i] = analogRead(US_POT_PINS[i]); // 0..1023
}

// ====== Read 3-position switch ======
static void US_readPage() {
  // Both inputs are pullup. A pressed/connected-to-GND reads LOW.
  bool s1 = (digitalRead(US_SWITCH_DIGITAL) == LOW);
  int  a0 = analogRead(US_SWITCH_ANALOG); // also pullup -> HIGH when open
  // Heuristic (stable and simple):
  // digital LOW dominates -> page 0
  // else analog below threshold -> page 1
  // else -> page 2
  if (s1)                     s_page = 0;
  else if (a0 < 341)          s_page = 1;
  else                        s_page = 2;
}

// ====== Public getters ======
bool UnoSynth::getKey(uint8_t i) { return (i < 8) ? s_keys[i] : false; }
bool UnoSynth::getFn (uint8_t i) { return (i < 4) ? s_fns[i]  : false; }
int  UnoSynth::getPot(uint8_t i) { return (i < 4) ? s_pots[i] : 0;     }
int  UnoSynth::getPage()         { return s_page;                       }

// ====== LEDs ======
void UnoSynth::setLED(uint8_t id, bool on) {
  if (id < 4) s_leds[id] = on;
}

// ====== Audio ======
void UnoSynth::playFreq(uint16_t hz) { tone(US_AUDIO_OUT_PIN, hz); }
void UnoSynth::stopAudio()           { noTone(US_AUDIO_OUT_PIN);   }

// ====== Update (call each loop) ======
void UnoSynth::update() {
  // Inputs first
  US_scanMatrix();
  US_readPots();
  US_readPage();

  // LEDs (multiplex Charlieplex — quick refresh for any ON led)
  // Very lightweight: show each ON LED for a few hundred microseconds
  for (int i = 0; i < 4; i++) {
    if (s_leds[i]) {
      US_showLED(i);
      delayMicroseconds(200);
      US_clearLEDs();
    }
  }
}
