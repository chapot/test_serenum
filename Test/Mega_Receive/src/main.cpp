#include <Arduino.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
int analogPin = A0;
int val = 0;
int msgCount = 0;

void setup() {
    Serial.begin(9600);
    
    mySwitch.enableReceive(0);
}

void loop() {    
    if(mySwitch.available()) {
        msgCount++;
        Serial.print("Received: ");
        Serial.println(mySwitch.getReceivedValue());
        Serial.print("Protocol: ");
        Serial.println(mySwitch.getReceivedProtocol());
        delay(300);
        mySwitch.resetAvailable();
    }
}