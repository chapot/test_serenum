#include <Arduino.h>
#include <RCSwitch.h>

// Hay 5 protocolos

RCSwitch mySwitch = RCSwitch();
int msgProtocol = 3;
int msgSend = 5050;
int msgDelay = 500;
int msgRepeat = 2;

void setup() {
    Serial.begin(9600);
    mySwitch.enableTransmit(10);
    randomSeed(analogRead(0));
    //mySwitch.setRepeatTransmit(msgRepeat);
}

void loop() {
    msgProtocol = random(1,4);
    msgSend = random(1, 1000);
    msgDelay = random(2000, 10000);
    // msgRepeat = random(1,2);
    // mySwitch.setRepeatTransmit(msgRepeat);
    mySwitch.setProtocol(msgProtocol);
    mySwitch.send(msgSend, 24);
    Serial.print("Send: ");
    Serial.println(msgSend);
    Serial.print("Protocol: ");
    Serial.println(msgProtocol);
    Serial.print("Delay: ");
    Serial.println(msgDelay);
    delay(msgDelay);
}