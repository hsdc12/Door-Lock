#ifndef RFID_H
#define RFID_H

#include <MFRC522.h>
#include <rgb_lcd.h>

void initRFID();

void handleRFID();

void AuthorizedAccess();

void readStoredCard(int cardNumber, char* buffer);

#endif  // RFID_H