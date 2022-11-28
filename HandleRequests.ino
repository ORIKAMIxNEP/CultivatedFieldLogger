#include "FS.h"

const char html[] =
  "<!DOCTYPE html>\
   <html lang='ja'>\
     <head>\
       <meta charset='utf-8'>\
       <title>Cultivated Field Logger</title>\
     </head>\
     <body>\
       <a href='/download'><button>CSVData Download</button></a>\
     </body>\
   </html>";

void HandleIndexPage() {
  server.send(200, "text/html", html);
    Serial.println("Display index page\n");
}

void HandleNotFound() {
  server.send(404, "text/plain", "Page Not Found");
  Serial.println("Display not found page\n");
}

void HandleDownload() {
  WriteFile();
  File file = SPIFFS.open("/log.txt", "r");
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }

  server.sendHeader("Content-Type", "text/text");
  server.sendHeader("Content-Disposition", "attachment; filename=log.txt");
  server.sendHeader("Connection", "close");
  server.streamFile(file, "application/octet-stream");
  file.close();
  Serial.println("Download completed\n");

  i = 0, j = 0;
  hasCapacity = true;
}
