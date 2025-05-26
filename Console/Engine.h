#pragma once
#include <Arduino.h>
#include <LedControl.h>


class Engine {
private:

	// Input pins:
	const int buttonPin = 2;
	const int xPin = 0;
	const int yPin = 1;

	// Audio pins:
	const int buzzerPin = 12;
	
	// Display pins:
	const int dinPinA = 9;
	const int csPinA = 8;
	const int clkPinA = 10;

	const int dinPinB = 7;
	const int csPinB = 5;
	const int clkPinB = 6;

	LedControl ledControllerA = LedControl(dinPinA,clkPinA,csPinA,1); // (DIN, CLK, CS, num displays
	LedControl ledControllerB = LedControl(dinPinB,clkPinB,csPinB,1); // (DIN, CLK, CS, num displays

	unsigned char rowsDisplayA[8];
	unsigned char rowsDisplayB[8];
	int buttonState;

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


	Engine();

	void playSound(int frequency, int duration);

	void updateLoop(float);

	void clearScreen();

	void setPixel(int x, int y);

	void setPixelToValue(int x, int y, bool on);

	void drawToDisplay();

	void setDisplayBrightness(int);
	
};