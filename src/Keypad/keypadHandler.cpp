#include <Arduino.h>
#include <Keypad.h>
#include "keypadHandler.h"

// Define the keymap (4x3 for standard keypad)
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Use analog pins A2, 3, 7, A0 for rows
byte rowPins[ROWS] = {A2, 3, 7, A0};
// Use analog pins A1, A3 and digital pin 8 for columns
byte colPins[COLS] = {A1, A3, 8};

// Create the Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void initKeypad() {
  // No initialization needed for the library,
  // but we keep this here in case we add custom behavior
}

char getKeyPressed() {
  return keypad.getKey();
}