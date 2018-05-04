#pragma once

#include "animation.h"

#include <FastLED.h>

struct Noise : Animation {
public:
  Noise(OctoWS2811* leds) : Animation(leds) {
    x = random16();
    y = random16();
    z = random16();

    // We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
    // use the z-axis for "time".  speed determines how fast time moves forward.  Try
    // 1 for a very slow moving effect, or 60 for something that ends up looking like
    // water.
    speed = random(3, 12);

    // Scale determines how far apart the pixels in our noise matrix are.  Try
    // changing these values around to see how it affects the motion of the display.  The
    // higher the value of scale, the more "zoomed out" the noise iwll be.  A value
    // of 1 will be so zoomed in, you'll mostly see solid colors.
    // uint16_t scale = 1; // mostly just solid colors
    // uint16_t scale = 4011; // very zoomed out and shimmery
    scale = random(12, 128);

    // random select a palette
    int choice = random(8);
    if (choice == 0) { currentPalette = RainbowColors_p; }
    if (choice == 1) { currentPalette = ForestColors_p; }
    if (choice == 2) { currentPalette = CloudColors_p; }
    if (choice == 3) { currentPalette = LavaColors_p; }
    if (choice == 4) { currentPalette = OceanColors_p; }
    if (choice == 5) { currentPalette = PartyColors_p; }
    if (choice == 6) { currentPalette = RainbowStripeColors_p; }
    if (choice == 7) {
      // 'black out' all 16 palette entries...
      fill_solid( currentPalette, 16, CRGB::Black);
      // and set every fourth one to white.
      currentPalette[0] = CRGB::White;
      currentPalette[4] = CRGB::White;
      currentPalette[8] = CRGB::White;
      currentPalette[12] = CRGB::White;
    }
    if (choice == 8 ) { currentPalette = ForestColors_p; }
  }

  void step() override {
    fillnoise8();

  	for (int i = 0; i < kMatrixWidth; i++) {
  		for (int j = 0; j < kMatrixHeight; j++) {
        uint8_t index = noise[j][i];
        uint8_t bri =   noise[i][j];

        // brighten up, as the color palette itself often contains the
        // light/dark dynamic range desired
        if( bri > 127 ) {
          bri = 255;
        } else {
          bri = dim8_raw( bri * 2);
        }

        CRGB color = ColorFromPalette( currentPalette, index, bri );
        setPixel(i, j, color.r, color.g, color.b);
  		}
  	}
  }


private:
  uint16_t x, y, z;
  uint16_t speed;
  uint16_t scale;

  // This is the array that we keep our computed noise values in
  uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

  CRGBPalette16 currentPalette;

  // Fill the x/y array of 8-bit noise values using the inoise8 function.
  void fillnoise8() {
    // If we're runing at a low "speed", some 8-bit artifacts become visible
    // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
    // The amount of data smoothing we're doing depends on "speed".
    uint8_t dataSmoothing = 0;
    if( speed < 10) {
      dataSmoothing = 200 - (speed * 8);
    }

    for(int i = 0; i < MAX_DIMENSION; i++) {
      int ioffset = scale * i;
      for(int j = 0; j < MAX_DIMENSION; j++) {
        int joffset = scale * j;

        uint8_t data = inoise8(x + ioffset,y + joffset,z);

        // The range of the inoise8 function is roughly 16-238.
        // These two operations expand those values out to roughly 0..255
        // You can comment them out if you want the raw noise data.
        data = qsub8(data,16);
        data = qadd8(data,scale8(data,39));

        if( dataSmoothing ) {
          uint8_t olddata = noise[i][j];
          uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
          data = newdata;
        }

        noise[i][j] = data;
      }
    }

    z += speed;

    // apply slow drift to X and Y, just for visual variation.
    x += speed / 8;
    y -= speed / 16;
  }

};
