#include <Arduino.h>
#include <RCSwitch.h>
#include "../lib/SimpleTimer/SimpleTimer.h"
#include "../lib/QueueArray/QueueArray.h"


SimpleTimer scheduler;
RCSwitch mySwitch = RCSwitch();
QueueArray <unsigned int> queueRSSI;

const int MAX_CUE_NUM = 200;
int statusLED = 11;
int alarmLED = 9;
int buzzerPin = 6;
bool toggleStatusLED = false;
int analogPin = A0;
int meanRSSI = 0;
int averageRSSI = 250;
long int soundAlarmInterval = 2000;
long int statusLEDBlinkInterval = 500;
long int storeNewKeyInterval = 200;
long int checkQueueInterval = 3000;
long int detectJammingInterval = 6000;
long int readRSSIValuesInterval = 200;
//unsigned int prueba2[20] = {0, 2, 3, 4, 5, 1, 1, 1, 1, 10, 11, 12, 10, 0, 0, 0, 0, 18, 0, 20};
unsigned int keyValues[MAX_CUE_NUM] = {};
int count = 0;
bool repetido = false;
bool soundAlarmCondition = false;
unsigned int key = 0;

bool firstIteration = true;
int head = 0;
int tail = 0;
int offset = 15;
int increment = 0;

void statusLEDBlink(){
    toggleStatusLED = !toggleStatusLED;
    digitalWrite(statusLED, toggleStatusLED);
}

void soundAlarm(){
    static int alarmCount = 0;

    if(soundAlarmCondition == true){
        //Serial.println("Sounding alarm");
        alarmCount++;
        digitalWrite(alarmLED, HIGH);
        digitalWrite(buzzerPin, HIGH);
        if(alarmCount > 5){
            //Serial.println("Alarm");
            soundAlarmCondition = false;
            alarmCount = 0;
            digitalWrite(alarmLED, LOW);
            digitalWrite(buzzerPin, LOW);
        }
    }

    //Serial.println(alarmCount);
}

void firstRSSIReading(){ // Executed only 1 time
    // Get first RSSI mean
    for(int i = 0; i < 50; i++){
        meanRSSI = meanRSSI + analogRead(analogPin);
    }

    meanRSSI = meanRSSI / 50;

    //Serial.print("Mean RSSI first read: ");
    //Serial.println(meanRSSI);
}

void detectJamming(){ // Executed periodically
    // Read RSSI values and compare them
    //Serial.print("Jamming detection: ");
    int count = 0;
    int tempMeanRSSI = 0;
    int queueValue = 0;
    int valuesOverAverage = 0;
    count = queueRSSI.count();
    if(!queueRSSI.isEmpty()){
        for(int i = 0; i < count; i++){
            queueValue = queueRSSI.dequeue();
            if(queueValue >= averageRSSI){
                valuesOverAverage++;
            }
            tempMeanRSSI = tempMeanRSSI + queueValue;
        }
        tempMeanRSSI = tempMeanRSSI / count;
    }

    if(valuesOverAverage >= 20){
        //Serial.println("Jamming detected, valores por encima de la media");
        soundAlarmCondition = true;
    }

    if(abs(tempMeanRSSI - meanRSSI) > 150){
        //Serial.println("Jamming detected, diferencia entre medias alta");
        soundAlarmCondition = true;
    }

    meanRSSI = tempMeanRSSI;

}

void readRSSIValues(){
    int RSSI = 0;
    RSSI = analogRead(analogPin);
    queueRSSI.enqueue(RSSI);
}

void storeNewKeyInQueue(){ // Executed periodically every 500 milliseconds
    // Store new key in queue, if available
    //Serial.println("Saving key values");
    static unsigned int previousValue = 0;
    static unsigned int repeatedCount = 0;

    if(mySwitch.available()) {
        key = mySwitch.getReceivedValue();
        // Serial.print("Received: ");
        // Serial.println(key);
        if(previousValue == key && key != 0){
        //if(previousValue == key){
            repeatedCount++;
            keyValues[head] = 0;
            //previousValue = 0;
        } else {
            keyValues[head] = key;
            repeatedCount = 0;
            //previousValue = key;
        }
        mySwitch.resetAvailable();
        previousValue = key;
    } else {
        keyValues[head] = 0;
        previousValue = 0;
    }

    // Serial.print(keyValues[head]);
    // Serial.print("    ");
   
    if(repeatedCount >= 15){
        //Serial.println("Jamming detected, numero repetido");
        soundAlarmCondition = true;
        repeatedCount = 0;
    }
    // previousValue = key;    
    // Serial.print(previousValue);
    // Serial.print("    ");

    // Serial.print("Head: ");
    // Serial.println(head);

    head = (head + 1) % MAX_CUE_NUM;
}

void checkQueueRepeatedKeys(){ // Executed periodically every 5 seconds
    // Read RF key values stored in queue
    // Serial.println("Checking queue"); 

    if(firstIteration == false){
        scheduler.disable(0);
        //Serial.println("Checking queue");
        tail = increment;
        int tempTail = tail + 1;
        for(int count = 0; count < 30; count ++){
            //Serial.print("Tail: ");
            //Serial.println(tail);
            //Serial.print("Valor array: ");
            //Serial.println(keyValues[tail]);
            if(keyValues[tail] != 0){
                for(int innerCount = 0; innerCount < (29 - count); innerCount++){                
                    // Serial.print("Temp tail: ");
                    // Serial.print(tempTail);
                    // Serial.print("            Head: ");
                    // Serial.println(head);
                    if (keyValues[tail] == keyValues[tempTail]){
                        //Serial.print("Repetido: ");
                        //Serial.println(keyValues[tail]);
                        repetido = true;
                        soundAlarmCondition = true;
                        memset(keyValues, 0, sizeof(keyValues));
                        break;
                    }
                    tempTail = (tempTail + 1) % MAX_CUE_NUM;                
                }
            }
            
            if(repetido == true)
            {
                break;
            }
            tail = (tail + 1) % MAX_CUE_NUM;
            tempTail = tail + 1;
        } 
        increment = (increment + offset) % MAX_CUE_NUM;
        repetido = false;
        // Serial.print("Increment: ");
        // Serial.println(increment);
        scheduler.enable(0);
    }

    firstIteration = false;
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    firstRSSIReading();

    mySwitch.enableReceive(0);

    pinMode(buzzerPin, OUTPUT);
    pinMode(statusLED, OUTPUT);
    pinMode(alarmLED, OUTPUT);

    digitalWrite(buzzerPin, LOW);
    digitalWrite(statusLED, LOW);
    digitalWrite(alarmLED, LOW);

    scheduler.setInterval(storeNewKeyInterval, storeNewKeyInQueue);
    scheduler.setInterval(checkQueueInterval, checkQueueRepeatedKeys);
    scheduler.setInterval(detectJammingInterval, detectJamming);
    scheduler.setInterval(readRSSIValuesInterval, readRSSIValues);
    scheduler.setInterval(statusLEDBlinkInterval, statusLEDBlink);
    scheduler.setInterval(soundAlarmInterval, soundAlarm);
}

void loop() {
    // put your main code here, to run repeatedly:
    scheduler.run();
}