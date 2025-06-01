// Pines - ya mapeados por ti
const int knobTempo = A3;

const int buttonStartStop = 13;
const int buttonChangeInst = 2;

const int keys[8] = {10, 8, 5, 1, 12, 7, 6, 0}; // key1 - key8
const int audioOut = 3; // PWM

// Estado
bool kickSteps[8] = {false};
bool snareSteps[8] = {false};
bool hatSteps[8] = {false};

int currentInstrument = 0; // 0: kick, 1: snare, 2: hat
int currentStep = 0;

bool isPlaying = false;
bool lastStartState = HIGH;
bool lastModeState = HIGH;

unsigned long lastStepTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(knobTempo, INPUT);

  pinMode(buttonStartStop, INPUT_PULLUP);
  pinMode(buttonChangeInst, INPUT_PULLUP);

  for (int i = 0; i < 8; i++) {
    pinMode(keys[i], INPUT_PULLUP);
  }

  pinMode(audioOut, OUTPUT);
}

void loop() {
  // Leer tempo
  int tempoRaw = analogRead(knobTempo);
  int bpm = map(tempoRaw, 0, 1023, 60, 180);
  unsigned long interval = 60000 / bpm / 8;

  // Start/Stop
  bool startState = digitalRead(buttonStartStop);
  if (startState == LOW && lastStartState == HIGH) {
    isPlaying = !isPlaying;
    delay(200);
  }
  lastStartState = startState;

  // Cambiar instrumento
  bool modeState = digitalRead(buttonChangeInst);
  if (modeState == LOW && lastModeState == HIGH) {
    currentInstrument = (currentInstrument + 1) % 3;
    delay(200);
  }
  lastModeState = modeState;

  // Leer pasos
  for (int i = 0; i < 8; i++) {
    if (digitalRead(keys[i]) == LOW) {
      switch (currentInstrument) {
        case 0: kickSteps[i] = !kickSteps[i]; break;
        case 1: snareSteps[i] = !snareSteps[i]; break;
        case 2: hatSteps[i] = !hatSteps[i]; break;
      }
      delay(150); // debounce simple
    }
  }

  // Avanzar secuencia
  if (isPlaying && millis() - lastStepTime >= interval) {
    lastStepTime = millis();

    if (kickSteps[currentStep]) playKick();
    if (snareSteps[currentStep]) playSnare();
    if (hatSteps[currentStep]) playHiHat();

    currentStep = (currentStep + 1) % 8;
  }
}

// 💥 Sonidos básicos

void playKick() {
  for (int f = 150; f >= 60; f -= 3) {
    tone(audioOut, f);
    delay(2);
  }
  noTone(audioOut);
}

void playSnare() {
  for (int i = 0; i < 30; i++) {
    tone(audioOut, random(2000, 4000));
    delay(1);
  }
  noTone(audioOut);
}

void playHiHat() {
  for (int i = 0; i < 10; i++) {
    tone(audioOut, random(5000, 8000));
    delay(1);
  }
  noTone(audioOut);
}
