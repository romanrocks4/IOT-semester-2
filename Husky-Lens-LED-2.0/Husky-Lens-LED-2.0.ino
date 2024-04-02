// This includes the librarys.
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

// This sets up the pins for the Husky Lens.
HUSKYLENS huskylens;
SoftwareSerial mySerial(9, 8); // RX, TX.
//HUSKYLENS green line >> Pin 9; blue line >> Pin 8.
void printResult(HUSKYLENSResult result);

//This sets up the pins for the LEDS.
const int buttonPin = 2;
    const int ledPinBL = 13;
    const int ledPinTL = 11;
    const int ledPinBR = 12;
    const int ledPinTR = 10;

// This sets up the baud to 9600, checks the connection of the Husky Lens, and sets all the pins to output.
void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
  pinMode(ledPinBL, OUTPUT);
  pinMode(ledPinTL, OUTPUT);
  pinMode(ledPinBR, OUTPUT);
  pinMode(ledPinTR, OUTPUT);

}


void loop() {
    if (!huskylens.request()) {
      //This prints if the Husky Lens does not communicate.
      Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
     
    } else if(!huskylens.isLearned()) {
      //This prints if the Husky Lens has not learned anything.
      Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
      
    } else if(!huskylens.available()) {
      //This prints if the Husky Lens does not see anything on the screen and sets all the LEDs off.
      Serial.println(F("No block or arrow appears on the screen!")); 
      digitalWrite(ledPinBL, LOW);
      digitalWrite(ledPinBR, LOW);
      digitalWrite(ledPinTL, LOW);
      digitalWrite(ledPinTR, LOW);
    } else
    {
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
        }    
    }
}

void printResult(HUSKYLENSResult result){

   //This activates if the Husky Lens detects an object.
    if (result.command == COMMAND_RETURN_BLOCK) {
          // This activates when the objects enters the top left of the screen, then turns on the top left LED and sends the info to the serial monitor.
          if (result.xCenter < 160 && result.yCenter < 120){
            analogWrite(ledPinBL, LOW);
            digitalWrite(ledPinBR, LOW);
            digitalWrite(ledPinTL, HIGH);
            digitalWrite(ledPinTR, LOW);
            Serial.println(F("Top left"));
          }
          // This activates when the objects enters the top right of the screen, then turns on the top right LED and sends the info to the serial monitor.
          if (result.xCenter > 160 && result.yCenter < 120){
            digitalWrite(ledPinBL, LOW);
            digitalWrite(ledPinBR, LOW);
            digitalWrite(ledPinTL, LOW);
            digitalWrite(ledPinTR, HIGH);
            Serial.println(F("Top right"));
          }
          // This activates when the objects enters the bottom left of the screen, then turns on the bottom left LED and sends the info to the serial monitor.
          if (result.xCenter < 160 && result.yCenter > 120){
            digitalWrite(ledPinBL, HIGH);
            digitalWrite(ledPinBR, LOW);
            digitalWrite(ledPinTL, LOW);
            digitalWrite(ledPinTR, LOW);
            Serial.println(F("Bottom left"));
          }
          // This activates when the objects enters the bottom right of the screen, then turns on the bottom right LED and sends the info to the serial monitor.
          if (result.xCenter > 160 && result.yCenter > 120){
            digitalWrite(ledPinBL, LOW);
            digitalWrite(ledPinBR, HIGH);
            digitalWrite(ledPinTL, LOW);
            digitalWrite(ledPinTR, LOW);
            Serial.println(F("Bottom right"));
          }
      
    } 
}