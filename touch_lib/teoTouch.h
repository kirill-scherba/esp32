#ifndef TEO_TOUCH_H
#define TEO_TOUCH_H

#include <Arduino.h>

typedef void (*ActionType)(int, bool) ;

typedef struct {
  int pin;
  bool state;
  unsigned long lastTouchMillis;
  ActionType action;
} TouchAction;

class TeoTouchClass {
public:
  TeoTouchClass() {}
  TeoTouchClass(int pin, ActionType action);

  int numTouchActions() { return touchActionsLength; }

private:
  static void handleTouch();

  // Define the static touch actions array
  static int touchActionsLength;
  static TouchAction *touchActions;

  // addTouchAction adds action to TouchAction
  static void addTouchAction(int pin, ActionType action) {
    if (touchActions == nullptr) {
      touchActions = (TouchAction *)malloc(sizeof(TouchAction));
    } else {
      touchActions = (TouchAction *)realloc(
          touchActions, sizeof(TouchAction) * (touchActionsLength + 1));
    }

    touchActions[touchActionsLength].pin = pin;
    touchActions[touchActionsLength].state = false;
    touchActions[touchActionsLength].lastTouchMillis = 0;
    touchActions[touchActionsLength].action = action;
    touchActionsLength++;
  }

  // removeTouchAction removes action from TouchAction
  static void removeTouchAction(int pin) {
    for (int i = 0; i < touchActionsLength; i++) {
      if (touchActions[i].pin == pin) {
        for (int j = i; j < touchActionsLength - 1; j++) {
          touchActions[j] = touchActions[j + 1];
        }
        touchActionsLength--;
        break;
      }
    }
  }

  // getTouchAction returns action from TouchAction by pin
  static TouchAction *getTouchAction(int pin) {
    for (int i = 0; i < touchActionsLength; i++) {
      if (touchActions[i].pin == pin) {
        return &touchActions[i];
      }
    }
    return nullptr;
  }
};

#endif