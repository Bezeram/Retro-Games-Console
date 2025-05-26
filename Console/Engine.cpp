#include "Engine.h"
#include "Arduino.h"
#include <LedControl.h>
#include "src/Game.h"

Engine :: Engine() {
	ledControllerLeft.shutdown(0, false);
	ledControllerRight.shutdown(0, false);

	pinMode(buttonPin, INPUT);
  	pinMode(buzzerPin, OUTPUT);

  	clearScreen();
  	time = 0;
}

void Engine :: updateLoop(float deltaTime) {
	this->deltaTime = deltaTime;
	time += deltaTime;

	// Button input:
	int buttonStateOld = buttonState;
	buttonState = digitalRead(buttonPin);
	buttonDown = buttonState == 1;
  	buttonDownThisFrame = buttonDown && buttonState != buttonStateOld;
  	buttonUpThisFrame = buttonState == 0 && buttonStateOld == 1;

  	if (buttonDownThisFrame) {
  		buttonDownDuration = 0;
  	}
  	if (buttonDown) {
  		buttonDownDuration += deltaTime;
  	}

	// Get analog stick input:
	const float inputThreshold = 0.1;
	inputX = remap(analogRead(xPin), 0, 1023, -1, 1);
	inputY = remap(analogRead(yPin), 0, 1023, -1, 1);

	if (abs(inputX) < inputThreshold) {
		inputX = 0;
	}
	if (abs(inputY) < inputThreshold) {
		inputY = 0;
	}
}

void Engine :: playSound(int frequency, int duration) {
	
	tone(buzzerPin, frequency, duration);
}

void Engine :: clearScreen() {
 	for (int i = 0; i < 8; i ++) {
      rowsDisplayRight[i] = 0;
      rowsDisplayLeft[i] = 0;
 	}
}

void Engine :: drawToDisplay() {
	for (int row = 0; row < 8; row ++) {
		ledControllerRight.setRow(0, row, rowsDisplayRight[row]);
		ledControllerLeft.setRow(0, row, rowsDisplayLeft[row]);
 	}
}


// set display brightness [0,15]
void Engine :: setDisplayBrightness(int brightness) {
	ledControllerRight.setIntensity(0, brightness);
  ledControllerLeft.setIntensity(0, brightness);
}

void Engine :: setPixel(int x, int y) {
	setPixelToValue(x, y, true);
}

void Engine :: setPixelToValue(int x, int y, bool on) {
	if (x >= 16 || x < 0 || y >= 8 || y < 0) {
		return;
	}

	if (x >= 8) {
		// x apartine [8, 15]
		// y apartine [0, 7]
		// x -> y
		// y -> x
		if (on) {
			rowsDisplayRight[x-8] |= 1 << y;
		}
		else {
			rowsDisplayRight[x-8] &= ~(1 << y);
		}
	}
	else {
		// x apartine [0, 7]
		// y apartine [0, 7]
		// x -> -y
		// y -> -x
		if (on) {
			rowsDisplayLeft[7 - x] |= 1 << (7 - y);
		}
		else {
			rowsDisplayLeft[7 - x] &= ~(1 << (7 - y));
		}
	}
}

float Engine :: remap(float value, float minOld, float maxOld, float minNew, float maxNew) {
  return minNew + (value - minOld)/(maxOld-minOld) * (maxNew-minNew);
}
