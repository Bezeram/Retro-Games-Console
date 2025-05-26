#pragma once
#include <Arduino.h>
#include <LedControl.h>

class Engine {
private:

	// Input pins:
	const int buttonPin = 2;
	const int buttonJoystickPin = 4;
	const int xPin = 0;
	const int yPin = 1;

	// Audio pins:
	const int buzzerPin = 12;
	
	// Display pins:
	const int dinPinRight = 9;
	const int csPinRight = 8;
	const int clkPinRight = 10;

	const int dinPinLeft = 7;
	const int csPinLeft = 5;
	const int clkPinLeft = 6;

	LedControl ledControllerLeft = LedControl(dinPinLeft,clkPinLeft,csPinLeft,1); // (DIN, CLK, CS, num displays
	LedControl ledControllerRight = LedControl(dinPinRight,clkPinRight,csPinRight,1); // (DIN, CLK, CS, num displays

	unsigned char rowsDisplayLeft[8];
	unsigned char rowsDisplayRight[8];
	int buttonState;
	int buttonJoystickState;

	float remap(float, float, float, float, float);

public:
	float deltaTime;
	unsigned long time;

	// Player input info:
	float inputX;
	float inputY;
	bool buttonDown;
	bool buttonUpThisFrame;
	bool buttonDownThisFrame;
	float buttonDownDuration;

	// Player input info:
	bool buttonJoystickDown;
	bool buttonJoystickUpThisFrame;
	bool buttonJoystickDownThisFrame;
	float buttonJoystickDownDuration;


	Engine();

	void playSound(int frequency, int duration);

	void updateLoop(float);

	void clearScreen();

	void setPixel(int x, int y);

	void setPixelToValue(int x, int y, bool on);

	void drawToDisplay();

	void setDisplayBrightness(int);
	
};