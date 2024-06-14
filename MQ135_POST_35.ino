#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define MQ135PIN 36  

// WiFi credentials
const char* ssid = "Redmi Note 10S";
const char* password = "daffa1258";

// URL server Flask
const String url = "http://192.168.39.108:5000/sensors";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  Serial.println("HTTP Started");
}

void loop() {
  // Membaca nilai dari sensor MQ-135
  int mq135Value = analogRead(MQ135PIN);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    // Buat JSON data
    StaticJsonDocument<200> doc;
    doc["mq135"] = mq135Value;
    doc["time"] = millis();
    String requestBody;
    serializeJson(doc, requestBody);

    // Tampilkan JSON data yang akan dikirim
    Serial.print("Sending data: ");
    Serial.println(requestBody);

    // Lakukan HTTP POST request
    int httpResponseCode = http.POST(requestBody);

    // Tampilkan respons dari server
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    // Akhiri koneksi HTTP
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(2000); // Delay 2 detik sebelum mengirim data lagi
}
