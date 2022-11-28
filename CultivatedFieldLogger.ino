#include <Adafruit_BMP280.h>
#include <WebServer.h>
#include "SPIFFS.h"
Adafruit_BMP280 bmp280;

const char ssid[] = "ESP32-M5StickC";
const IPAddress ip(192, 168, 0, 1);
const IPAddress subnet(255, 255, 255, 0);

uint32_t THSData[7][144];
uint8_t i = 0, j = 0;
bool hasCapacity = true;
uint32_t count = 0;

WebServer server(80);

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid);
  Serial.println("SSID: " + (String)ssid);
  delay(100);
  WiFi.softAPConfig(ip, ip, subnet);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HandleIndexPage);
  server.on("/download", HandleDownload);
  server.onNotFound(HandleNotFound);
  server.begin();
  Serial.println("Server Start\n");

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  SPIFFS.format();

  Wire.begin(0, 26);
  if (!bmp280.begin(0x76)) {
    Serial.println("BMP280Sensor is not found");
    return;
  }

  for (uint8_t i = 0; i < sizeof(THSData) / sizeof(THSData[0]); i++) {
    for (uint8_t j = 0; j < sizeof(THSData[0]) / sizeof(THSData[0][0]); j++) {
      THSData[i][j] = -1;
    }
  }
}

void loop() {
  server.handleClient();
  delay(1);

  if (hasCapacity && count == 10000) { // 600000
    MeasureTHS();
    j++;
    count = 0;
    if (j == sizeof(THSData[0]) / sizeof(THSData[0][0])) {
      if (i == sizeof(THSData) / sizeof(THSData[0])) {
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
