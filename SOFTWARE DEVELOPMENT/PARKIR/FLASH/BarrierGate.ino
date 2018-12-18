/**
 * REFERENSI :
 *
 */
#include <TimedAction.h>

// Input signal
#define IN1 A0 // loop1
#define IN2 A1 // tombol tiket
#define IN3 A2 // loop2
#define IN4 A3

// Output
#define OUT1 8
#define OUT2 7
#define OUT3 6
#define OUT4 5
#define OUT5 4

// Wiegan Card 125KHz
#define WD0 2
#define WD1 3

// LED POWER BLINK
#define LED0 9 

int led_flag = false;

// Sensitifity Sensor Input
// Max 1024
#define SENSITIFITY_SENSOR 1022

void setup() {
  // Serial
  Serial.begin(9600);
  
  // Set output
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  pinMode(OUT5, OUTPUT);

  pinMode(LED0, OUTPUT);
}

void start(String a) {

  if(a.length() > 0) {
    if(a == "Q") {
      digitalWrite(OUT1,HIGH);
      digitalWrite(OUT2,HIGH);
      digitalWrite(OUT3,HIGH);
      delay(1000);
      digitalWrite(OUT1,LOW);
      digitalWrite(OUT2,LOW);
      digitalWrite(OUT3,LOW);
    } else if(a == "P") {
      Serial.print("X");
    }
  }  
  
  // read analog input
  int AV1 = analogRead(IN1);
  int AV2 = analogRead(IN2);
  int AV3 = analogRead(IN3);
  
  bool An1 = false;
  bool An2 = false;
  bool An3 = false;

  // Input sensor dengan ground
  // optocouple PC817
  if(AV1 == 0) {
    An1 = true;
  }
  if(AV2 == 0) {
    An2 = true;
  }
  if(AV3 == 0) {
    An3 = true;
  }

  // loop1(0),tombol(1),loop2(1)
  if((An1 == true) && (An2 == false) && (An3 == false)) {
    Serial.print("A");
  }
  // loop1(1),tombol(0),loop2(1)
  if((An1 == false) && (An2 == true) && (An3 == false)) {
    Serial.print("C");
  }
  // loop1(1),tombol(1),loop2(0)
  if((An1 == false) && (An2 == false) && (An3 == true)) {
    Serial.print("E");
  }
  // loop1(1),tombol(1),loop2(1)
  if((An1 == false) && (An2 == false) && (An3 == false)) {
    Serial.print("R");
  }
}

void receiveSerial() {
  String a = "";
  while(Serial.available()) {
      a = Serial.readString();// read the incoming data as string
  }
  a.trim(); // REMOVE WHITESPACES FROM STRING 
  Serial.print(a);
  start(a);
}

void ledCtrl() {
  if(!led_flag) {
    digitalWrite(LED0,HIGH);
    led_flag = true;
  } else {
    digitalWrite(LED0,LOW);
    led_flag = false;
  }
}

TimedAction t_ReceiveSerial = TimedAction(150,receiveSerial);
TimedAction t_LedCtrl = TimedAction(500,ledCtrl);

void loop() {
  t_ReceiveSerial.check();
  t_LedCtrl.check();
}