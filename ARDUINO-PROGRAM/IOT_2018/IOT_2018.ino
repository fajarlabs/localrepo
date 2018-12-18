/* ========================================================================
 * INCLUDE LIBRARY
 * LIBRARY FOR CODE
 * ========================================================================
 */
// library access EEPROM arduino
#include <EEPROM.h>
// library json
#include <ArduinoJson.h>
// library current sensor
#include "EmonLib.h"
#include <ctype.h>
#include <DHT.h>

/* ========================================================================
 * DIVISION VARIABLE  
 * INITIALIZE VARIABLE
 * ========================================================================
 */

// DHT22 pin configuration
#define DHTPIN 9     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

// define all relay switch
#define SW_1 4
#define SW_2 5
#define SW_3 6
#define SW_4 7
#define SW_5 10
#define SW_6 11
#define SW_7 12
#define SW_8 13

// define Current Sensor
#define CURRENT_SENSOR A0
#define VOLTAGE_SENSOR 220.0

// struct data current
typedef struct {
  String switch_relay;
  String current_sensor;
  String humidity_temperature;
} EEPROMData;

// definisi energy monitoring current sensor 100A
EnergyMonitor emon1;
// definisi untuk current data untuk disimpan
EEPROMData currentData;

// DHT22 sensor
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino


/* ========================================================================
 * GLOBAL FUNCTION
 * CODE FOR UTILITY
 * ========================================================================
 */

char * strToCharArray  (String str) 
{
  long size_char = (long) str.length();
  char result[size_char+1];
  str.toCharArray(result,size_char+1);
  return strdup((char*)result);
}

char *strToLower(char *str)
{
    unsigned char *mystr = (unsigned char *)str;
    while (*mystr) {
        *mystr = tolower(*mystr);
        mystr++;
    }
    return str;
}

void command_explode(String &command)
{
    // switch,current,temperature-humidity
    // setEEPROMData("0.0.0.1.1.1.0.1","23.15","24.25");
    const char* action = strToLower(strtok(strToCharArray(command), ":"));
    const char* value = strtok(NULL, ":");

    const char* _SW_ = "sw"; // Switch command
    const char* _HT_ = "ht"; // Humidity sensor
    const char* _CS_ = "cs"; // Current sensor

    if(strcmp(action,_SW_) == 0) 
    {
      currentData.switch_relay = value;
      set_switch(value);
      Serial.println(value);
    }

    if(strcmp(action,_HT_) == 0) 
    {
      String curr_dht22 = get_data_dht22();
      currentData.humidity_temperature = curr_dht22;
      Serial.println(curr_dht22);
    }

    if(strcmp(action,_CS_) == 0) 
    {
      String r_curr = read_current();
      currentData.current_sensor = r_curr;
      Serial.println(r_curr);
    }
}

/* ========================================================================
 * TEMPERATURE & HUMIDITY 
 * CODE FOR GET DATA FROM SENSOR DHT22
 * ========================================================================
 */

 String get_data_dht22()
 {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); 
    return String(humidity)+","+String(temperature);
 }

/* ========================================================================
 * SWITCH SENSOR DIVISION 
 * CODE FOR OUTPUT
 * ========================================================================
 */

void set_switch(char* c_switch_relay) 
{ 
  // convert string switch to token
  int _sw1 = atoi(strtok(c_switch_relay, "."));
  int _sw2 = atoi(strtok(NULL, "."));
  int _sw3 = atoi(strtok(NULL, "."));
  int _sw4 = atoi(strtok(NULL, "."));
  int _sw5 = atoi(strtok(NULL, "."));
  int _sw6 = atoi(strtok(NULL, "."));
  int _sw7 = atoi(strtok(NULL, "."));
  int _sw8 = atoi(strtok(NULL, "."));

  // set write digital from data token
  if(_sw1 == 1) digitalWrite(SW_1,HIGH); else digitalWrite(SW_1,LOW);
  if(_sw2 == 1) digitalWrite(SW_2,HIGH); else digitalWrite(SW_2,LOW);
  if(_sw3 == 1) digitalWrite(SW_3,HIGH); else digitalWrite(SW_3,LOW);
  if(_sw4 == 1) digitalWrite(SW_4,HIGH); else digitalWrite(SW_4,LOW);
  if(_sw5 == 1) digitalWrite(SW_5,HIGH); else digitalWrite(SW_5,LOW);
  if(_sw6 == 1) digitalWrite(SW_6,HIGH); else digitalWrite(SW_6,LOW);
  if(_sw7 == 1) digitalWrite(SW_7,HIGH); else digitalWrite(SW_7,LOW);
  if(_sw8 == 1) digitalWrite(SW_8,HIGH); else digitalWrite(SW_8,LOW);
}

