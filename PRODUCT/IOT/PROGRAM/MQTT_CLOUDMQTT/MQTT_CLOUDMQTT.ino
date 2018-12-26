#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid         = "INFINET";
const char* password     = "thxtoalloh";
const char* mqttServer   = "m15.cloudmqtt.com";
const int mqttPort       = 15944;
const char* mqttUser     = "hhrblzkj";
const char* mqttPassword = "5fDMVfZA3RM3";
const char* send_recv_topic = "data-6e5c4b13-357f-4a56-abb9-a22a23f2e505";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
 
  client.publish(send_recv_topic, "device-6e5c4b13-357f-4a56-abb9-a22a23f2e505 OK");
  client.subscribe(send_recv_topic);
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();
}
