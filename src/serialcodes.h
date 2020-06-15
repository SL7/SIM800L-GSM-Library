#pragma once

#include <Arduino.h>

class Codes;

class Codes {

private:
    HardwareSerial * terminal;
public:
    Codes();
    Codes(HardwareSerial * serial);
    void error(String message);
    void errorln(String message);
    void success(String message);
    void successln(String message);
    void config(String message);
    void configln(String message);
    void warning(String message);
    void warningln(String message);

    void mode(String message);

    void print(String = "");
    void println(String = "");
};