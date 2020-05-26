#include "serialcodes.h"

Codes::Codes() {
    
}

Codes::Codes(HardwareSerial * terminal) {
    this->terminal = terminal;
}

void Codes::error(String message) {
    this->terminal->print("[x] " + message);
}

void Codes::errorln(String message) {
    this->terminal->println("[x] " + message);
}

void Codes::success(String message) {
    this->terminal->print("[+] " + message);
}

void Codes::successln(String message) {
    this->terminal->println("[+] " + message);
}

void Codes::config(String message) {
    this->terminal->print("[?] " + message);
}

void Codes::configln(String message) {
    this->terminal->println("[?] " + message);
}
void Codes::warning(String message) {
    this->terminal->print("[!] " + message);
}

void Codes::warningln(String message) {
    this->terminal->println("[!] " + message);
}


void Codes::mode(String message) {
    this->terminal->println("------  " + message + "  ------");
}


void Codes::print(String message) {
    this->terminal->print(message);
}

void Codes::println(String message) {
    this->terminal->println(message);
}
