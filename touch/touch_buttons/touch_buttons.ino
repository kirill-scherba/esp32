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

  // Set the touch interrupt pins
  touchAttachInterrupt(buttons[0].pin, touchButtonPin1, threshold);
  touchAttachInterrupt(buttons[1].pin, touchButtonPin2, threshold);
  touchAttachInterrupt(buttons[2].pin, touchButtonPin3, threshold);
}

/**
 * @brief Handler for touch button presses
 *
 * This function is called when a touch button press is detected. It will
 * toggle the state of the button and turn the associated LED on or off.
 *
 * @param i The index of the button in the buttons array
 */
void touchButtonPin(int i) {

  auto val = touchRead(buttons[i].pin);
  // Serial.printf("  ---- Touch %d was Pressed = %d \n", i, val);

  // Ignore the touch if the value is above the threshold
  if (val > threshold) {
    return;
  }

  // Get the current time in milliseconds
  auto ms = millis();
  // Calculate the difference between the current time and the last touch time
  auto diff = ms - buttons[i].lastTouchMillis;
  // Update the last touch time
  buttons[i].lastTouchMillis = ms;

  // Ignore the touch if it was less than repeatInterval since the last touch
  if (diff < repeatInterval) {
    return;
  }

  // Switch active state and toggle LED
  buttons[i].state = !buttons[i].state;
  if (buttons[i].state) {
    digitalWrite(buttons[i].ledPin, HIGH);
  } else {
    digitalWrite(buttons[i].ledPin, LOW);
  }
}

void touchButtonPin1() { touchButtonPin(0); }
void touchButtonPin2() { touchButtonPin(1); }
void touchButtonPin3() { touchButtonPin(2); }

void loop() {

  // Turn the LED on
  for (int i = 0; i < 3; i++) {
    if (buttons[i].state) {
      digitalWrite(buttons[i].ledPin, HIGH);
    }
  }
  digitalWrite(ledPin, HIGH);

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

