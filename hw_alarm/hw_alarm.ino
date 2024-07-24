#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include "secrets.h"

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

/* MQTT broker config */
#define MQTT_BROKER                       "192.168.1.106"
#define MQTT_BROKER_PORT                  1883
#define MQTT_TOPIC                        "/alarm"

/* pin config */
#define PIN_BUZZER                        26
#define BUZZER_ACTIVE                     LOW
#define BUZZER_INACTIVE                   HIGH

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Arduino fire sensor node");

  pinMode(PIN_BUZZER, OUTPUT); digitalWrite(PIN_BUZZER, BUZZER_INACTIVE);

  Serial.print("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD /* defined in secrets.h */);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(5000);
  }
  Serial.println("done.");

  Serial.print("Connecting to MQTT broker...");
  if(!mqttClient.connect(MQTT_BROKER, MQTT_BROKER_PORT)) {
    Serial.print("failed - "); Serial.println(mqttClient.connectError());
    while(1);
  } else Serial.println("done.");

  mqttClient.onMessage(onMessage);
  mqttClient.subscribe(MQTT_TOPIC);
  Serial.println("Subscribed to MQTT topic.");
}

void onMessage(int size) {
  char c; while(mqttClient.available()) c = mqttClient.read();
  Serial.println(c);
  digitalWrite(PIN_BUZZER, (c == '1') ? BUZZER_ACTIVE : BUZZER_INACTIVE);
}

void loop() {
  // put your main code here, to run repeatedly:
  mqttClient.poll();
}