void set_pin()
{
  pinMode(SW_1,OUTPUT);
  pinMode(SW_2,OUTPUT);
  pinMode(SW_3,OUTPUT);
  pinMode(SW_4,OUTPUT);
  pinMode(SW_5,OUTPUT);
  pinMode(SW_6,OUTPUT);
  pinMode(SW_7,OUTPUT);
  pinMode(SW_8,OUTPUT);
}

/* ========================================================================
 * EEPROM DIVISION  
 * CODE FOR EEPROM CONFIGURATION
 * ========================================================================
 */

/**
 * Fungsi untuk membuat format json yang akan disimpan kedalam EEPROM
 */
String _GenerateJsonData(String switch_relay, String current_sensor, String humidity_sensor) 
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  
  root["SWITCH_RELAY"] = switch_relay;
  root["CURRENT_SENSOR"] = current_sensor;
  root["HUMIDITY_TEMPERATURE"] = humidity_sensor;
  
  String output;
  root.printTo(output);
  return output;
}

/**
 * Fungsi untuk init panjang memory EEPROM
 */
void initEEPROMData() 
{
    // size 1024 Kb memory
    // if using wemos use its
    // EEPROM.begin(1024);
}

/**
 * Overload
 * Fungsi untuk generate string json ke json object
 */
EEPROMData _ConvertStrJsonToObject(String strJson) 
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(strJson);
  
  EEPROMData eedata;
  eedata.switch_relay = (char*) root["SWITCH_RELAY"];
  eedata.current_sensor = (char*) root["CURRENT_SENSOR"];
  eedata.humidity_temperature = (char*) root["HUMIDITY_TEMPERATURE"];
  
  return eedata;
}


/**
 * Fungsi untuk menge-set EEPROM dengan inputan yang sudah ditentukan
 */
void setEEPROMData(String switch_relay, String current_sensor, String humidity_sensor)
{ 
  String str = _GenerateJsonData(switch_relay,current_sensor,humidity_sensor);
  if(str.length() < EEPROM.length()) 
  {
    for(int i = 0; i < str.length(); i++) {
      char item = str.charAt(i);
      EEPROM.write(i,item);
    }
  }

  // if using wemos use it
  // EEPROM.commit();
}

/**
 * Fungsi untuk mengambil data dari EEPROM dengan output STRUCT
 */
EEPROMData getEEPROMData()
{  
  String strJson = "";
  for(int i = 0; i < EEPROM.length(); i++) {
    uint8_t e = EEPROM.read(i);
    if(e < 1) break;
    strJson += (char)e;
  }

  EEPROMData result = _ConvertStrJsonToObject(strJson);
  return result;
}

/**
 * Fungsi untuk mereset EEPROM agar kembali 0
 */
void resetEEPROMData() 
{
  for(int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i,0);
  }
}

/* ========================================================================
 * CURRENT SENSOR DIVISION  
 * CODE FOR SENSOR INPUT & SEND TO SERIAL
 * ========================================================================
 */
void setup_current()
{
  //calib - Cur Const= Ratio/BurdenR. 2000/33 = 60
  emon1.current(CURRENT_SENSOR, 60);
}

String read_current()
{
  double Irms = emon1.calcIrms(1480);

  String curr = String(Irms);
  String watt = String(Irms*VOLTAGE_SENSOR);

  return curr +","+watt;
}

/* ========================================================================
 * SETUP DIVISION  
 * CODE FOR CONFIGURATION
 * ========================================================================
 */

void setup() 
{
  Serial.begin(9600);
  // Set pin to output
  set_pin();
  
  // Get data from EEPROM arduino
  EEPROMData data_eeprom = getEEPROMData();
  
  // Save to current data
  currentData.switch_relay = data_eeprom.switch_relay;
  currentData.humidity_temperature = data_eeprom.humidity_temperature;
  currentData.current_sensor = data_eeprom.current_sensor;
  
  // setup current configuration
  setup_current();
}

/* ========================================================================
 * LOOP DIVISION  
 * START APPLICATION
 * ========================================================================
 */
void loop() 
{
  if(Serial.available())
  {
    // get request from serial
    String request = Serial.readString();
    // clean from whitespace required
    request.trim();
    // process data input from serial
    command_explode(request);
  }
}
