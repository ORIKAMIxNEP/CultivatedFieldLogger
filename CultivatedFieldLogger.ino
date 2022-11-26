#include <WiFi.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp280;

const char ssid[] = "ESP32-M5StickC";
const IPAddress ip(192, 168, 0, 1);
const IPAddress subnet(255, 255, 255, 0);

uint32_t THSdata[7][144];
unsigned char count = 0;

WiFiServer server(80);

void setup() {
  WiFi.softAP(ssid);
  delay(1000);
  WiFi.softAPConfig(ip, ip, subnet);
  server.on("/download", fileDownload);
  server.begin();
  Wire.begin(0, 26);

  if (!bmp280.begin(0x76)) {
    Serial.println("BMP280Sensor is not found");
    while (1);
  }

  Serial.begin(115200);
  Serial.println("SSID: " + (String)ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Server Start");
}

void loop() {
  CheckWeb();
  if (count == 10) {
    MeasureTHS();
    count = 0;
  }
  count++;
  delay(1000);
}
