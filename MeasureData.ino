#include "DHT12.h"
DHT12 dht12;

void MeasureData() {
  Serial.printf("i:%d, j:%d\n", i, j);

  const float temperature = dht12.readTemperature();
  temperatureData[i][j] = (int)(temperature * 10);
  Serial.printf("Temperature: %.1f°C\n", temperature);

  humidityData[i][j] = (int)dht12.readHumidity();
  Serial.printf("Humidity: %d%%\n", humidityData[i][j]);

  soilHumidityData[i][j] = (int)50.0; // 土壌湿度
  Serial.printf("SoilHumidity: %d%%\n", soilHumidityData[i][j]);

  insolationData[i][j] = (int)500.0; // 日射量
  Serial.printf("Insolation: %dW/m²\n\n", insolationData[i][j]);
}
