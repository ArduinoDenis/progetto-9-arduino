/**
 * Burglar alarm system built with Arduino Uno. Materials:
 * Arduino UNO, RFID RC522 reader, PIR sensor, 3 colored LEDs,
 * a button, a buzzer.
 * Important: Connect the RFID reader to Arduino pins as follows:
 * MOSI: Pin 11 / ICSP-4
 * MISO: Pin 12 / ICSP-1
 * SCK: Pin 13 / ISCP-3
 * SDA: Pin 10
 * RST: Pin 9
 */

#include <SPI.h>
#include <RFID.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9

#define masnum0 70 
#define masnum1 43
#define masnum2 191
#define masnum3 180
#define masnum4 102

#define pir 8
#define ledGreen 4
#define ledRed 3
#define ledgial 5
#define resetkey 6
#define pausa 3000 // Constant defining entry and exit time, here set to 3 seconds

RFID rfid(SS_PIN, RST_PIN); 
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

boolean antiON = false; // Burglar alarm activated
boolean allarm = false; // Central in alarm
boolean cardmas = false; // Master key variable
int ledState = 0;
int slave; // Saved key counter

unsigned long previousMillis = 0;       
unsigned long interval = 500;
int sernum0;
int sernum1;
int sernum2;
int sernum3;
int sernum4;

void setup()
{ 
  lcd.begin(16, 2);
  SPI.begin(); 
  rfid.init();
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(ledgial, OUTPUT);
  pinMode(resetkey, INPUT);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledgial, LOW);
  lcd.print("ArduinoDenis");
}

void loop() {

  slave = EEPROM.read(0);

  // Detect a tag...
  if (rfid.isCard()) {
      // Read the serial...
      if (rfid.readCardSerial()) {
        sernum0 = rfid.serNum[0];
        sernum1 = rfid.serNum[1];
        sernum2 = rfid.serNum[2];
        sernum3 = rfid.serNum[3];
        sernum4 = rfid.serNum[4];
        
        // If the read serial matches the Master serial,
        // activate or deactivate the Key Storage mode
        // and also display the list of saved keys...
        if (sernum0 == masnum0
            && sernum1 == masnum1
            && sernum2 == masnum2
            && sernum3 == masnum3
            && sernum4 == masnum4) {
            if (!cardmas) {
              lcd.clear();
              lcd.print("CARD MASTER");
              delay(1500);
              lcd.clear();
              lcd.print("KEY MANAGEMENT");
              cardmas = true;
              lcd.setCursor(0, 1);
              lcd.print("Slave keys: ");
              lcd.setCursor(15, 1);
              lcd.print(slave);
              delay(2000);
            }  else {
              cardmas = false;
              lcd.setCursor(0, 1);
              lcd.print("Cancelled");
              delay(3000);
              standby();
            }
        } 
        // If the read serial matches one of the three groups
        // of stored keys then activate or deactivate the alarm.
        else if ((sernum0 == EEPROM.read(1) && sernum1 == EEPROM.read(2) && sernum2 == EEPROM.read(3) && sernum3 == EEPROM.read(4) && sernum4 == EEPROM.read(5))
                 || (sernum0 == EEPROM.read(6) && sernum1 == EEPROM.read(7) && sernum2 == EEPROM.read(8) && sernum3 == EEPROM.read(9) && sernum4 == EEPROM.read(10))
                 || (sernum0 == EEPROM.read(11) && sernum1 == EEPROM.read(12) && sernum2 == EEPROM.read(13) && sernum3 == EEPROM.read(14) && sernum4 == EEPROM.read(15))) {
            if (!antiON) {  
              antiON = true;
              lcd.clear();
              lcd.print("VALID KEY");
              delay(1500);
              lcd.clear();
              lcd.print("BURGLAR ALARM");
              lcd.setCursor(0, 1);
              lcd.print("ARMED...");
              delay(pausa);
              digitalWrite(ledRed, HIGH);
              lcd.clear();
              lcd.print("BURGLAR ALARM");
              lcd.setCursor(0, 1);
              lcd.print("ACTIVE");
            }  else {
              antiON = false;
              allarm = false;
              digitalWrite(ledRed, LOW);
              digitalWrite(ledGreen, LOW);
              lcd.clear();
              lcd.print("VALID KEY");
              delay(1500);
              lcd.clear();
              lcd.print("BURGLAR ALARM");
              lcd.setCursor(0, 1);
              lcd.print("DEACTIVATED");
              delay(3000);
              standby();
            }
        } 
        // If the read serial is different from the master and not stored in memory,
        // and if the Key Storage mode is active, save the serial to memory
        // as slave1, slave2 or slave3
