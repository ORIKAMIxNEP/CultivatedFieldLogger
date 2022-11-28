void WriteFile() {
  File file = SPIFFS.open("/log.txt", "w");
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }
  for (uint8_t i = 0; i < sizeof(THSData) / sizeof(THSData[0]); i++) {
    for (uint8_t j = 0; j < sizeof(THSData[0]) / sizeof(THSData[0][0]); j++) {
      if (THSData[i][j] == -1) {
        Serial.printf("No more data: THSData[%d][%d]\n", i, j);
        break;
      }
      float temperature = (THSData[i][j] >> 7) / 10.0 - 50;
      uint8_t humidity = THSData[i][j] % 128;
      file.printf("%.1f,%d\n", temperature, humidity);
      THSData[i][j] = -1;
    }
    if (THSData[i][j] == -1) {
      break;
    }
  }
  file.close();
}
