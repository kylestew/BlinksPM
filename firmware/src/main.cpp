#include <Arduino.h>

#include "animation.h"

// == LEDS ==
DMAMEM int displayMemory[LED_COUNT * 6];
int drawingMemory[LED_COUNT * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(LED_COUNT, displayMemory, drawingMemory, config);

// == ANIMATION ==
Animation* animation = nullptr;
void setupAnimations();
void animationTick();

// == BRIGHT POT ==
#define BRIGHTNESS_POT	A1
int brightness = 12;
int briPollTick = 0;
const int POLL_EVERY = 12;

// == STATUS PIN ==
#define STATUS_PIN 		13


void setup() {
	delay(200);

  Serial.begin(115200);

	// non-deterministic random
	randomSeed(analogRead(2));

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
	 	briPollTick = 0;

	 	brightness = analogRead(BRIGHTNESS_POT);
		brightness *= brightness; // double for non-linear response
		brightness /= 4096*8; // limit amount of value changes
		animation->setBrightness(map((float)brightness, 0.0, 31.0, 0.2, 1.0));
	}

	animationTick();
}

// == ANIMATIONS ==

// #include "sandbox.h"
#include "rainbow.h"
#include "plasma.h"
#include "noise.h"
#include "fire.h"

void setupAnimations() {
	// animation = new Sandbox(&leds);
	// animation = new Rainbow(&leds);
	animation = new Plasma(&leds);
	// animation = new Noise(&leds);
	// animation = new Fire(&leds);
}

void animationTick() {
	digitalWrite(STATUS_PIN, HIGH);
	animation->step();
  digitalWrite(1, LOW);
	leds.show();
}
