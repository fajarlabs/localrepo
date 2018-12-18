#include <SoftwareSerial.h>

// ARDUINO UNO 
// -- PIN 9 -> 5VT
// -- PIN 10 -> 5VR

// WEMOS D9 
// -- PIN D1 -> 3VT
// -- PIN D2 -> 3VR

SoftwareSerial mySerial(D1, D2); // RX TX
char msg;
char call;
bool start = false;
bool start_begin = false;

void setup()
{
  Serial.begin(115200);    // Setting the baud rate of Serial Monitor (Arduino)
  mySerial.begin(115200);   // Setting the baud rate of GSM Module  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); 
}

void loop()
{  
  if (Serial.available()>0)
  {
    if(start == false) {
      if(Serial.read() == 'r') {
        Serial.println("GSM SIM900A BEGIN");
        Serial.println("Enter character for control option:");
        Serial.println("h : to disconnect a call");
        Serial.println("i : to receive a call");
        Serial.println("s : to send message");
        Serial.println("c : to make a call");  
        Serial.println("e : to redial");
        Serial.println();
        start = true;
        if(start_begin == false) {
          start_begin = true;
          digitalWrite(LED_BUILTIN, LOW); 
        }
      }
    }
  
    if(start == true) {
      switch(Serial.read())
      {
        case 's':
          SendMessage();
          break;
        case 'c':
          MakeCall();
          break;
        case 'h':
          HangupCall();
          break;
        case 'e':
          RedialCall();
          break;
        case 'i': 
          ReceiveCall();
          break;
      }
    }
  }

  if(start == true) 
  {
    if (mySerial.available()>0)
    {
      Serial.write(mySerial.read());
    }
  }
}

void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+6287882737107\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("sim900a sms");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void ReceiveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  if (mySerial.available()>0)
  {
    msg=mySerial.read();
    Serial.print(msg);
  }
}

void MakeCall()
{
  mySerial.println("ATD+6287882737107;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}


void HangupCall()
{
  mySerial.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void ReceiveCall()
{
  mySerial.println("ATA");
  delay(1000);
  {
    call=mySerial.read();
    Serial.print(call);
  }
}

void RedialCall()
{
  mySerial.println("ATDL");
  Serial.println("Redialing");
  delay(1000);
}
