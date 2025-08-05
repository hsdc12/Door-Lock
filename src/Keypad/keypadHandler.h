#ifndef KEYPAD_HANDLER_H
#define KEYPAD_HANDLER_H

char getKeyPressed(); // call this in loop() to get the current key press (or '\0' if none)

void initKeypad();    // call in setup()

void keypadloop();

void checkCode();

void enterAdminMode(); // Allows setting a new passcode

#endif