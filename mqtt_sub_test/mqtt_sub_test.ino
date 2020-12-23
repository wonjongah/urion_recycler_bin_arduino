#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

// Connect to the WiFi
const char* ssid = "SK_WiFiGIGA9A48";
const char* password = "1903003177";
const char* mqtt_server = "192.168.35.148";

WiFiClient espClient;
PubSubClient client(espClient);

const byte ledPin = 13; // Pin with LED on Adafruit Huzzah

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '0')
  digitalWrite(ledPin, HIGH);
  if (receivedChar == '1')
  digitalWrite(ledPin, LOW);
  }
  Serial.println();
}


void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("wonjongah96")) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("mqtt_test");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}

void setup()
{
 Serial.begin(115200);

 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);

 pinMode(ledPin, OUTPUT);
}

void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}
