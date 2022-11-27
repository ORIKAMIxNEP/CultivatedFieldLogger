#include "DHT12.h"
DHT12 dht12;

void MeasureTHS() {
  float temperature = (int)(dht12.readTemperature() * 10) / 10.0;
  uint32_t formattedTemperature = (int)((temperature + 50) * 10) << 7;
  Serial.printf("i: %d, j: %d\n", i, j);
  Serial.printf("Temperature: %.1f°C\n", temperature);
  Serial.printf("FormattedTemperature: %d\n", formattedTemperature);

  uint8_t humidity = (int)dht12.readHumidity();
  Serial.printf("Humidity: %d%%\n", humidity);

  uint32_t formattedData = formattedTemperature + humidity;
  THSData[i][j] = formattedData;
  Serial.printf("THSData: %d\n\n", formattedData);
}
