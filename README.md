UnoSynth – Hardware & Firmware Overview
=======================================

UnoSynth is a modular synthesis platform built entirely on the Arduino Uno.
It uses every available pin for musical and control purposes, with fixed hardware
and flexible software.

──────────────────────────────────────────────
Hardware Pin Configuration (Arduino Uno)
──────────────────────────────────────────────

Digital Pins:
  D0 / RX      → Switch 3-pos (Line A)           [INPUT_PULLUP]
  D1 / TX      → Switch 3-pos (Line B)           [INPUT_PULLUP]
  D2 - D5      → Button Matrix Rows (0–3)        [OUTPUT LOW]
  D6 - D8      → Button Matrix Columns (0–2)     [INPUT_PULLUP]
  D9           → Audio Out A (PWM OC1A)          [OUTPUT, 31 kHz PWM]
  D10          → Audio Out B (PWM OC1B)          [OPTIONAL]
  D11 - D13    → LED 1–3                          [OUTPUT]
  A1 (15)      → LED 4                            [OUTPUT]

Analog Pins:
  A0 (14)      → Audio IN (or alternative Out)   [ADC or PWM]
  A2 - A5      → Potentiometers 1–4              [ADC]

──────────────────────────────────────────────
Button Matrix Layout (4x3)
──────────────────────────────────────────────

Columns: D6 (Col 0), D7 (Col 1), D8 (Col 2)
Rows:    D2 (Row 0), D3 (Row 1), D4 (Row 2), D5 (Row 3)

  ┌────────────┬────────────┬────────────┐
  │ B0 (D2↔D6) │ K0 (D2↔D7) │ K1 (D2↔D8) │
  ├────────────┼────────────┼────────────┤
  │ B1 (D3↔D6) │ K2 (D3↔D7) │ K3 (D3↔D8) │
  ├────────────┼────────────┼────────────┤
  │ B2 (D4↔D6) │ K4 (D4↔D7) │ K5 (D4↔D8) │
  ├────────────┼────────────┼────────────┤
  │ B3 (D5↔D6) │ K6 (D5↔D7) │ K7 (D5↔D8) │
  └────────────┴────────────┴────────────┘

──────────────────────────────────────────────
Audio Output – Filtered + Amplified (Mono)
──────────────────────────────────────────────

• Pins 9 and 10: PWM Audio signals
• Passive Filter: 1 kΩ resistor + 10 µF capacitor (low-pass)
• Merge both channels before amplification
• Amplifier: LM386 (configured for ×20 gain)
• Output: Line-level or headphones (via jack)

LM386 BASIC SETUP:
  - Pin 3: Audio Input (via RC filtered signal)
  - Pin 2 & 4: GND
  - Pin 5: Audio Output → Capacitor → Tip of Jack
  - Pin 6: VCC (+5V)
  - Pin 7: Bypass (optional cap to GND)
  - Optional: 10 µF cap between Pin 1 & 8 to increase gain (×200)

──────────────────────────────────────────────
Firmware
──────────────────────────────────────────────

Each sketch (.ino) defines a different module:
  • Oscillator
  • Sampler
  • FX Unit
  • Envelope / LFO

UnoSynth = fixed hardware, infinite software-defined personalities.

