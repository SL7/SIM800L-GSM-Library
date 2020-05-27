#include "gsm.h"

GSM::GSM(HardwareSerial * gsm_serial, HardwareSerial * console) {
    this->gsm = gsm_serial;
    this->terminal = Codes(console);
    this->term = console;
}


//TODO: Some more Commands
String GSM::cmnd_str(commands command) {
    switch(command) {
        case AT:
            return "AT";
        case SET_PIN:
            return "AT+CPIN";
        case SET_SMS_MODE:
            return "AT+CMGF=1";
        case GET_SMS_CONFIG:
            return "AT+CSMP?";
        case SEND_SMS:
            return "AT+CMGS";
        case START_TCPUDP:
            return "AT+CIPSTART";
        case STOP_TCPUDP:
            return "AT+CIPCLOSE";
        case LOCAL_IP:
            return "AT+CIFSR";
        case LIST_SMS:
            return "AT+CMGL";
        case SMS_FORMAT:
            return "AT+CMGF";
        case DELETE_SMS:
            return "AT+CMGD";
        default:
            return "AT";
    }
}

bool GSM::delete_SMS(int index) {
    if (index == 0 ) {
        for(uint16_t n = 1; n < 11; n++) {
            gsm->println(cmnd_str(DELETE_SMS) + "=" + String(n) + ",0");
            delay(200);
        }
        return true;
    } else {
        gsm->println(cmnd_str(DELETE_SMS) + "=" + String(index) + ",0");
        delay(200);
        if (gsm->available()) {
            if (gsm->readString().indexOf("OK") > 0) {
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

void GSM::at_test() {
    gsm->println("AT");
    Serial1.println("AT");
    if (gsm->available()) {
        String str;
        while(gsm->available()) {
            str += (char) gsm->read();
        }
        if (str.indexOf("OK") > 0) {
            this->terminal.configln("YES");
            return;
        } else {
            terminal.configln("NO");
        }
    } else {
        terminal.errorln("Not available");
    }
}

void GSM::initGSM(pinmodes modes) {
    this->mode = modes;
    int counter = 0;
    delay(3000);
    terminal.mode("GSM MODULE");
    
    terminal.println();

    if (modes == NO_PIN) {
        this->pin_set = true;
        terminal.configln("NO PIN Mode selected");
    }
    counter = 0;
    terminal.println();
    terminal.config("GSM Initializing");
    while(1) {
        gsm->println("AT");
        //delay(50);
        //terminal.println(gsm->readString());
        if (gsm->available()) {
            String feedback; 
            while(gsm->available()) {
                feedback += (char) gsm->read();
                //terminal.println("test");
            }
            if (feedback.indexOf("OK") > 0) {
                terminal.println();
                terminal.successln("GSM Module initialized");
                break;
            }   
            
        } else {
            terminal.print(".");
        }
        if (counter >= 120) {
            break;
        }
        counter++;
        delay(1000);
    }
    counter = 0;
    terminal.println();
}

void GSM::setPIN(int pin) {
    switch(this->mode) {
        case NO_PIN:
            terminal.warningln("Selected NO PIN mode");
            terminal.warningln("No Pin will be set");
            break;
        case PIN:
            terminal.configln("Setting Pin...");

            gsm->println(cmnd_str(SET_PIN) + "=\"" + String(pin) + "\"");
            delay(300);
            String out = "";
            if (gsm->available() ) {
                out = gsm->readString();
            }
            if (out.indexOf("OK") > 0) {
                terminal.successln("Pin set");
            } else {
                terminal.errorln("Pin couldn't be set");
            }
            //terminal.successln("PIN set");
            break;
    }
}

void GSM::sendSMS(String message, countrycode code, String number) {
    String country;
    switch (code) {
        case DE:
            country = "+49";
            break;
        case US:
            country = "+01";
            break;
        case RU:
            country = "+07";
            break;
        case IT:
            country = "+39";
            break;
        default:
            country = "+49";
            break;
    }

    if (this->pin_set) {
        this->gsm->println("AT+CMGF=1");
        //terminal.warningln(this->gsm->readString());
        delay(100);
        if (this->gsm->available()) {
            String str;
            str = gsm->readString();
            if (str.indexOf("OK") == 0){
                terminal.println();
                terminal.errorln("There was an error setting the SMS Mode");
                return;
            }   
        }
        else {
            terminal.errorln("GSM not responding.");
            terminal.errorln("Check wiring or you are not connected");
        }
        this->gsm->println(cmnd_str(SEND_SMS) + "=\"" + country + number + "\"");
        delay(100);
        this->gsm->println(message); 
        delay(100); 
        this->gsm->write(26);
        if (this->gsm->available()) {
            String out = gsm->readString();
            if (out.indexOf(number) > 0) {
                terminal.successln("Message Sent...");
                return;
            } else {
                terminal.errorln("Error sending the message");
            }
        }

    } else {
        terminal.println();
        terminal.warningln("Set PIN before sending messages");
    }
}


String GSM::rxSMS(String number, countrycode code, int change_read_unread) {
    String out;
    String country;

    switch (code) {
        case DE:
            country = "+49";
            break;
        case US:
            country = "+01";
            break;
        case RU:
            country = "+07";
            break;
        case IT:
            country = "+39";
            break;
        default:
            country = "+49";
            break;
    }

    gsm->println(cmnd_str(SMS_FORMAT) + "=1");
    delay(200);
    gsm->println(cmnd_str(LIST_SMS) + "=\"REC UNREAD\"," + String(change_read_unread));
    delay(200);
    while (gsm->available()) {
        out = gsm->readString();
    }
    //terminal.println(out);
    if (out.indexOf(country + number) > 0) {
        return "[+] Message: " + out.substring(out.lastIndexOf('"') + 1, out.indexOf("OK") - 2);
    } else {
        return "null";
    }
}


String GSM::allSMS() {
    String out;
    gsm->println(cmnd_str(SMS_FORMAT) + "=1");
    delay(200);
    gsm->println(cmnd_str(LIST_SMS) + "=\"ALL\",1");
    delay(200);
    while(gsm->available()) {
        out = gsm->readString();
    }

    return out;
}



