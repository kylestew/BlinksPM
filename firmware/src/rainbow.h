#pragma once

#include "animation.h"

struct Rainbow : Animation {
public:
  Rainbow(CRGB* leds) : Animation(leds) {
    // pick a random speed and hue step
    // hue step how much the rainbow color shifts over each LED
    hueStep = random(2, 16);
    speed = (random(2000) / 500.0f) + 0.5f; // [0.5 - 3.5]
  }

  void step() override {
    // TODO: doesn't deal with surpentine
    fill_rainbow(leds, NUM_LEDS, hue, hueStep);

    addGlitter(80);

    hue += speed;
  }

private:
  float hue = 0;
  uint8_t hueStep = 0;
  float speed = 0.0f;


  void addGlitter(fract8 chanceOfGlitter) {
    if (random8() < chanceOfGlitter) {
      leds[ random16(NUM_LEDS) ] += CRGB::White;
    }
  }

};
