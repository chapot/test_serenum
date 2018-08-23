#include <Arduino.h>
#include <RCSwitch.h>

// Hay 5 protocolos

RCSwitch mySwitch = RCSwitch();
int msgProtocol = 1;
unsigned long int msgSend = 141516;
int msgDelay = 3000;
int msgRepeat = 4;

void setup() {
    Serial.begin(9600);
    mySwitch.enableTransmit(10);
    randomSeed(analogRead(0));
    mySwitch.setPulseLength(500);
    mySwitch.setRepeatTransmit(1);
}

void loop() {
    // msgProtocol = random(1,7);
    // msgSend = random(1000, 40000);
    // msgDelay = random(1000, 5000);
    // msgRepeat = random(1,2);
    //mySwitch.setRepeatTransmit(8);
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


