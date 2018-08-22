#include <Arduino.h>
#include <RCSwitch.h>
#include "../lib/SimpleTimer/SimpleTimer.h"
#include "../lib/QueueArray/QueueArray.h"

SimpleTimer scheduler;
QueueArray <unsigned int> queue;
RCSwitch mySwitch = RCSwitch();


int analogPin = A0;
int meanRSSI = 0;
long int storeNewKeyInterval = 300;
long int checkQueueInterval = 5000;
long int detectJammingInterval = 10000;
unsigned int prueba[30] = {};
int count = 0;
bool repetido = false;
unsigned int key = 0;

void firstRSSIReading(){ // Executed only 1 time
    // Get first RSSI mean
    for(int i = 0; i < 50; i++){
        meanRSSI = meanRSSI + analogRead(analogPin);
    }

    meanRSSI = meanRSSI / 50;

    Serial.print("Mean RSSI first read: ");
    Serial.println(meanRSSI);
}

void detectJamming(){ // Executed periodically
    // Read RSSI values and compare them
    meanRSSI = analogRead(analogPin);
    Serial.print("Jamming detection: ");
    Serial.println(meanRSSI);
}

void checkQueueRepeatedKeys(){ // Executed periodically every 5 seconds
    // Read RF key values stored in queue
    Serial.println("Checking queue");
    count = queue.count();
    if(!queue.isEmpty()){
        prueba[0] = queue.dequeue();
        for(int i = 1; i < count; i++){
            prueba[i] = queue.dequeue();
            for(int j = i - 1; j >= 0; j--){
                if(prueba[i] == prueba[j]){
                    Serial.print("Repetido: ");
                    Serial.println(prueba[i]);
                    repetido = true;
                }                        
            }
        }
    }    
}

void storeNewKeyInQueue(){ // Executed periodically every 500 milliseconds
    // Store new key in queue, if available
    if(mySwitch.available()) {
        key = mySwitch.getReceivedValue();
        Serial.print("Received: ");
        Serial.println(key);
        //Serial.print("Protocol: ");
        //Serial.println(mySwitch.getReceivedProtocol());
        queue.enqueue(key);
        mySwitch.resetAvailable();
    }
}

void soundAlarm(){
    // Activate alarm for 10 seconds
}

void setupScheduler(){ 
    // Configure scheduler
    scheduler.setInterval(storeNewKeyInterval, storeNewKeyInQueue);
    scheduler.setInterval(checkQueueInterval, checkQueueRepeatedKeys);
    scheduler.setInterval(detectJammingInterval, detectJamming);
}

void setup() {
    Serial.begin(9600);    
    queue.setPrinter(Serial);
    mySwitch.enableReceive(0);
    firstRSSIReading();
    setupScheduler();
}

void loop() {    
    scheduler.run();
}