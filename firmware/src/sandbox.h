#pragma once

#include "animation.h"

struct Sandbox : Animation {
public:
  Sandbox(OctoWS2811* leds) : Animation(leds) {
    for (int i=0; i<180; i++) {
      int hue = i * 2;
      int saturation = 100;
      int lightness = 2;
      // pre-compute the 180 rainbow colors
      rainbowColors[i] = makeColor(hue, saturation, lightness);
    }
  }

  void step() override {
    rainbow(24);
  }

private:
  int rainbowColors[180];
  int color = 0;

  // phaseShift is the shift between each row.  phaseShift=0
  // causes all rows to show the same colors moving together.
  // phaseShift=180 causes each row to be the opposite colors
  // as the previous.
  //
  // cycleTime is the number of milliseconds to shift through
  // the entire 360 degrees of the color wheel:
  // Red -> Orange -> Yellow -> Green -> Blue -> Violet -> Red
  //
  void rainbow(int phaseShift) {
    int x, y;
    for (x = 0; x < kMatrixWidth; x++) {
      for (y = 0; y < kMatrixHeight; y++) {
        int index = (color + x + y*phaseShift/2) % 180;
        leds->setPixel(XY(x, y), rainbowColors[index]);
      }
    }
    color++;
  }

};
