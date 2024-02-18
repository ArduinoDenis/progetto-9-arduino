# Arduino Burglar Alarm System

This Arduino Uno sketch implements a burglar alarm system using an RFID RC522 reader, a PIR sensor, LEDs, a button, and a buzzer.

## Installation:

1. Connect the RFID RC522 reader to the Arduino Uno according to the specified pin layout.
2. Upload the sketch to your Arduino Uno board.

## Usage:

The system reads RFID cards and activates or deactivates the alarm based on the detected cards. Master cards can be used to manage stored keys. In addition, the system includes a PIR sensor to detect motion and activate the alarm.

## Dependencies:

This sketch relies on the RFID, SPI, EEPROM, and LiquidCrystal libraries.

## Files:

- `antifurto_rfid.ino`: The main Arduino sketch for the burglar alarm system.
- `leggere-carte-magnetiche.ino`: This file is a program used to read the unique code of your cards (1 white, 1 blue), hence the project name "antifurto_rfid.ino".

## Libraries:

- `MFRC522-1.4.11.zip`: This library is required for the RFID functionality in the project. It should be placed in the following directory: `Documents\Arduino\libraries\paste here`.
