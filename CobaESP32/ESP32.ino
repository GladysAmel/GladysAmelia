#include <WiFi.h>
#include <HTTPClient.h>
#include "MQ135.h"

#define SENSOR 34
const char* ssid = "Redmii_i";
const char* password = "Stacker30";
const char* serverName = "http://192.168.102.112:5000/data"; 

void setup()
{
   Serial.begin(115200);
   delay(2000);

   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
       delay(1000);
       Serial.println("Connecting to WiFi...");
   }
   Serial.println("Connected to WiFi");
}

void loop()
{
    MQ135 gasSensor = MQ135(SENSOR);
    float air_quality = gasSensor.getPPM();
    
    Serial.print("Air Quality: ");
    Serial.print(air_quality);
    Serial.println(" PPM");

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        http.begin(serverName);
        http.addHeader("Content-Type", "application/json");

        String jsonPayload = "{\"air_quality\": " + String(air_quality) + "}";

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

    Serial.println("Waiting...");
    delay(60000);
}