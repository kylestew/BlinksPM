#include <FastLED.h>
#include "TM1637Display.h"

#define SERIAL_DEBUG     true

// == LEDS ==
#define NUM_LEDS    300
#define DATA_PIN    2
// CRGB leds [NUM_LEDS];
// int brightness = 120;

// == 7x4 SEG DISPLAY
#define DISP_CLK    3
#define DISP_DIO    4
TM1637Display display(DISP_CLK, DISP_DIO);

const uint8_t SEG_DONE[] = {
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
	SEG_C | SEG_E | SEG_G,                           // n
	SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
	};
#define TEST_DELAY   2000



void setup() {
  if (SERIAL_DEBUG) {
    Serial.begin(115200);
  }

  // // LEDs
  // pinMode(DATA_PIN, OUTPUT);
  // pinMode(CLOCK_PIN, OUTPUT);
  // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
	// FastLED.setBrightness(brightness);
  // FastLED.setCorrection(Typical8mmPixel);
  // FastLED.setTemperature(FullSpectrumFluorescent);
  // fill_solid(&(leds[0]), NUM_LEDS, CRGB(0, 0, 0));
  // FastLED.clear();
  //
  // // Master
  // Serial1.begin(115200);
  //
  // // clear LEDs
  // fill_solid(&(leds[0]), NUM_LEDS, CRGB(0, 0, 0));
  // FastLED.show();
  //
  // // prep for animation
  // reset();
}

void loop() {
  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  display.setBrightness(0x0f);

  // All segments on
  display.setSegments(data);
  delay(TEST_DELAY);

  // Selectively set different digits
  data[0] = 0b01001001;
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);

  for(k = 3; k >= 0; k--) {
  display.setSegments(data, 1, k);
  delay(TEST_DELAY);
  }

  display.setSegments(data+2, 2, 2);
  delay(TEST_DELAY);

  display.setSegments(data+2, 2, 1);
  delay(TEST_DELAY);

  display.setSegments(data+1, 3, 1);
  delay(TEST_DELAY);


  // Show decimal numbers with/without leading zeros
  bool lz = false;
  for (uint8_t z = 0; z < 2; z++) {
  for(k = 0; k < 10000; k += k*4 + 7) {
    display.showNumberDec(k, lz);
    delay(TEST_DELAY);
  }
  lz = true;
  }

  // Show decimal number whose length is smaller than 4
  for(k = 0; k < 4; k++)
  data[k] = 0;
  display.setSegments(data);

  // Run through all the dots
  for(k=0; k <= 4; k++) {
    display.showNumberDecEx(0, (0x80 >> k), true);
    delay(TEST_DELAY);
  }

  display.showNumberDec(153, false, 3, 1);
  delay(TEST_DELAY);
  display.showNumberDec(22, false, 2, 2);
  delay(TEST_DELAY);
  display.showNumberDec(0, true, 1, 3);
  delay(TEST_DELAY);
  display.showNumberDec(0, true, 1, 2);
  delay(TEST_DELAY);
  display.showNumberDec(0, true, 1, 1);
  delay(TEST_DELAY);
  display.showNumberDec(0, true, 1, 0);
  delay(TEST_DELAY);

  // Brightness Test
  for(k = 0; k < 4; k++)
  data[k] = 0xff;
  for(k = 0; k < 7; k++) {
    display.setBrightness(k);
    display.setSegments(data);
    delay(TEST_DELAY);
  }

  // On/Off test
  for(k = 0; k < 4; k++) {
    display.setBrightness(7, false);  // Turn off
    display.setSegments(data);
    delay(TEST_DELAY);
    display.setBrightness(7, true); // Turn on
    display.setSegments(data);
    delay(TEST_DELAY);
  }

  // Done!
  display.setSegments(SEG_DONE);

  while(1);
}

