/**
 * REFERENSI :
 *
 */
#include <ResponsiveAnalogRead.h>
#include <TimedAction.h>

// Input signal
#define IN1 A0 // loop1
#define IN2 A1 // tombol tiket
#define IN3 A2 // loop2
#define IN4 A3 // not use

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

int anti_bounching_1 = 0;
int anti_bounching_2 = 0;
int anti_bounching_3 = 0;
int anti_bounching_4 = 0;
int anti_bounching_5 = 0;
int anti_bounching_6 = 0;
int anti_bounching_7 = 0;

// batas karakter yg di accept/setujui untuk muncul
int threshold = 6;

// Anti bounching script
ResponsiveAnalogRead analog1(IN1, true, 1.0);
ResponsiveAnalogRead analog2(IN2, true, 1.0);
ResponsiveAnalogRead analog3(IN3, true, 1.0);

void setup() {
  // Serial
  Serial.begin(9600);
  
  // Set output
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  pinMode(OUT5, OUTPUT)
  pinMode(IN1, INPUT);
  pinMode(IN2, INPUT);
  pinMode(IN3, INPUT);
  pinMode(IN4, INPUT);

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
      return;
    } else if(a == "P") {
      Serial.print("X");
      return;
    }
  }  

  analog1.update();
  analog2.update();
  analog3.update();
  
  // read analog input
  int AV1 = analog1.getValue();
  int AV2 = analog2.getValue();
  int AV3 = analog3.getValue();
  
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
    anti_bounching_1++;
    if(anti_bounching_1 > threshold) {
       Serial.print("A"); 
    }
    return;
  }
  anti_bounching_1 = 0;
  
  // loop1(1),tombol(0),loop2(1)
  if((An1 == false) && (An2 == true) && (An3 == false)) {
    anti_bounching_2++;
    if(anti_bounching_2 > threshold) {
       Serial.print("C"); 
    }
    return;
  }
  anti_bounching_2 = 0;
  
  // loop1(1),tombol(1),loop2(0)
  if((An1 == false) && (An2 == false) && (An3 == true)) {
    anti_bounching_3++;
    if(anti_bounching_3 > threshold) {
       Serial.print("E"); 
    }
    return;
  }
  anti_bounching_3 = 0;
  
  // loop1(0),tombol(0),loop2(1)
  if((An1 == true) && (An2 == true) && (An3 == false)) {
    anti_bounching_4++;
    if(anti_bounching_4 > threshold) {
       Serial.print("B"); 
    }
    return;
  }
  anti_bounching_4 = 0;
  
  // loop1(1),tombol(0),loop2(0)
  if((An1 == false) && (An2 == true) && (An3 == true)) {
    anti_bounching_5++;
    if(anti_bounching_5 > threshold) {
       Serial.print("D"); 
    }
    return;
  }
  anti_bounching_5 = 0;
  
  // loop1(0),tombol(1),loop2(0)
  if((An1 == true) && (An2 == false) && (An3 == true)) {
    anti_bounching_6++;
    if(anti_bounching_6 > threshold) {
       Serial.print("H"); 
    }
    return;
  }
  anti_bounching_6 = 0;
  
  // loop1(0),tombol(0),loop2(0)
  if((An1 == true) && (An2 == true) && (An3 == true)) {
    anti_bounching_7++;
    if(anti_bounching_7 > threshold) {
       Serial.print("G"); 
    }
    return;
  }
  anti_bounching_7 = 0;
  
  // loop1(1),tombol(1),loop2(1)
  if((An1 == false) && (An2 == false) && (An3 == false)) {
    Serial.print("R");
    return;
  }

}

void receiveSerial() {
  String a = "";
  while(Serial.available()) {
      a = Serial.readString();// read the incoming data as string
  }
  a.trim(); // REMOVE WHITESPACES FROM STRING 
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

TimedAction t_ReceiveSerial = TimedAction(200,receiveSerial);
TimedAction t_LedCtrl = TimedAction(500,ledCtrl);

void loop() {
  t_ReceiveSerial.check();
  t_LedCtrl.check();
}
