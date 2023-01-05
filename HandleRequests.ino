#include "FS.h"

void HandleDownload() {
  WriteFile();
  File file = SPIFFS.open("/log.csv", "r");
  if (!file) {
    Serial.println("Failed to open file\n");
    return;
  }

  server.sendHeader("Content-Type", "text/csv");
  server.sendHeader("Content-Disposition", "attachment; filename=log.csv");
  server.sendHeader("Connection", "close");
  server.streamFile(file, "application/octet-stream");
  file.close();
  Serial.println("Download completed\n");

  i = 0, j = 0;
  if (!hasCapacity) {
    count = 0;
    hasCapacity = true;
  }
}

void HandleNotFound() {
  server.send(404, "text/plain", "Page Not Found");
  Serial.println("Display not found page\n");
}
