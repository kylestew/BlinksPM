#pragma once

#include "animation_base.h"

struct Noise : Animation {
public:
  Noise(CRGB* leds) : Animation(leds) {
    x = random16();
    y = random16();
    z = random16();
  }

  void step() override {
    fillnoise8();

  	for (int i = 0; i < kMatrixWidth; i++) {
  		for (int j = 0; j < kMatrixHeight; j++) {
        leds[XY(i, j)] = CHSV(noise[j][i],255,noise[i][j]);
  		}
  	}
  }


private:
  uint16_t x, y, z;

  // This is the array that we keep our computed noise values in
  uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

  // We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
  // use the z-axis for "time".  speed determines how fast time moves forward.  Try
  // 1 for a very slow moving effect, or 60 for something that ends up looking like
  // water.
  uint16_t speed = 1; // almost looks like a painting, moves very slowly
  // uint16_t speed = 20; // a nice starting speed, mixes well with a scale of 100
  // uint16_t speed = 33;
  // uint16_t speed = 100; // wicked fast!

  // Scale determines how far apart the pixels in our noise matrix are.  Try
  // changing these values around to see how it affects the motion of the display.  The
  // higher the value of scale, the more "zoomed out" the noise iwll be.  A value
  // of 1 will be so zoomed in, you'll mostly see solid colors.
  // uint16_t scale = 1; // mostly just solid colors
  // uint16_t scale = 4011; // very zoomed out and shimmery
  uint16_t scale = 311;

  void fillnoise8() {
  	for (int i = 0; i < MAX_DIMENSION; i++) {
  		int iOffset = scale * i;
  		for (int j = 0; j < MAX_DIMENSION; j++) {
  			int jOffset = scale * j;
  			noise[i][j] = inoise8(x + iOffset, y + jOffset, z);
  		}
  	}
  	z += speed;
  }

};
