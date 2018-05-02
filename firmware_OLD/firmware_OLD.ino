#import "animation_base.h"

// == LEDS ==
#define DATA_PIN    		2
CRGB leds[NUM_LEDS];
int brightness = 0;
const unsigned long FRAME_TIME = 15; // ms - ~66.6 FPS
unsigned long lastFrameEndTime;

// == BRIGHT POT ==
#define BRIGHTNESS_POT	A0
int briPollTick = 0;
const int POLL_EVERY = 8;

void setupAnimations();
void animationTick();

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

	setupAnimations();
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

	animationTick();

	// burn rest of frame time (total time to burn - what has been used)
	unsigned long burnTime = (millis() - lastFrameEndTime);
	if (burnTime < FRAME_TIME) {
		// FastLED.delay(FRAME_TIME - burnTime);
		delay(FRAME_TIME - burnTime);
	}
	lastFrameEndTime = millis();
}

// == ANIMATIONS ==

#include "rainbow.h"
#include "noise.h"
#include "fire.h"

Animation* animation = nullptr;

void setupAnimations() {
	// animation = new Rainbow(leds);
	// animation = new Noise(leds);
	animation = new Fire(leds);
}

void animationTick() {
	animation->step();
	LEDS.show();

	// TODO: crossfade animation changes please
}
