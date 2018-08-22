#include <Arduino.h>
#include "../lib/QueueArray/QueueArray.h"
#include "../lib/StackList/Stacklist.h"

QueueArray <unsigned int> queue;
StackList <unsigned int> stack;
//const String msg = "Happy Hacking!";
unsigned int prueba[30] = {};
unsigned int prueba2[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
//QueueArray <char> queue;
int count = 0;
int comparador = 0;
bool repetido = false;

void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(0));
    queue.setPrinter(Serial);
    stack.setPrinter(Serial);
    for (int i = 0; i < 20; i++){
        //queue.enqueue(random(1, 10));
        queue.enqueue(prueba2[i]);
    }
    count = queue.count();
    Serial.println(count);
    prueba[0] = queue.dequeue();
    Serial.println(prueba[0]);

    for(int i = 1; i < count; i++){
        prueba[i] = queue.dequeue();
        Serial.println(prueba[i]);
        for(int j = i - 1; j >= 0; j--){
            if(prueba[i] == prueba[j]){
                Serial.print("Repetido: ");
                Serial.println(prueba[i]);
                repetido = true;
            }                        
        }
    }

    Serial.println(queue.count());
    Serial.println(queue.isEmpty());

}

void loop() {
    // put your main code here, to run repeatedly:
}