#pragma once

#include "animation.h"

struct Sandbox : Animation {
public:
  Sandbox(CRGB* leds) : Animation(leds) {
  }

  void step() override {
    // // fade all others
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].nscale8(250);
    }

    // light current strip we are on to full
    fill_solid(leds + kMatrixHeight * stripIdx, kMatrixHeight, CRGB(255, 0, 255));

    // sweep time = 2s
    if (++tick > 40) {
      tick = 0;
      if (++stripIdx >= kMatrixWidth) stripIdx = 0;
    }
  }

private:
  int stripIdx = 0;
  uint8_t tick = 0;

};
