# LeoSynth
DIY sound synthesis framework.

LeoSynth is an open-source Arduino-based synthesiser platform. It is designed to learn sound synthesis by building real hardware and writing simple synth code. No black boxes, no expensive gear, just pure exploration.

```
LeoSynth
├── Hardware    – build your own LeoSynth with simple components
├── Library     – LeoSynth Hardware Abstraction Layer/API to control the hardware
└── Sketches    – Several engines for sound synthesis/midi control, feel free to contribute with your own!
```

## Overview
LeoSynth gives you:
- A fixed reproducible open hardware layout anyone can build
- A lean Arduino library that handles all hardware I/O
- Example synth sketches that focus only on sound logic

The hardware design is fixed. No pin changes, no wiring surprises and no dependency hell. Everything is simple, clear and hackable.

## Hardware
The standard LeoSynth board includes:
- 12-button matrix
- 3-way mode switch
- 4 analogue potentiometers
- 3 Charlieplexed LEDs
- Audio output (PWM/tone-based to start)
- Analog input (which can also be used for digital input)
- Arduino Leonardo as the synth engine

Full hardware documentation will be available in this repository.

## Library
The LeoSynth Arduino library handles:
- Button matrix scanning
- Switch input
- Analogue pot reading with smoothing
- Charlieplex LED routing
- Audio output helper
- Stable update cycle per loop

You write sound logic only. LeoSynth handles the rest.

Example:

```
#include <LeoSynth.h>

void setup() {
  LeoSynth::begin();
}

void loop() {
  LeoSynth::update();
  if (LeoSynth::buttonEdge(0)) {
    LeoSynth::beep(440, 120);
  }
}
```

## Examples
The examples folder contains synth sketches built using the LeoSynth API. Each sketch is a standalone sound engine, from simple beeps to evolving textures. Community sketches will be added over time.

## Install
1. Download or clone this repository
2. Place the LeoSynth folder inside your Arduino libraries directory
3. Restart Arduino IDE
4. Go to File > Examples > LeoSynth to load the example sketch

## Roadmap
- Hardware assembly guide
- Multiple synthesis engines
- Envelope and modulation tools
- Audio tables and noise generators
- Community sound library

## Project Link
https://github.com/raimun2/LeoSynth

## License
Public domain. See LICENSE.
