#include <Arduino.h>

#import "animation.h"

// == LEDS ==
DMAMEM int displayMemory[NUM_LEDS * 6];
int drawingMemory[NUM_LEDS * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(NUM_LEDS, displayMemory, drawingMemory, config);
int brightness = 80;

// == BRIGHT POT ==
#define BRIGHTNESS_POT	A1
int briPollTick = 0;
const int POLL_EVERY = 8;

// == STATUS PIN ==
#define STATUS_PIN 		13

// == ANIMATION ==
unsigned long lastFrameEndTime;
const unsigned long FRAME_TIME = 8; // ms - 125 FPS
void setupAnimations();
void animationTick();


void setup() {
	delay(200);

  Serial.begin(115200);

  // LEDs
	leds.begin();
	leds.show();

	// Brightness
	pinMode(BRIGHTNESS_POT, INPUT);

	// Status
	pinMode(STATUS_PIN, OUTPUT);

	setupAnimations();
}

void loop() {
	if (briPollTick++ > POLL_EVERY) {
		// divide to keep noise from flickering value
		// should give 64 brightness levels
	 	float bri = analogRead(BRIGHTNESS_POT) / 16.0;
	 	brightness = map(bri, 0, 64, 20, 254);
	 	briPollTick = 0;
	// 	FastLED.setBrightness(brightness);
	}

	animationTick();

	// burn rest of frame time (total time to burn - what has been used)
	unsigned long timeBurnt = (millis() - lastFrameEndTime);
	if (timeBurnt < FRAME_TIME) {
		delay(FRAME_TIME - timeBurnt);
	}
	lastFrameEndTime = millis();
}

// == ANIMATIONS ==

// #include "rainbow.h"
// #include "noise.h"
// #include "fire.h"
#include "sandbox.h"

Animation* animation = nullptr;

void setupAnimations() {
	// animation = new Rainbow(leds);
	// animation = new Noise(leds);
	// animation = new Fire(leds);
	animation = new Sandbox(&leds);
}

void animationTick() {
	digitalWrite(STATUS_PIN, HIGH);
	animation->step();
	leds.show();
  digitalWrite(1, LOW);
}
