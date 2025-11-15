#pragma once

// Button matrix: 3 rows x 4 cols
#define USYN_ROW0 10
#define USYN_ROW1 11
#define USYN_ROW2 12

#define USYN_COL0 5
#define USYN_COL1 6
#define USYN_COL2 7
#define USYN_COL3 8

// 3-way switch (two pins encoding 3 states)
#define USYN_SW_A 13
#define USYN_SW_B A0

// Potentiometers (Knobs)
#define USYN_POT0 A3
#define USYN_POT1 A4
#define USYN_POT2 A2
#define USYN_POT3 A1

// Audio jack for output
#define USYN_AUDIO_OUT 9

// Audio jack for input
#define USYN_AUDIO_IN A5

// Charlieplex LED pins
#define USYN_LED_A 2
#define USYN_LED_B 3
#define USYN_LED_C 4

// Matrix size constants
#define USYN_NUM_ROWS 3
#define USYN_NUM_COLS 4
#define USYN_NUM_BTNS (USYN_NUM_ROWS * USYN_NUM_COLS)
#define USYN_NUM_POTS 4
