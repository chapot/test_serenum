#include <Arduino.h>
#include <RCSwitch.h>

// Hay 5 protocolos

RCSwitch mySwitch = RCSwitch();
int msgProtocol = 1;
unsigned long int msgSend = 141516;
int msgDelay = 5000;
int msgRepeat = 1;

void setup() {
    Serial.begin(9600);
    mySwitch.enableTransmit(10);
    randomSeed(analogRead(0));
    mySwitch.setPulseLength(500);
    mySwitch.setRepeatTransmit(4);
    //mySwitch.enableReceive(0);
}

void loop() {
    msgProtocol = random(1,7);
    msgSend = random(1, 10);
    msgDelay = random(200, 1000);
    msgRepeat = random(1,2);
    mySwitch.setRepeatTransmit(4);
    mySwitch.setProtocol(msgProtocol);
    mySwitch.send(msgSend, 24);
    Serial.print("Send: ");
    Serial.println(msgSend);
    Serial.print("Protocol: ");
    Serial.println(msgProtocol);
    Serial.print("Delay: ");
    Serial.println(msgDelay);
    delay(msgDelay);

    // if(mySwitch.available()) {
    //     Serial.print("Received: ");
    //     Serial.println(mySwitch.getReceivedValue());
    //     Serial.print("Protocol: ");
    //     Serial.println(mySwitch.getReceivedProtocol());
    //     delay(300);
    //     mySwitch.resetAvailable();
    // }
}