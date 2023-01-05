#include <Adafruit_BMP280.h>
#include <WebServer.h>
#include "SPIFFS.h"

const uint8_t logDays = 1, logRate = 3;
int16_t temperatureData[logDays][logRate];
int8_t humidityData[logDays][logRate];
uint8_t soilHumidityData[logDays][logRate];
uint16_t insolationData[logDays][logRate];
uint8_t i = 0, j = 0;
bool hasCapacity = true;
uint32_t count = 0;

WebServer server(80);

void setup() {
  Serial.begin(9600);

  const char ssid[] = "ESP32-M5StickC";
  const char password[] = "JNu#N&0kpYZJDaDM";
  WiFi.softAP(ssid, password);
  Serial.println("SSID: " + (String)ssid);
  delay(100);
  const IPAddress ip(192, 168, 0, 1);
  const IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, ip, subnet);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HandleDownload);
  server.onNotFound(HandleNotFound);
  server.begin();
  Serial.println("Server Started\n");

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  SPIFFS.format();

  Wire.begin(0, 26);
  Adafruit_BMP280 bmp280;
  if (!bmp280.begin(0x76)) {
    Serial.println("BMP280Sensor is not found");
    return;
  }

  for (uint8_t i = 0; i < logDays; i++) {
    for (uint8_t j = 0; j < logRate; j++) {
      humidityData[i][j] = -1;
    }
  }
}

void loop() {
  server.handleClient();
  delay(1000);

  if (hasCapacity && count == 24 * 60 * 60 / logRate / 60) {
    MeasureData();
    j++;
    count = 0;
    if (j == logRate) {
      if (i == logDays) {
        hasCapacity = false;
        i = 0;
        Serial.println("No free space for variable");
      }
      i++;
      j = 0;
    }
  }
  count++;
}
