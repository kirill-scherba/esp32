/*
  Touch buttons example.

  This example shows how to use the touch inputs of ESP32
  board to control 3 LEDs.

  The circuit:
  - 3 LEDs connected to pins 18, 19, 33 and 32
  - 3 touch buttons connected to pins 15, 2 and 4

  created 11 Nov 2024
  modified 14 Nov 2024
  by Kirill Scherba
*/

#include <teoTouch.h>

// Define the LED pin
const int ledPin = 2;        // Boaar Led
const int ledPinGreen1 = 18; // Green Led 1
const int ledPinRed1 = 19;   // Red Led 1
const int ledPinGreen2 = 33; // Green Led 2
const int ledPinRed2 = 32;   // Red Led 2

// Define touch buttons pins
const int buttonPin1 = 15; // Button 1
const int buttonPin2 = 4;  // Button 2
const int buttonPin3 = 14; // Button 3

// Define the touch threshold
const int threshold = 20;

// Define the touch repeat interval
const int repeatInterval = 200;

// Touch button struct
struct Button {
  int pin;
  int ledPin;
  bool state = false;
  unsigned long lastTouchMillis = 0;
};

// Array of touch buttons
Button buttons[] = {{buttonPin1, ledPinRed1},
                    {buttonPin2, ledPinGreen1},
                    {buttonPin3, ledPinGreen2}};

void setup() {

  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the LED pins as an output
  pinMode(ledPin, OUTPUT);
  pinMode(ledPinGreen1, OUTPUT);
  pinMode(ledPinRed1, OUTPUT);
  pinMode(ledPinGreen2, OUTPUT);
  pinMode(ledPinRed2, OUTPUT);

  // Create the touch pins callbacks
  new TeoTouchClass(buttons[0].pin, switchLed);
  new TeoTouchClass(buttons[1].pin, switchLed);
  new TeoTouchClass(buttons[2].pin, switchLed);

  // Serial.printf("setup, num buttons: %d\n", touch->numTouchActions());
}

void switchLed(int i, bool state) {
  Serial.printf("switchLed %d, state: %d\n", i, state);
  buttons[i].state = state;
  buttons[i].lastTouchMillis = millis();
  if (state) {
    digitalWrite(buttons[i].ledPin, HIGH);
  } else {
    digitalWrite(buttons[i].ledPin, LOW);
  }
}

bool lastBlink = true;

void loop() {

  // Get last touch time to check if we need to blink after 5 seconds of
  // inactivity
  unsigned long lastTouchMillis = 0;
  for (int i = 0; i < 3; i++) {
    lastTouchMillis = max(lastTouchMillis, buttons[i].lastTouchMillis);
  }
  bool blink = millis() - lastTouchMillis > 5000;

  // Turn the LEDs on
  digitalWrite(ledPin, HIGH);
  if (lastBlink) {
    for (int i = 0; i < 3; i++) {
      if (buttons[i].state) {
        digitalWrite(buttons[i].ledPin, HIGH);
      }
    }
  }

  // Delay for 50 ms and skip if not blinking
  if (!blink) {
    lastBlink = false;
    delay(50);
    return;
  }
  lastBlink = true;

  // Delay for 1 second
  delay(1000);

  // Turn the LED off
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPinGreen1, LOW);
  digitalWrite(ledPinRed1, LOW);
  digitalWrite(ledPinGreen2, LOW);
  digitalWrite(ledPinRed2, LOW);

  // Delay for 1 second
  delay(1000);
}
