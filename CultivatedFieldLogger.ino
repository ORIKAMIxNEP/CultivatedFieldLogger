#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include "FS.h"
#include "SPIFFS.h"
Adafruit_BMP280 bmp280;

const char ssid[] = "ESP32-M5StickC";
const IPAddress ip(192, 168, 0, 1);
const IPAddress subnet(255, 255, 255, 0);

File file;
uint32_t THSData[7][144];
uint16_t count = 0;
uint8_t i = 0, j = 0;
bool hasCapacity = true;

WiFiServer server(80);

void setup() {
  for (uint8_t i = 0; i < sizeof(THSData) / sizeof(THSData[0]); i++) {
    for (uint8_t j = 0; j < sizeof(THSData[0]) / sizeof(THSData[0][0]); j++) {
      THSData[i][j] = -1;
    }
  }

  WiFi.softAP(ssid);
  delay(1000);
  WiFi.softAPConfig(ip, ip, subnet);
  server.begin();
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  //SPIFFS.remove("/log.txt");
  SPIFFS.format();
  Wire.begin(0, 26);

  if (!bmp280.begin(0x76)) {
    Serial.println("BMP280Sensor is not found");
    while (1);
  }

  Serial.begin(115200);
  Serial.println("SSID: " + (String)ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Server Start\n");
}

void loop() {
  CheckWeb();
  if (hasCapacity && count == 10) { // 600
    MeasureTHS();
    j++;
    count = 0;
  }
  if (j == sizeof(THSData[0]) / sizeof(THSData[0][0])) {
    if (i == sizeof(THSData) / sizeof(THSData[0])) {
      hasCapacity = false;
      i = 0;
      Serial.println("No free space for variable");
    }
    i++;
    j = 0;
  }
  count++;
  delay(1000);
}
