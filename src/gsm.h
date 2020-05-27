#pragma once

#include "Arduino.h"
#include "IPAddress.h"
#include "serialcodes.h"

class GSM;

enum commands {
    AT,
    SET_PIN,
    SET_SMS_MODE,
    GET_SMS_CONFIG,
    SEND_SMS,
    START_TCPUDP,
    STOP_TCPUDP,
    LOCAL_IP,
    LIST_SMS,
    READ_SMS,
    SMS_FORMAT,
    DELETE_SMS,
};

enum pinmodes {
    NO_PIN,
    PIN,
};

enum countrycode { 
    DE,
    US,
    RU,
    IT,
};



class GSM {
private:
    HardwareSerial * gsm;
    HardwareSerial * term;
    Codes terminal;
    bool pin_set = false;
    pinmodes mode;
    String cmnd_str(commands command);
    
public:
    bool delete_SMS(int = 0);
    GSM();
    GSM(HardwareSerial * gsm_serial, HardwareSerial * console);
    void initGSM(pinmodes modes);
    void setPIN(int pin = 0000);

    void sendSMS(String msg, countrycode code, String number);
    String rxSMS(String number, countrycode code, int = 0);
    void at_test();
    void self_test();
};