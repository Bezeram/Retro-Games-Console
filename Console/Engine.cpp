#include "Engine.h"
#include "Arduino.h"
#include <LedControl.h>
#include "src/Game.h"

Engine :: Engine() {
	ledControllerLeft.shutdown(0, false);
	ledControllerRight.shutdown(0, false);

	pinMode(buttonPin, INPUT);
	pinMode(buttonJoystickPin, INPUT_PULLUP);
	pinMode(buzzerPin, OUTPUT);

	clearScreen();
	time = 0;
}

void Engine :: updateLoop(float deltaTime) {
	this->deltaTime = deltaTime;
	time += deltaTime;

	// Module Button input:
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

	// Joystick button input:
	int buttonJoystickStateOld = buttonJoystickState;
	buttonJoystickState = digitalRead(buttonJoystickPin);
	buttonJoystickDown = buttonJoystickState == 0;
	buttonJoystickDownThisFrame = buttonJoystickDown == 0 && buttonJoystickStateOld == 1;
	buttonJoystickUpThisFrame = buttonJoystickState == 1 && buttonJoystickStateOld == 0;

	if (buttonJoystickDownThisFrame) {
		buttonJoystickDownDuration = 0;
	}
	if (buttonJoystickDown) {
		buttonJoystickDownDuration += deltaTime;
	}

	// Get analog stick input:
	const float inputThreshold = 0.1;
	inputX = remap(analogRead(xPin), 0, 1023, -1, 1);
	inputY = remap(analogRead(yPin), 0, 1023, -1, 1);

	// Convert into display coordinates:
	{
		int temp = inputX;
		inputX = inputY;
		inputY = temp;

		inputX *= -1;
		inputY *= -1;
	}

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

	// This code assumes that the left display is rotated 90 degrees clockwise
	// and the right display is rotated 90 degrees counter-clockwise.
	// As such, we must perform some coordinate transformations.
	//
	// The "virtual" system of axis we want to achieve is 
	// origin at the bottom left corner of the left display,
	// +x going right, +y going up.
	if (x >= 8) {
		// x in [8, 15]
		// y in [0, 7]
		if (on) {
			rowsDisplayRight[y] |= 1 << (x-8);
		}
		else {
			rowsDisplayRight[y] &= ~(1 << (x-8));
		}
	}
	else {
		// x in [0, 7]
		// y in [0, 7]
		if (on) {
			rowsDisplayLeft[7-y] |= 1 << (7-x);
		}
		else {
			rowsDisplayLeft[7-y] &= ~(1 << (7-x));
		}
	}
}

float Engine :: remap(float value, float minOld, float maxOld, float minNew, float maxNew) {
  return minNew + (value - minOld)/(maxOld-minOld) * (maxNew-minNew);
}
