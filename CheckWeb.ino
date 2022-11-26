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
        String request = client.readString();
        Serial.println(request);

        if (request.indexOf("GET / ") >= 0) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.print(html);

        } else if (request.indexOf("GET /download") >= 0) {
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
}
