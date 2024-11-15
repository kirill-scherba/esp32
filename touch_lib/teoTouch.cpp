/**
 * @file teoTouch.cpp
 * @brief Library for using the touch input on the ESP32.
 *
 * Copyright (c) 2024, Kirill Scherba
 * 
 * @license teoWiFi is free software: you can redistribute it and/or modify it
 * under the terms of the BSD 3-clause license as published by the
 * University of California, Berkeley.
 *
 * teoWiFi is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the BSD 3-clause license for more details.
 *
 * You should have received a copy of the BSD 3-clause license along
 * with this program.  If not, see <http://www.opensource.org/licenses/bsd-license.php>.
 * 
 * @details
 * This library was written to make it easy to use the touch input of the ESP32
 * board. It provides a simple way to attach a touch interrupt to a pin and
 * call a function when the touch interrupt is triggered.
 *
 */

#include "teoTouch.h"
#include <Arduino.h>

// Define the touch threshold
const int threshold = 20;

// Define the touch repeat interval
const int repeatInterval = 200;

// Define the static member variable
int TeoTouchClass::touchActionsLength = 0;
TouchAction *TeoTouchClass::touchActions = nullptr;

/**
 * @brief TeoTouchClass constructor.
 *
 * @param pin The pin to attach the touch interrupt to.
 * @param action The action to call when the touch interrupt is triggered.
 *
 * This constructor adds the touch action to the static TouchAction array
 * and attaches a touch interrupt to the given pin.
 */
TeoTouchClass::TeoTouchClass(int pin, ActionType action) {
  // Add action to TouchAction
  addTouchAction(pin, action);

  // Attach interrupt.
  // The threshold is the minimum value that must be read from the pin
  // before the interrupt is triggered.
  touchAttachInterrupt(pin, handleTouch, threshold);
}

/**
 * @brief Handles the touch interrupt by going through all the TouchActions
 * and calling their action if the touch is valid.
 *
 * The touch is valid if the value read from the pin is less than the
 * threshold, and if the difference between the current time and the last
 * touch time is greater than repeatInterval.
 */
void TeoTouchClass::handleTouch() {
  for (int i = 0; i < touchActionsLength; i++) {
    auto val = touchRead(touchActions[i].pin);
    if (val > threshold) {
      // Ignore the touch if the value is above the threshold
      continue;
    }

    auto action = &touchActions[i];

    // Get the current time in milliseconds, and calculate the difference
    auto ms = millis();
    auto diff = ms - action->lastTouchMillis;
    action->lastTouchMillis = ms;

    // Ignore the touch if it was less than repeatInterval since the last
    // touch
    if (diff < repeatInterval) {
      // Ignore the touch if it was less than repeatInterval since the last
      // touch
      continue;
    }

    // Switch active state and call action
    action->state = !action->state;
    action->action(i, action->state);
  }
}
