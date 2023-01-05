void WriteFile() {
  File file = SPIFFS.open("/log.csv", "w");
  if (!file) {
    Serial.println("Failed to open file\n");
    return;
  }
  for (uint8_t i = 0; i < logDays; i++) {
    for (uint8_t j = 0; j < logRate; j++) {
      if (humidityData[i][j] == -1) {
        break;
      }
      const float temperature = temperatureData[i][j] / 10.0;
      file.printf("%.1f,%d,%d,%d\n", temperature, humidityData[i][j], soilHumidityData[i][j], insolationData[i][j]);
      humidityData[i][j] = -1;
    }
    if (humidityData[i][j] == -1) {
      break;
    }
  }
  file.close();
}