/*
void loop() {
  // incoming commands
  byte bite;
  while (Serial1.available() > 0) {
    bite = Serial1.read();

    // handle command modes
    if (bite == BRIGHTNESS) {
      mode = BRIGHTNESS;
      byteNum = 0;
      Serial.print("BRIGHTNESS: ");
    } else if (bite == RESET) {
      Serial.print("RESET");
      reset();
    } else if (bite == NOTE_ON) {
      mode = NOTE_ON;
      byteNum = 0;
    } else {
      if (mode == BRIGHTNESS) {

        FastLED.setBrightness(bite);
        FastLED.show();
        Serial.println(bite);

      } else if (mode == NOTE_ON) {
        if (byteNum == 0) {
          channel = bite;
        } else if (byteNum == 1) {
          beat = bite;
        } else if (byteNum == 2) {
          noteNum = bite;
        } else if (byteNum == 3) {
          velocity = bite;
          noteOn(channel, beat, noteNum, velocity);
        }
      }
      byteNum++;
    }
  }

  // animation loop
  if (fadeMetro.check() == 1) {
    step();
    fadeAllPixels();
    FastLED.show();
  }
}
*/

/* === Animation == */

/*
enum ANIMATION_TYPE {
  HUE,
  HUE_SHIFT,
  PURPLE
};
const int ANIMATION_TYPE_COUNT = 3;
ANIMATION_TYPE animationType = HUE;

int fadeAmount = 2;
int hueShift = 0;

void reset() {
  // next
  switch (animationType) {
    case HUE:
      animationType = HUE_SHIFT;
      break;
    case HUE_SHIFT:
      animationType = PURPLE;
      break;
    case PURPLE:
      animationType = HUE;
      break;
    default:
      break;
  }

  // setup
  switch (animationType) {
    case HUE:
      // Note number used to select HUE of note
      break;

    case HUE_SHIFT:
      // Note number used to select HUE of note
      // hue wheel rotates CCW
      hueShift = 0;
      break;

    case PURPLE:
      // Purple heat fades to blue
      break;

    default:
      break;
  }
}

void step() {
  switch (animationType) {
    case HUE:
      break;

    case HUE_SHIFT:
      hueShift += 1;
      break;

    case PURPLE:
      // Purple heat fades to blue
      break;
    default:
      break;
  }

}

void noteOn(int channel, int beat, int noteNum, int velocity) {
  // Serial.print("NOTE ON: ");
  // Serial.print(channel);
  // Serial.print(" :: ");
  // Serial.print(beat);
  // Serial.print(" :: ");
  // Serial.print(noteNum);
  // Serial.print(" :: ");
  // Serial.println(velocity);

  int hue;
  switch (animationType) {
    case HUE:
      // Note number used to select HUE of note
      // translate note number to hue
      hue = map(noteNum, 62, 76, 0, 255);
      setPixel(channel, beat, CHSV(hue, 255, 255));

      break;

    case HUE_SHIFT:
      // Note number used to select HUE of note
      // hue wheel rotates CCW
      hue = map(noteNum, 62, 76, 0, 255);
      hue += hueShift;
      Serial.println(hue);
      setPixel(channel, beat, CHSV(hue, 255, 255));

      break;

    case PURPLE:
      // Purple heat fades to blue
      setPixel(channel, beat, CRGB::Magenta);
      break;

    default:
      break;
  }

  FastLED.show();
}
*/

/* === LEDs === */

/*
int channel0[] = { 22, 8, 7, 6, 5, 4, 3, 2, 1, 0, 12, 23 };
int channel1[] = { 9, 21, 20, 19, 18, 17, 16, 15, 14, 13, 11, 10 };

void setPixel(int channel, int index, CHSV color) {
  int idx = channel == 0 ? channel0[index] : channel1[index];
  leds[idx] = color;
}

void setPixel(int channel, int index, CRGB color) {
  int idx = channel == 0 ? channel0[index] : channel1[index];
  leds[idx] = color;
}

void fadeAllPixels() {
  switch (animationType) {
    case HUE:
    case HUE_SHIFT:
      // simple fade out
      for(int i = 0; i < NUM_LEDS; i++) { leds[i].fadeToBlackBy(fadeAmount); }
      break;

    case PURPLE:
      // blend toward another color
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = nblend(leds[i], CRGB::LightSeaGreen, 1.0);
      }

      break;

    default:
      break;
  }
}






void test() {
	static uint8_t hue = 0;
	// First slide the led in one direction
	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to red
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeAllPixels();
		// Wait a little bit before we loop around and do it again
		delay(40);
	}

	// Now go in the other direction.
	for(int i = (NUM_LEDS)-1; i >= 0; i--) {
		// Set the i'th led to red
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeAllPixels();
		// Wait a little bit before we loop around and do it again
		delay(40);
	}
}
*/
