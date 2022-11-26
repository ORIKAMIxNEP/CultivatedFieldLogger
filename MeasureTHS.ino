#include "DHT12.h"
DHT12 dht12;

void MeasureTHS() {
  int tmperature = (int)(dht12.readTemperature()*10)/10;
  Serial.printf("Temperature: %d°C\n", tmperature);

  int humidity = (int)dht12.readHumidity();
  Serial.printf("Humidity: %d%%\n", humidity);
}
