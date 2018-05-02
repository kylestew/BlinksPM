#pragma once

#include <FastLED.h>

const uint8_t kMatrixWidth = 12;
const uint8_t kMatrixHeight = 24;
#define NUM_LEDS    		(kMatrixWidth * kMatrixHeight)
#define MAX_DIMENSION   ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)

struct Animation {
public:
  Animation(CRGB* leds) : leds{leds} {}

  virtual void step() = 0;

protected:
  CRGB* leds;

  uint16_t XY(uint8_t x, uint8_t y) {
    // TOP is 0

  	// strips are serpentine layout
  	if (x & 0x01) {
  		// backwards
  		return (x * kMatrixHeight) + (kMatrixHeight - 1 - y);
  	} else {
  		return (x * kMatrixHeight) + y;
  	}
  }

  uint16_t revXY(uint8_t x, uint8_t y) {
    // BOTTOM is 0

  	// strips are serpentine layout
  	if (x & 0x01) {
  		return (x * kMatrixHeight) + y;
  	} else {
  		// backwards
  		return (x * kMatrixHeight) + (kMatrixHeight - 1 - y);
  	}
  }

};
