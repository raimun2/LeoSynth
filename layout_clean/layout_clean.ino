/*
  UnoSynth – Pin Mapping

  Author: Raimundo (adapted by ChatGPT)
  Date:   June 2025

  Pin assignments:

    D0  (RX)       → Selector 3-pos, line A  (INPUT_PULLUP)
    D1  (TX)       → Selector 3-pos, line B  (INPUT_PULLUP)

    D2              → Button matrix ROW 0 (OUTPUT)
    D3 (OC2B)       → Button matrix ROW 1 (OUTPUT)
    D4              → Button matrix ROW 2 (OUTPUT)
    D5              → Button matrix ROW 3 (OUTPUT)

    D6 (OC0A)       → Button matrix COL 0 (INPUT_PULLUP)
    D7              → Button matrix COL 1 (INPUT_PULLUP)
    D8              → Button matrix COL 2 (INPUT_PULLUP)

    D9 (OC1A) PWM   → Audio Out A (31 kHz PWM via Timer-1)
    D10 (OC1B) PWM  → Audio Out B (optional stereo)

    D11 (OC2A) PWM  → LED 1
    D12             → LED 2
    D13 (+ onboard) → LED 3

    A0 (ADC0)       → Audio IN  or Audio Out C via tone()
    A1 (ADC1)       → LED 4

    A2 (ADC2)       → Pot 1 (Cutoff / Param 1)
    A3 (ADC3)       → Pot 2 (Reson / Param 2)
    A4 (SDA/ADC4)   → Pot 3 (Env / Param 3)
    A5 (SCL/ADC5)   → Pot 4 (LFO / Param 4)
*/

// ─────────────────────────────────────────────────────────────────────────────
//   PIN DEFINITIONS
// ─────────────────────────────────────────────────────────────────────────────

// 3-position switch
const uint8_t PIN_SW_A = 0;   // D0 (RX)
const uint8_t PIN_SW_B = 1;   // D1 (TX)

// Button matrix (4 rows × 3 columns)
const uint8_t ROW_PINS[4] = { 2, 3, 4, 5 };   // D2, D3, D4, D5  (OUTPUT)
const uint8_t COL_PINS[3] = { 6, 7, 8 };      // D6, D7, D8        (INPUT_PULLUP)

// Audio outputs
const uint8_t PIN_AUDIO_A = 9;   // D9 (Timer-1 PWM @ ~31 kHz)
const uint8_t PIN_AUDIO_B = 10;  // D10 (Timer-1 PWM @ ~31 kHz)
// Optional: use tone() on A0 for a third voice if desired
const uint8_t PIN_AUDIO_C = A0;  // A0 (ADC0/tone())

// LEDs
const uint8_t PIN_LED_1 = 11;  // D11 (OC2A PWM)
const uint8_t PIN_LED_2 = 12;  // D12
const uint8_t PIN_LED_3 = 13;  // D13 (+ on-board LED)
const uint8_t PIN_LED_4 = A1;  // A1 (ADC1 pin used as digital OUTPUT)

// Pots (analog inputs)
const uint8_t PIN_POT_1 = A2;  // Cutoff / Param 1
const uint8_t PIN_POT_2 = A3;  // Reson / Param 2
const uint8_t PIN_POT_3 = A4;  // Env / Param 3
const uint8_t PIN_POT_4 = A5;  // LFO / Param 4

// ─────────────────────────────────────────────────────────────────────────────
//   GLOBAL STATE
// ─────────────────────────────────────────────────────────────────────────────

// Button matrix state (true = pressed)
bool buttonState[4][3] = { {false} };

// Last scan timestamp
unsigned long lastScanTime = 0;
const unsigned long SCAN_INTERVAL_MS = 5;  // debounce / scanning interval

// Pot values (0–1023)
uint16_t potValues[4] = { 0, 0, 0, 0 };

// Switch position enum
enum SwitchPos {
  SW_POS_UP = 0,
  SW_POS_MID,
  SW_POS_DOWN,
  SW_POS_UNKNOWN
};
SwitchPos currentSwitch = SW_POS_UNKNOWN;

// Placeholder for audio phase / envelope state, etc.
// (fill in with your synth internals)
struct SynthVoice {
  // e.g. phase accumulator, envelope stage, etc.
};
SynthVoice voiceA, voiceB, voiceC;  // up to 3 voices

