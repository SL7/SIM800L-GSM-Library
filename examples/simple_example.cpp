#include <Arduino.h>
#include "gsm.h"

#define GSM_Serial Serial3

const int CONSOLE_BAUD = 115200;
const int GPS_BAUD = 9600;
const int GSM_BAUD = 115200;


//replace this with you destenation Number
const char * number = "12345678910";

GSM gsm(&GSM_Serial, &Serial1);                                                                     // Create the GSM Object

//GSM gsm;

void setup() {
    Serial.begin(CONSOLE_BAUD);
    GSM_Serial.begin(GSM_BAUD);
    gsm.initGSM(PIN);                                                                               // Select if you want to provide a PIN or not (PIN / NO_PIN)
    gsm.setPIN(1234);                                                                               // Set yout PIN Code with this function
    delay(50);
    gsm.sendSMS("Hello.\nThis is a message from the SIM800L module. :D" , DE, number);              // Send a SMS message specifying the Message, Countrycode of the Destenation Number, and the number
    delay(1000);
}

void loop() {
    delay(1000);
}
