#include <EEPROM.h>
#include <ArduinoJson.h>

/**
 * STRUCT untuk penyimpanan data EEPROM
 */
typedef struct {
  String deviceId;
  String currentData;
} EEPROMData;

/**
 * Fungsi untuk membuat format json yang akan disimpan kedalam EEPROM
 */
String _GenerateJsonData(String deviceId, String currentData) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  
  root["DEVICE_ID"] = deviceId;
  root["CURR_DATA"] = currentData;
  
  String output;
  root.printTo(output);
  return output;
}

/**
 * Overload
 * Fungsi untuk generate string json ke json object
 */
EEPROMData _ConvertStrJsonToObject(String strJson) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(strJson);
  
  EEPROMData eedata;
  eedata.deviceId = (char*) root["DEVICE_ID"];
  eedata.currentData = (char*) root["CURR_DATA"];
  
  return eedata;
}


/**
 * Fungsi untuk menge-set EEPROM dengan inputan yang sudah ditentukan
 */
void setEEPROMData(String deviceID, String currentData){ 
  String str = _GenerateJsonData(deviceID,currentData);
  if(str.length() < EEPROM.length()) {
    for(int i = 0; i < str.length(); i++) {
      char item = str.charAt(i);
      EEPROM.write(i,item);
    }
  }
}

/**
 * Fungsi untuk mengambil data dari EEPROM dengan output STRUCT
 */
EEPROMData getEEPROMData(){  
  String strJson = "";
//  for(int i = 0; i < EEPROM.length(); i++) {
//    uint8_t e = EEPROM.read(i);
//    if(e < 1) break;
//    strJson += (char)e;
//  }

  EEPROMData result = _ConvertStrJsonToObject(strJson);
  return result;
}

/**
 * Fungsi untuk mereset EEPROM agar kembali 0
 */
void resetEEPROMData() {
  for(int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i,0);
  }
}

void setup() {
  Serial.begin(9600);
  EEPROMData obj = getEEPROMData();
}

void loop() {}
