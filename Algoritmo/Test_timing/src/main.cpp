#include <Arduino.h>
#include "../lib/SimpleTimer/SimpleTimer.h"

SimpleTimer timer;

bool timerStop = false;
bool timerEnable = false;


// function to be called repeatedly
void RepeatTask() {
  static int j = 0;
  j++;
  Serial.print("15 second timer, called ");
  Serial.print(j);
  Serial.println(" times");  
  if(j >= 2){
    timerStop = true;
  }
}
 
// function to be called just once
void OnceOnlyTask() {
  Serial.println("This timer only triggers once");  
}
 
// function to be called exactly 10 times
void TenTimesTask() {
  static int k = 0;
  k++;
  Serial.print("called ");
  Serial.print(k);
  Serial.println(" / 10 times.");
}

//
// utility function for digital clock display:
// prints preceding colon and leading 0
//
void printDigits(int digits) {
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

// print current arduino "uptime" on the serial port
void DigitalClockDisplay() {
  int h,m,s;
  s = millis() / 1000;
  m = s / 60;
  h = s / 3600;
  s = s - m * 60;
  m = m - h * 60;
  Serial.print(h);
  printDigits(m);
  printDigits(s);
  Serial.println();
}
 

 
void setup() {
  Serial.begin(9600);
 
  // welcome message
  Serial.println("SimpleTimer Example");
  Serial.println("One timer is triggered every 15 seconds");
  Serial.println("Another timer is set to trigger only once after 10 seconds");
  Serial.println("Another timer is set to trigger 10 times");
  Serial.println();
 
  // timed actions setup
  timer.setInterval(15000, RepeatTask);
  timer.setTimeout(10000, OnceOnlyTask);
  timer.setInterval(1000, DigitalClockDisplay);
  timer.setTimer(1200, TenTimesTask, 10);
}
 
void loop() {
  // this is where the "polling" occurs
  timer.run();
  if(timerStop == true){
    timer.disable(0);
    Serial.println("Timer disabled");
    timerStop = false;
  }
}