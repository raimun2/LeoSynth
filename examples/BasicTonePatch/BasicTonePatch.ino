/*
  UnoSynth – BasicTonePatch
  - Uses tone() on D5
  - Pages (3-pos switch) change the scale:
      Page 0: Major, Page 1: Minor, Page 2: Control (no keys)
  - 8 keys trigger 8 notes from the selected scale
  - Fn buttons:
      fn[0] = Octave Up
      fn[1] = Octave Down
      fn[2] = Toggle Glide (placeholder)
      fn[3] = Mute
  - Pots:
      pot[0] = Note duration (ms)
      pot[1] = Glide time (placeholder)
      pot[2] = Volume placeholder (not used with tone)
      pot[3] = Vibrato depth (simple LFO on freq)
  - LEDs:
      LED0..2: show active page
      LED3: blink while note is playing
*/

#include <UnoSynth.h>

// ---- musical data ----
const int MAJOR_ST[8] = {0,2,4,5,7,9,11,12};
const int MINOR_ST[8] = {0,2,3,5,7,8,10,12};
int octave = 4;                 // start at C4
bool mute = false;
bool glideOn = false;           // placeholder

// timing
unsigned long noteStartMs = 0;
bool noteActive = false;
int lastKey = -1;

void setup() {
  UnoSynth.begin();
}

void loop() {
  UnoSynth.update();

  // Page LEDs
  for (int i=0;i<3;i++) UnoSynth::setLED(i, UnoSynth::getPage()==i);

  // Fn buttons
  if (UnoSynth::getFn(0)) octave = min(octave+1, 7);
  if (UnoSynth::getFn(1)) octave = max(octave-1, 1);
  if (UnoSynth::getFn(2)) glideOn = true;   // placeholder toggle could be added
  if (UnoSynth::getFn(3)) mute = true;      // momentary mute (for demo)

  // Pots
  int durMs   = map(UnoSynth::getPot(0), 0,1023, 50, 1200);
  int vibAmt  = map(UnoSynth::getPot(3), 0,1023, 0, 20); // +/- cents approx

  // Keys → notes
  int page = UnoSynth::getPage();
  int activeKey = -1;
  for (int k=0;k<8;k++) if (UnoSynth::getKey(k)) activeKey = k;

  if (page <= 1 && activeKey >= 0 && !mute) {
    // scale select
    const int* SCALE = (page==0) ? MAJOR_ST : MINOR_ST;
    int semis = SCALE[activeKey] + (octave - 4) * 12;
    // base A4=440 tuning
    float freq = 440.0f * powf(2.0f, (semis - 9) / 12.0f); // C4 = -9 semis from A4
    // vibrato: slow LFO on frequency
    float lfo = sinf(millis()*0.006f); // ~6 rad/s
    float vib = 1.0f + (vibAmt/1200.0f) * lfo; // small cents modulation
    UnoSynth::playFreq((uint16_t)(freq * vib));

    // LED activity
    UnoSynth::setLED(3, true);
    noteActive = true; noteStartMs = millis(); lastKey = activeKey;
  } else {
    // stop note after duration
    if (noteActive && millis() - noteStartMs > (unsigned long)map(UnoSynth::getPot(0), 0,1023, 50, 1200)) {
      UnoSynth::stopAudio();
      UnoSynth::setLED(3, false);
      noteActive = false;
      mute = false; // reset simple mute
      glideOn = false;
      lastKey = -1;
    }
  }
}
