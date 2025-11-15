#include "LeoSynth.h"

static const uint16_t DEBOUNCE_MS = 10;
static const uint8_t  POT_ALPHA_Q = 4;

uint8_t LeoSynth::btnState_[USYN_NUM_BTNS] = {0};
uint8_t LeoSynth::btnPrev_[USYN_NUM_BTNS] = {0};
uint32_t LeoSynth::btnTs_[USYN_NUM_BTNS] = {0};

uint16_t LeoSynth::potsRaw_[USYN_NUM_POTS] = {0};
uint16_t LeoSynth::potsSm_[USYN_NUM_POTS] = {0};

LeoSynth::SwitchState LeoSynth::swState_ = SW_POS2;
uint32_t LeoSynth::beepUntilMs_ = 0;

// ===== Logical pad/control mapping =====
static const uint8_t PAD_MAP[8]  = {9,8,10,11,5,4,6,7};  // Verified hardware layout
static const uint8_t CTRL_MAP[4] = {0,1,2,3};            // Top row buttons

void LeoSynth::begin() {
  pinMode(USYN_ROW0, OUTPUT); pinMode(USYN_ROW1, OUTPUT); pinMode(USYN_ROW2, OUTPUT);
  digitalWrite(USYN_ROW0, HIGH); digitalWrite(USYN_ROW1, HIGH); digitalWrite(USYN_ROW2, HIGH);

  pinMode(USYN_COL0, INPUT_PULLUP); pinMode(USYN_COL1, INPUT_PULLUP);
  pinMode(USYN_COL2, INPUT_PULLUP); pinMode(USYN_COL3, INPUT_PULLUP);

  pinMode(USYN_SW_A, INPUT_PULLUP);
  pinMode(USYN_SW_B, INPUT_PULLUP);

  pinMode(USYN_AUDIO_OUT, OUTPUT);
  noTone(USYN_AUDIO_OUT);

  ledClear();
}

void LeoSynth::update() {
  scanButtons_();
  scanSwitch_();
  scanPots_();

  if (beepUntilMs_ && millis() >= beepUntilMs_) {
    noTone(USYN_AUDIO_OUT);
    beepUntilMs_ = 0;
  }
}

bool LeoSynth::button(uint8_t index) {
  return index < USYN_NUM_BTNS ? btnState_[index] : false;
}

bool LeoSynth::buttonEdge(uint8_t index) {
  if (index >= USYN_NUM_BTNS) return false;
  bool edge = (btnState_[index] && !btnPrev_[index]);
  btnPrev_[index] = btnState_[index];
  return edge;
}

// ===== PAD / CTRL MAPPING =====
bool LeoSynth::pad(uint8_t index) {
  if (index >= 8) return false;
  return button(PAD_MAP[index]);
}

bool LeoSynth::padEdge(uint8_t index) {
  if (index >= 8) return false;
  return buttonEdge(PAD_MAP[index]);
}

bool LeoSynth::ctrl(uint8_t index) {
  if (index >= 4) return false;
  return button(CTRL_MAP[index]);
}

bool LeoSynth::ctrlEdge(uint8_t index) {
  if (index >= 4) return false;
  return buttonEdge(CTRL_MAP[index]);
}

LeoSynth::SwitchState LeoSynth::triSwitch() {
  return swState_;
}

uint16_t LeoSynth::potRaw(uint8_t index) {
  return index < USYN_NUM_POTS ? potsRaw_[index] : 0;
}

uint16_t LeoSynth::potSmooth(uint8_t index) {
  return index < USYN_NUM_POTS ? potsSm_[index] : 0;
}

uint16_t LeoSynth::audioIn() {
  return analogRead(USYN_AUDIO_IN);
}

void LeoSynth::ledClear() {
  pinMode(USYN_LED_A, INPUT);
  pinMode(USYN_LED_B, INPUT);
  pinMode(USYN_LED_C, INPUT);
}

void LeoSynth::setLEDPair_(uint8_t id) {
  switch (id) {
    case 0: pinMode(USYN_LED_A, OUTPUT); digitalWrite(USYN_LED_B, HIGH); break;
    case 1: pinMode(USYN_LED_B, OUTPUT); digitalWrite(USYN_LED_A, HIGH); break;
    case 2: pinMode(USYN_LED_A, OUTPUT); digitalWrite(USYN_LED_C, HIGH); break;
    case 3: pinMode(USYN_LED_C, OUTPUT); digitalWrite(USYN_LED_A, HIGH); break;
  }
}

void LeoSynth::ledSet(uint8_t id) {
  ledClear();
  setLEDPair_(id);
}

void LeoSynth::ledFlash(uint8_t id, uint16_t ms) {
  ledSet(id);
  delay(ms);
  ledClear();
}

void LeoSynth::beep(uint16_t freq, uint16_t ms) {
  tone(USYN_AUDIO_OUT, freq);
  beepUntilMs_ = millis() + ms;
}

void LeoSynth::audioOff() {
  noTone(USYN_AUDIO_OUT);
  beepUntilMs_ = 0;
}

void LeoSynth::scanButtons_() {
  const uint8_t rows[3] = {USYN_ROW2, USYN_ROW0, USYN_ROW1};
  const uint8_t cols[4] = {USYN_COL0, USYN_COL1, USYN_COL2, USYN_COL3};

  for (uint8_t r = 0; r < 3; r++) {
    digitalWrite(USYN_ROW0, HIGH); digitalWrite(USYN_ROW1, HIGH); digitalWrite(USYN_ROW2, HIGH);
    digitalWrite(rows[r], LOW);

    for (uint8_t c = 0; c < 4; c++) {
      uint8_t i = r * 4 + c;
      bool pressed = (digitalRead(cols[c]) == LOW);
      uint32_t now = millis();
      if (pressed != btnState_[i] && (now - btnTs_[i]) >= DEBOUNCE_MS) {
        btnState_[i] = pressed;
        btnTs_[i] = now;
      }
    }
  }
}

void LeoSynth::scanSwitch_() {
  bool a = digitalRead(USYN_SW_A) == LOW;
  bool b = digitalRead(USYN_SW_B) == LOW;
  if (a && !b) swState_ = SW_POS0;
  else if (!a && b) swState_ = SW_POS1;
  else swState_ = SW_POS2;
}

void LeoSynth::scanPots_() {
  const uint8_t pins[4] = {USYN_POT0, USYN_POT1, USYN_POT2, USYN_POT3};
  for (uint8_t i = 0; i < 4; i++) {
    uint16_t v = analogRead(pins[i]);
    potsRaw_[i] = v;
    potsSm_[i] = potsSm_[i] + ((int32_t)v - (int32_t)potsSm_[i]) / (1 << POT_ALPHA_Q);
  }
}
