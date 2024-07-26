#include <WiFi.h>
#include <HTTPClient.h>
#include "MQ135.h"

#define SENSOR_PIN 32
#define BUZZER_PIN1 13
#define BUZZER_PIN2 26

const char* ssid = "Redmii_i";
const char* password = "Stacker30";
const char* serverName = "http://192.168.42.184:5000/data"; 

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN1, OUTPUT);
  pinMode(BUZZER_PIN2, OUTPUT);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int sensorValue = analogRead(SENSOR_PIN);
  float voltage = sensorValue * (3.3 / 4095.0);
  float rLoad = 10.0;
  float rZero = 76.63;
  float rSensor = rLoad * ((3.3 / voltage) - 1.0);
  float ppm = pow((rSensor / rZero), -2.65);

  if (ppm > 10) { 
    digitalWrite(BUZZER_PIN1, HIGH);
    digitalWrite(BUZZER_PIN2, HIGH);
    delay(0);

    digitalWrite(BUZZER_PIN1, LOW);
    digitalWrite(BUZZER_PIN2, LOW);
    delay(50);
  } else {
    digitalWrite(BUZZER_PIN1, LOW);
    digitalWrite(BUZZER_PIN2, LOW);
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"ppm\": " + String(ppm) + "}";
    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(2500); 
}