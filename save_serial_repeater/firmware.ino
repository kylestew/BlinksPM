#include <FastLED.h>

// == LEDS ==
#define NUM_LEDS    288
#define DATA_PIN    2
CRGB leds [NUM_LEDS];
int brightness = 0;
// TODO: use system clock for actual FPS locking
const int FPS_DELAY = 15; // 1/60 = 16.66666 (give time to process between frames)

// == BRIGHT POT ==
#define BRIGHTNESS_POT	A0
int briPollTick = 0;
const int POLL_EVERY = 4;


void setup() {
	delay(200);

  Serial.begin(115200);

  // LEDs
  pinMode(DATA_PIN, OUTPUT);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
	FastLED.setBrightness(brightness);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setTemperature(FullSpectrumFluorescent);
  fill_solid(&(leds[0]), NUM_LEDS, CRGB(0, 0, 0));
  FastLED.clear();

	// Brightness
	pinMode(BRIGHTNESS_POT, INPUT);
}

void loop() {
	if (briPollTick++ > POLL_EVERY) {
		// divide to keep noise from flickering value
		// should give 64 brightness levels
	 	float bri = analogRead(BRIGHTNESS_POT) / 16.0;
	 	brightness = map(bri, 0, 64, 20, 254);
	 	briPollTick = 0;
		FastLED.setBrightness(brightness);
	}

	// LED serial data overrides animation playback
	// if (Serial.available()) {
	// 	Serial.write(Serial.read());
	// }
	if (Serial.available() > 0) {
		// read in all LED data at once
		Serial.readBytes( (char*)leds, NUM_LEDS * 3);
	}
	 // FastLED.show();

	FastLED.delay(FPS_DELAY);
}
