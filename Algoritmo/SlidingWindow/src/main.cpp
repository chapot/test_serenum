#include <Arduino.h>
#include "../lib/SimpleTimer/SimpleTimer.h"
#include "../lib/QueueArray/QueueArray.h"

SimpleTimer scheduler;
QueueArray <unsigned int> queue;

int analogPin = A0;
int meanRSSI = 0;
long int storeNewKeyInterval = 1000;
long int checkQueueInterval = 5000;
long int detectJammingInterval = 2500;
long int jammingSampling = 500;
unsigned int prueba2[20] = {0, 2, 3, 4, 5, 1, 1, 1, 1, 10, 11, 12, 10, 0, 0, 0, 0, 18, 0, 20};
unsigned int prueba[20] = {};
unsigned int pruebaJamming[20] = {};
unsigned int pruebaJamming2[20] = {};
int count = 0;
bool repetido = false;
unsigned int key = 0;

bool firstIteration = true;
int head = 0;
int tail = 0;
int offset = 5;
int increment = 0;

bool jammingFirstIteration = true;
int jammingHead = 0;
int jammingTail = 0;
int jammingOffset = 5;
int jammingIncrement = 0;

void storeNewKeyInQueue(){ // Executed periodically every 500 milliseconds
    // Store new key in queue, if available
    //Serial.println("Saving key values");
    static unsigned int previousValue = 0;
    static unsigned int repeatedCount = 0;
    if(previousValue == prueba2[head] && prueba2[head] != 0){
        repeatedCount++;
        prueba[head] = 0;
    } else {
        prueba[head] = prueba2[head];
        repeatedCount = 0;
    }

    if(repeatedCount >= 3){
        Serial.println("Jamming detected");
        repeatedCount = 0;
    }
    previousValue = prueba2[head];    

    Serial.print("Head: ");
    Serial.println(head);

    head = (head + 1) % 20;
}

void detectJamming(){ // Executed periodically
    // Read RSSI values and compare them
    //meanRSSI = analogRead(analogPin);
    //Serial.print("Jamming detection: ");
    //Serial.println(meanRSSI);
}

void checkQueueRepeatedKeys(){ // Executed periodically every 5 seconds
    // Read RF key values stored in queue
    // Serial.println("Checking queue"); 

    if(firstIteration == false){
        scheduler.disable(0);
        Serial.println("Checking queue");
        tail = increment;
        int tempTail = tail + 1;
        for(int count = 0; count < 10; count ++){
            //Serial.print("Tail: ");
            //Serial.println(tail);
            //Serial.print("Valor array: ");
            //Serial.println(prueba[tail]);
            if(prueba[tail] != 0){
                for(int innerCount = 0; innerCount < (9 - count); innerCount++){                
                    // Serial.print("Temp tail: ");
                    // Serial.print(tempTail);
                    // Serial.print("            Head: ");
                    // Serial.println(head);
                    if (prueba[tail] == prueba[tempTail]){
                        Serial.print("Repetido: ");
                        Serial.println(prueba[tail]);
                        repetido = true;
                        break;
                    }
                    tempTail = (tempTail + 1) % 20;                
                }
            }
            
            if(repetido == true)
            {
                break;
            }
            tail = (tail + 1) % 20;
            tempTail = tail + 1;
        } 
        increment = (increment + offset) % 20;
        repetido = false;
        Serial.print("Increment: ");
        Serial.println(increment);
        scheduler.enable(0);
    }

    firstIteration = false;
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    randomSeed(analogRead(0));
    scheduler.setInterval(storeNewKeyInterval, storeNewKeyInQueue);
    scheduler.setInterval(checkQueueInterval, checkQueueRepeatedKeys);
    scheduler.setInterval(detectJammingInterval, detectJamming);
}

void loop() {
    // put your main code here, to run repeatedly:
    scheduler.run();
}