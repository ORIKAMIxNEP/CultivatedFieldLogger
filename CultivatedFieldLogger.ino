#include <WiFi.h>
#include "DHT12.h"
#include <Adafruit_BMP280.h>
DHT12 dht12;
Adafruit_BMP280 bmp280;

const char html[] =
  "<!DOCTYPE html>\
  <html lang='ja'>\
    <head>\
      <meta charset='utf-8'>\
      <title>Cultivated Field Logger</title>\
    </head>\
    <body>\
      <a href='log.txt'><button>CSVData Download</button></a>\
    </body>\
  </html>";

const char ssid[] = "ESP32-M5StickC";
const IPAddress ip(192, 168, 0, 1);
const IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);

void setup() {
  WiFi.softAP(ssid);
  delay(1000);
  WiFi.softAPConfig(ip, ip, subnet);
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
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client Connected");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readString();
        Serial.println(request);
        if (request.indexOf("GET / ") >= 0) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.print(html);

          float tmperature = dht12.readTemperature();
          Serial.printf("Temperature: %2.1f C\n", tmperature);

          float humidity = dht12.readHumidity();
          Serial.printf("Humidity: %2.0f %%\n", humidity);

        } else if (request.indexOf("GET /log.txt") >= 0) {
          Serial.printf("Download");
        } else {
          client.println("HTTP/1.1 404 Not Found");
          client.println("Connection:close");
        }
        break;
      }
    }
    client.stop();
    Serial.println("Client Disconnected");
  }
  delay(1000);
}
