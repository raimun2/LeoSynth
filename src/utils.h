#pragma once
#include <Arduino.h>

// Small helpers (reserved for future use)
// Debounce, map floats, etc. — kept minimal for now.
inline uint16_t clampu16(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}
