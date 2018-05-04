#pragma once

#include "animation.h"

#include "FireColor.h"

struct Fire : Animation {
public:
  Fire(OctoWS2811* leds) : Animation(leds) {
    // copy in fire colors from function
    unsigned int* colors = fireColor_1p0();
    for (int i = 0; i < 100; i++) {
      fireColor[i] = colors[i];
    }
    delete colors;

    focus = random(8, 18);
    cool = random(20, 40);
  }

  void step() override {
    if (msec >= 40) {
      msec = 0;
      animateFire();
    }
  }

private:
  unsigned int heat = kMatrixWidth / 5;
  unsigned int focus = 12;
  unsigned int cool = 30;

  unsigned char canvas[kMatrixWidth * kMatrixHeight];
  unsigned int fireColor[100];
  elapsedMillis msec;

  void animateFire() {
    unsigned int i, c, n, x, y;

    // Step 1: move all data up one line
    memmove(canvas + kMatrixWidth, canvas, kMatrixWidth * (kMatrixHeight - 1));
    memset(canvas, 0, kMatrixWidth);

    // Step 2: draw random heatspots on bottom line
    i = heat;
    while (i > 0) {
      x = random(kMatrixWidth) + 1;
      if (canvas[x] == 0) {
        canvas[x] = 99;
        i--;
      }
    }

    // Step 3: interpolate
    for (y=0; y < kMatrixHeight; y++) {
      for (x=0; x < kMatrixWidth; x++) {
        c = canvas[y * kMatrixWidth + x] * focus;
        n = focus;
        if (x > 0) {
          c = c + canvas[y * kMatrixWidth + (x - 1)];
          n = n + 1;
        }
        if (x < kMatrixWidth-1) {
          c = c + canvas[y * kMatrixWidth + (x + 1)];
          n = n + 1;
        }
        if (y > 0) {
          c = c + canvas[(y -1) * kMatrixWidth + x];
          n = n + 1;
        }
        if (y < kMatrixHeight-1) {
          c = c + canvas[(y + 1) * kMatrixWidth + x];
          n = n + 1;
        }
        c = (c + (n / 2)) / n;
        i = (random(1000) * cool) / 10000;
        if (c > i) {
          c = c - i;
        } else {
          c = 0;
        }
        canvas[y * kMatrixWidth + x] = c;
      }
    }

    // Step 4: render canvas to LEDs
    for (y=0; y < kMatrixHeight; y++) {
      for (x=0; x < kMatrixWidth; x++) {
        c = canvas[((kMatrixHeight - 1) - y) * kMatrixWidth + x];
        leds->setPixel(XY(x, y), fireColor[c]);
      }
    }
  }

};
