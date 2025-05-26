#include <LedControl.h>

// Display pins: DIN (data in), CLK (clock), CS (chip select)
const int numDisplays = 2;
LedControl ledController = LedControl(10, 8, 9, numDisplays);

// PWM out pin
int buzzerPin = 3;

// module button in pin
int buttonPin = 2;

int xPin = A0;     // X-axis connected to analog pin A0
int yPin = A1;     // Y-axis connected to analog pin A1
int joystickButtonPin = 4; // Optional button connected to digital pin 2

void setup() {
  // put your setup code here, to run c:\Users\basar\Desktop\arduino_display_test\LedControl\LedControl.honce:
  ledController.shutdown(0, false);
  ledController.shutdown(1, false);
  ledController.setIntensity(0, 1);
  ledController.setIntensity(1, 1);

  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT); // Set pin as input
  pinMode(joystickButtonPin, INPUT_PULLUP); // Button uses internal pull-up resistor

  Serial.begin(9600);
}

void toneWithVolume(int pin, int freq, int duration, int volume) {
  if (volume < 0)
    volume = 0;
  if (volume > 100)
    volume = 100;

  long period = 1000000L / freq;               // Period in microseconds
  long highTime = period * volume / 100 / 2;   // ON time
  long lowTime = period * (100 - volume) / 100 / 2; // OFF time

  long cycles = (long)freq * duration / 1000;

  for (long i = 0; i < cycles; i++) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(highTime);
    digitalWrite(pin, LOW);
    delayMicroseconds(lowTime);
  }
}

void test_buzzer()
{
  // Play 440 Hz at 20% volume
  toneWithVolume(buzzerPin, 392, 500, 1);
  delay(1000);
}

void test_led_displays()
{
  for (int i = 0; i < 2; i++)
  {
    for (int y = 0; y < 8; y++)
      for (int x = 0; x < 8; x++)
      {
        bool ledState = (x + y) % 2 == i;
        ledController.setLed(0, x, y, ledState);
        ledController.setLed(1, x, y, !ledState);
      }

    delay(500);
  }
}

int buttonState = HIGH;

void test_module_button()
{
  int previousButtonState = buttonState;
  buttonState = digitalRead(buttonPin);
  int buttonPressed = previousButtonState == HIGH && buttonState == LOW;

  // TODO: stop it from printing so many times. use interruptions?
  if (buttonPressed)
  {
    Serial.println("Button is pressed!");
  }
}

void loop() {
  float xValue = analogRead(xPin) / 1023.0 * 2.0 - 1.0;  // Reads 0-1023 (left-right)
  float yValue = analogRead(yPin) / 1023.0 * 2.0 - 1.0;  // Reads 0-1023 (up-down)
  int buttonState = digitalRead(joystickButtonPin);

  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print("\tY: ");
  Serial.print(yValue);
  Serial.print("\tButton: ");
  Serial.println(buttonState == LOW ? "Pressed" : "Released");
}
