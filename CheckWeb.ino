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

void CheckWeb() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client Connected");
    while (client.connected()) {
      if (client.available()) {
        const String request = client.readString();
        Serial.println(request);

        if (request.indexOf("GET / ") >= 0) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.print(html);

        } else if (request.indexOf("GET /download") >= 0) {
          FileDownload();
          delay(1000);
          file = SPIFFS.open("/log.txt", "r");
          while (file.available()) {
            Serial.println(file.readStringUntil('\n'));
          }
          if (!file) {
            Serial.println("Failed to open file");
          } else {
            delay(1000);
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Content-Disposition: attachment; filename=log.txt");
            //            client.println("Content-Length: " + (String)file.size());
            client.println("Connection: close");
            client.println("Access-Control-Allow-Origin: *");
            client.println();
            client.print(file);
            file.close();
            i = 0, j = 0;
          }

        } else {
          client.println("HTTP/1.1 404 Not Found");
          client.println("Connection:close");
        }
        break;
      }
    }
    client.stop();
    Serial.println("Client Disconnected\n");
  }
}