// ─────────────────────────────────────────────────────────────────────────────
//   SETUP()
//
//   Configure pinModes and any timers for fast PWM.
// ─────────────────────────────────────────────────────────────────────────────
void setup() {
  // Initialize Serial for debugging if needed (comment out if not used)
  Serial.begin(115200);

  // 3-pos switch pins
  pinMode(PIN_SW_A, INPUT_PULLUP);
  pinMode(PIN_SW_B, INPUT_PULLUP);

  // Button matrix rows as OUTPUTs (we’ll drive them one at a time LOW)
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(ROW_PINS[i], OUTPUT);
    digitalWrite(ROW_PINS[i], HIGH);  // idle HIGH (so no column reads LOW)
  }

  // Button matrix columns as INPUT_PULLUP
  for (uint8_t j = 0; j < 3; j++) {
    pinMode(COL_PINS[j], INPUT_PULLUP);
  }

  // Audio pins as OUTPUT
  pinMode(PIN_AUDIO_A, OUTPUT);
  pinMode(PIN_AUDIO_B, OUTPUT);
  // PIN_AUDIO_C (A0) will be used by tone(), no need to pinMode()

  // LEDs as OUTPUT
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);

  // Pot pins are automatically inputs, no need to pinMode(A2–A5)

  // (Optional) Configure Timer-1 for 31 kHz fast PWM on D9/D10 if you want
  // to bypass tone() and generate your own waveform. Example:
  //
  //  // Timer1: Phase-correct PWM, prescaler 1 → ~31 kHz on OC1A/OC1B
  //  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
  //  TCCR1B = _BV(WGM12) | _BV(CS10);
  //
  // If you prefer all audio via tone(), just rely on tone() calls.

  // Initial LED state OFF
  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_LED_2, LOW);
  digitalWrite(PIN_LED_3, LOW);
  digitalWrite(PIN_LED_4, LOW);

  // Record initial scan time
  lastScanTime = millis();
}

// ─────────────────────────────────────────────────────────────────────────────
//   LOOP()
//
//   - Scan switch
//   - Scan button matrix
//   - Read pots
//   - Update synth parameters
//   - Drive LEDs / audio
// ─────────────────────────────────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  // 1) SAMPLE THE 3-POSITION SWITCH
  readSwitch();

  // 2) SCAN BUTTON MATRIX (every SCAN_INTERVAL_MS ms)
  if (now - lastScanTime >= SCAN_INTERVAL_MS) {
    lastScanTime = now;
    scanButtonMatrix();
  }

  // 3) READ POTENTIOMETERS
  readPots();

  // 4) UPDATE SYNTH PARAMETERS based on switch, buttons, pots, etc.
  updateSynthParameters();

  // 5) GENERATE OR UPDATE AUDIO
  //    (e.g. call tone() or directly write PWM duty)
  generateAudio();

  // 6) UPDATE LEDS (e.g. show envelope or activity)
  updateLEDs();
}

// ─────────────────────────────────────────────────────────────────────────────
//   readSwitch()
//   Reads PIN_SW_A and PIN_SW_B to determine 3-position state.
// ─────────────────────────────────────────────────────────────────────────────
void readSwitch() {
  bool a = digitalRead(PIN_SW_A) == LOW;  // pulled LOW when closed?
  bool b = digitalRead(PIN_SW_B) == LOW;

  SwitchPos newPos;
  if (a && !b) {
    newPos = SW_POS_UP;    // e.g. top position
  } else if (!a && !b) {
    newPos = SW_POS_MID;   // middle position
  } else if (!a && b) {
    newPos = SW_POS_DOWN;  // bottom position
  } else {
    newPos = SW_POS_UNKNOWN;
  }

  if (newPos != currentSwitch) {
    currentSwitch = newPos;
    // Optionally handle switch-change event here:
    // onSwitchChanged(newPos);
  }
}

// ─────────────────────────────────────────────────────────────────────────────
//   scanButtonMatrix()
//   Drives each row LOW one by one, reads columns, debounces.
//   Updates buttonState[row][col] = true/false.
// ─────────────────────────────────────────────────────────────────────────────
void scanButtonMatrix() {
  for (uint8_t row = 0; row < 4; row++) {
    // Drive only this row LOW; keep others HIGH
    for (uint8_t r = 0; r < 4; r++) {
      digitalWrite(ROW_PINS[r], (r == row ? LOW : HIGH));
    }

    // Small delay to let line settle
    // (very short, but required if scanning fast)
    delayMicroseconds(5);

    // Read each column: LOW means the button at (row,col) is pressed
    for (uint8_t col = 0; col < 3; col++) {
      bool pressed = (digitalRead(COL_PINS[col]) == LOW);
      buttonState[row][col] = pressed;
      // You can add debouncing logic here if needed
    }
  }

  // After scanning all rows, drive all rows HIGH again
  for (uint8_t r = 0; r < 4; r++) {
    digitalWrite(ROW_PINS[r], HIGH);
  }
}

