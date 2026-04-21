//Code for soil moisture sensor that takes input from DHT22
// and soil moisture sensor and sends it to ThingSpeak using HTTP GET request

#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";

String apiKey = "K4BDLIZ56VNO9X5E";

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int soilPin = 34;

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    int soilValue = analogRead(soilPin);

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("DHT22 error!");
      return;
    }

    Serial.print("Temperature: ");
    Serial.println(temperature);

    Serial.print("Humidity: ");
    Serial.println(humidity);

    Serial.print("Soil Moisture: ");
    Serial.println(soilValue);

    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(temperature) +
                 "&field2=" + String(humidity) +
                 "&field3=" + String(soilValue);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.println("Data sent to ThingSpeak");
    } else {
      Serial.println("Error sending data");
    }

    http.end();
  }

  delay(15000);
}
