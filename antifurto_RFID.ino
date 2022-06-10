/**

* Centrale antifurto costruita con Arduino uno. Materiali:

* Arduino UNO, lettore RFID RC522, sensore Pir, 3 led colorati,

* un pulsante, un buzzer.

* Importante, collegare il lettore RFID ai pin di Arduino come segue:

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

 

// Codice della chiave master.

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

#define pausa 3000 // Costante che definisce il tempo di ingresso e uscita, in questo caso 3 secondi

 

RFID rfid(SS_PIN, RST_PIN); 

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

 

boolean antiON = 0; // Variabile antifurto attivo

boolean allarm = 0; // Variabile centrale in allarme

boolean cardmas = 0; // Variabile chiave master

int ledState = 0;

int slave; // Contatore di chiavi salvate

 

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

  

  // Rileva un tag...

  if (rfid.isCard()) {

      // Legge il seriale...

      if (rfid.readCardSerial()) {

        sernum0 = rfid.serNum[0];

        sernum1 = rfid.serNum[1];

        sernum2 = rfid.serNum[2];

        sernum3 = rfid.serNum[3];

        sernum4 = rfid.serNum[4];

                // Se il seriale letto corrisponde con il seriale Master

                // attiva o disattiva la modalita Memorizzazione chiavi

                // e in più visualizza l'elenco della chiavi salvate... 

                if (sernum0 == masnum0

                && sernum1 == masnum1

                && sernum2 == masnum2

                && sernum3 == masnum3

                && sernum4 == masnum4) {

                if (cardmas==0) {

                  lcd.clear();

                  lcd.print("CARD MASTER");

                  delay(1500);

                  lcd.clear();

                  lcd.print("GESTIONE CHIAVI");

                cardmas = 1;

                lcd.setCursor(0, 1);

                lcd.print("Chiavi slave: ");

                lcd.setCursor(15, 1);

                lcd.print(slave);

                delay(2000);

                }  else { cardmas = 0;

                  lcd.setCursor(0, 1);

                  lcd.print("Annullata");

                delay(3000);

                standby();

                }

                } 

                // Se invece il seriale letto corrisponde con uno dei tre gruppi 

                // di chiavi memorizzate allora attiva o disattiva l'allarme.

                else if ((sernum0 == EEPROM.read(1) && sernum1 == EEPROM.read(2) && sernum2 == EEPROM.read(3) && sernum3 == EEPROM.read(4) && sernum4 == EEPROM.read(5))

                        || (sernum0 == EEPROM.read(6) && sernum1 == EEPROM.read(7) && sernum2 == EEPROM.read(8) && sernum3 == EEPROM.read(9) && sernum4 == EEPROM.read(10))

                        || (sernum0 == EEPROM.read(11) && sernum1 == EEPROM.read(12) && sernum2 == EEPROM.read(13) && sernum3 == EEPROM.read(14) && sernum4 == EEPROM.read(15))) {

                              

                             if (antiON == 0) {  

                             antiON = 1;

                             lcd.clear();

                             lcd.print("CHIAVE VALIDA");

                             delay(1500);

                             lcd.clear();

                             lcd.print("ANTIFURTO");

                             lcd.setCursor(0, 1);

                             lcd.print("INSERITO...");

                             delay(pausa);

                             digitalWrite(ledRed, HIGH);

                             lcd.clear();

                             lcd.print("ANTIFURTO");

                             lcd.setCursor(0, 1);

                             lcd.print("ATTIVO");

                             } 

                              else {

                                   antiON = 0;

                                   allarm = 0;

                                   digitalWrite(ledRed, LOW);

                                   digitalWrite(ledGreen, LOW);

                                   lcd.clear();

                                   lcd.print("CHIAVE VALIDA");

                                   delay(1500);

                                   lcd.clear();

                                   lcd.print("ANTIFURTO");

                                   lcd.setCursor(0, 1);

                                   lcd.print("DISATTIVO");

                                   delay(3000);

                                   standby();

                    } 

                } 

                // Se il seriale letto è diverso dal master e nemmeno presente in memoria,

                // e se è attiva la modalita Memorizzazione chiavi, salva il seriale in memoria

                // come slave1, slave2 o slave3.

                else if (cardmas == 1 && slave == 0) {

                    lcd.clear();

                    lcd.print("Chiave rilevata!");

                    EEPROM.write(0, 1);

                    EEPROM.write(1, sernum0);

                    EEPROM.write(2, sernum1);

                    EEPROM.write(3, sernum2);

                    EEPROM.write(4, sernum3);

                    EEPROM.write(5, sernum4);

                    cardmas = 0;

                    delay(1000);

                    lcd.setCursor(0, 1);

                    lcd.print("Slave 1 salvata!");

                    delay(3000);

                    standby();

                                        

                }

                    else if (cardmas == 1 && slave == 1) {

                              lcd.clear();

                              lcd.print("Chiave rilevata!");

                              EEPROM.write(0, 2);

                              EEPROM.write(6, sernum0);

                              EEPROM.write(7, sernum1);

                              EEPROM.write(8, sernum2);

                              EEPROM.write(9, sernum3);

                              EEPROM.write(10, sernum4);

                              cardmas = 0;

                              delay(1000);

                              lcd.setCursor(0, 1);

                              lcd.print("Slave 2 salvata!");

                              delay(3000);

                              standby();

                    }

                              else if (cardmas == 1 && slave == 2) {

                                       lcd.clear();

                                       lcd.print("Chiave rilevata!");

                                       EEPROM.write(0, 3);

                                       EEPROM.write(11, sernum0);

                                       EEPROM.write(12, sernum1);

                                       EEPROM.write(13, sernum2);

                                       EEPROM.write(14, sernum3);

                                       EEPROM.write(15, sernum4);

                                       cardmas = 0;

                                       lcd.setCursor(0, 1);

                                       lcd.print("Slave 3 salvata!");

                                       delay(3000);

                                       standby();

                              }

                }

                }

         

  

 // Se l'infrarosso rileva un movimento accende il led giallo

  if (digitalRead(pir) == HIGH) {

          digitalWrite(ledgial, HIGH);

     }

     else {

           digitalWrite(ledgial, LOW);

          }

          

  // Se l'infrarosso rileva un movimento e l'antifurto e attivo 

  // e quindi non in allarme, attiva l'allarme facendo lampeggiare 

  // il led verde e facendo suonare il buzzer     

  if (digitalRead(pir) == HIGH && antiON == 1 && allarm == 0){

          delay(pausa);

          allarm = 1;

          lcd.clear();

          lcd.print("ALLARME!!");

     }

  if (antiON == 1 && allarm == 1) {

          lamp();

          buzz(7, 4186, 100);

          delay(10); // Attesa

     }

     

 // Se è stata attivata con la card Master la modalità "Memorizzazione chiavi",

 // premendo il pulsante di reset si azzera il database 

 // delle chiavi salvate fino a quel momento. 

 if (digitalRead(resetkey) == HIGH && cardmas == 1) {

         cardmas = 0;

         for (int i=0; i<16; i++){

           EEPROM.write(i, 0);

             } 

         lcd.clear();

         lcd.print("Reset chiavi...");

         delay(3000);

         standby();

     }        

                

 rfid.halt();

}

void lamp() {

  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {

    previousMillis = currentMillis; 

    ledState ^= 1;

    digitalWrite(ledGreen, ledState);

  }

}

  

void buzz(int targetPin, long frequency, long length) {

 long delayValue = 1000000/frequency/2;

 long numCycles = frequency * length/ 1000;

 for (long i=0; i < numCycles; i++)

 {

 digitalWrite(targetPin,HIGH);

 delayMicroseconds(delayValue);

 digitalWrite(targetPin,LOW);

 delayMicroseconds(delayValue);

 }

  }

  

void standby() {

 lcd.clear();

 lcd.print("ArduinoDenis"); 

}
