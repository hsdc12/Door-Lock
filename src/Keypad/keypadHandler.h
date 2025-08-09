#ifndef KEYPAD_HANDLER_H
#define KEYPAD_HANDLER_H

char getKeyPressed(); // call this in loop() to get the current key press (or '\0' if none)

void initKeypad();    // call in setup()

void keypadloop();

void checkCode();

void enterAdminMode(); // Allows setting a new passcode
    void adminPassword(); // Allows setting a new passcode
    void adminTime();
    void adminDate();
    void adminReset();
        void adminResetInit(); // Resets the device to factory settings
    void adminCard();
        void CardMenu(); // Allows setting a new card
            void IDCard1(); // Allows setting a new card
            void IDCard2(); // Allows setting a new card
            void IDCard3(); // Allows setting a new card
            void IDCard4(); // Allows setting a new card
            void IDReset(); // Allows resetting all cards
    void adminExit();
    void adminMenu();

#endif