#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <DHT.h>
#include "secrets.h"

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

/* MQTT broker config */
#define MQTT_BROKER                       "192.168.1.106"
#define MQTT_BROKER_PORT                  1883
#define MQTT_TOPIC                        "/sensors/arduino" // TODO: generate random IDs

/* pin config */
#define PIN_DHT                           2
#define PIN_TEST                          3

DHT dht(PIN_DHT, DHT22);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Arduino fire sensor node");

  pinMode(PIN_TEST, INPUT_PULLUP);
  dht.begin();

  Serial.print("Connecting to WiFi...");
  while(WiFi.begin(WIFI_SSID, WIFI_PASSWORD /* defined in secrets.h */) != WL_CONNECTED) {
    Serial.print('.');
    delay(5000);
  }
  Serial.println("done.");

  Serial.print("Connecting to MQTT broker...");
  if(!mqttClient.connect(MQTT_BROKER, MQTT_BROKER_PORT)) {
    Serial.print("failed - "); Serial.println(mqttClient.connectError());
    while(1);
  } else Serial.println("done.");
}

#define INTERVAL                          2000
uint32_t last_updated = 0;

#define TEST_TEMP                         55.00

void loop() {
  // put your main code here, to run repeatedly:
  mqttClient.poll();

  uint32_t current_timestamp = millis();
  if(current_timestamp - last_updated >= INTERVAL) {
    last_updated = current_timestamp;

    float temp = (!digitalRead(PIN_TEST)) ? TEST_TEMP : dht.readTemperature();
    Serial.print("Temperature: "); Serial.println(temp);

    mqttClient.beginMessage(MQTT_TOPIC);
    mqttClient.print("{\"temperature\":");
    mqttClient.print(temp);
    mqttClient.print('}');
    mqttClient.endMessage();
  }
}