// ─────────────────────────────────────────────────────────────────────────────
//   readPots()
//   Reads A2–A5 into potValues[0..3].
// ─────────────────────────────────────────────────────────────────────────────
void readPots() {
  potValues[0] = analogRead(PIN_POT_1);  // 0…1023
  potValues[1] = analogRead(PIN_POT_2);
  potValues[2] = analogRead(PIN_POT_3);
  potValues[3] = analogRead(PIN_POT_4);
}

// ─────────────────────────────────────────────────────────────────────────────
//   updateSynthParameters()
//   Map potValues, switch state, buttons → synth engine parameters.
//   (fill in your own logic)
// ─────────────────────────────────────────────────────────────────────────────
void updateSynthParameters() {
  // Example placeholder:
  // float cutoff   = map(potValues[0], 0, 1023, 0, 1000) / 1000.0;  // 0.0–1.0
  // float resonance = map(potValues[1], 0, 1023, 0, 100) / 100.0;   // 0.0–1.0
  // float envAmt    = map(potValues[2], 0, 1023, 0, 1);
  // float lfoRate   = map(potValues[3], 0, 1023, 0, 20);  // 0–20 Hz
  //
  // // Switch might select waveform or mode
  // switch(currentSwitch) {
  //   case SW_POS_UP:    /* e.g. env on / off */ break;
  //   case SW_POS_MID:   /* e.g. LFO shape */ break;
  //   case SW_POS_DOWN:  /* e.g. filter mode */ break;
  //   default: break;
  // }
  //
  // // Buttons: you can trigger notes or cycle scales/etc.
  // for (uint8_t r = 0; r < 4; r++) {
  //   for (uint8_t c = 0; c < 3; c++) {
  //     if (buttonState[r][c]) {
  //       // e.g. noteIndex = r*3 + c; triggerNote(noteIndex);
  //     }
  //   }
  // }

  // … your synth parameter logic here …
}

// ─────────────────────────────────────────────────────────────────────────────
//   generateAudio()
//   Generates or updates audio on D9/D10 (and/or A0).
//   If you use tone(), it will take over Timer-2 or Timer-3, so be careful.
// ─────────────────────────────────────────────────────────────────────────────
void generateAudio() {
  // Example using tone() on D9 and D10:

  // float freqA = /* compute frequency for voice A */;
  // float freqB = /* compute frequency for voice B */;

  // if (/* voice A should be on */) {
  //   tone(PIN_AUDIO_A, (unsigned int)freqA);
  // } else {
  //   noTone(PIN_AUDIO_A);
  // }

  // if (/* voice B should be on */) {
  //   tone(PIN_AUDIO_B, (unsigned int)freqB);
  // } else {
  //   noTone(PIN_AUDIO_B);
  // }

  // // Optional voice C on A0:
  // float freqC = /* compute frequency for voice C */;
  // if (/* voice C should be on */) {
  //   tone(PIN_AUDIO_C, (unsigned int)freqC);
  // } else {
  //   noTone(PIN_AUDIO_C);
  // }

  // If you prefer direct PWM (31 kHz) on D9/D10, comment out tone() and
  // use Timer-1 to set OCR1A/OCR1B for duty cycle. Example:
  //
  // uint8_t dutyA = (uint8_t)(/* envelopeA * 255 */);
  // OCR1A = dutyA;
  //
  // uint8_t dutyB = (uint8_t)(/* envelopeB * 255 */);
  // OCR1B = dutyB;

  // … your waveform‐generation logic here …
}

// ─────────────────────────────────────────────────────────────────────────────
//   updateLEDs()
//   Drive the four LEDs based on envelope/activity or status.
// ─────────────────────────────────────────────────────────────────────────────
void updateLEDs() {
  // Example placeholders:

  // LED1 (D11) → show voice A activity
  // bool ledStateA = (/* voice A is active? */);
  // digitalWrite(PIN_LED_1, ledStateA ? HIGH : LOW);

  // LED2 (D12) → show voice B activity
  // bool ledStateB = (/* voice B is active? */);
  // digitalWrite(PIN_LED_2, ledStateB ? HIGH : LOW);

  // LED3 (D13) → show master MIDI or error
  // digitalWrite(PIN_LED_3, /*HIGH or LOW*/);

  // LED4 (A1) → “heartbeat” or LFO indicator
  // digitalWrite(PIN_LED_4, /*HIGH/LOW*/);

  // … your LED logic here …
}
