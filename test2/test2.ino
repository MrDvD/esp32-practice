#include <WiFi.h>
#include <iostream>

const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";

WiFiServer server(80);

String httpResponseHeader =
"HTTP/1.1 200 OK\r\n" \
"Content-Type: text/html\r\n" \
"Connection: close\r\n" \
"\r\n";

const char *webPage = R"""(
<html>
  <head>
    <title>ESP32 Web Page</title>
  </head>
  <body>
    <p>Write LED State</p>
    <a href="/on"><button>ON</button></a>
    <a href="/off"><button>OFF</button></a>
  </body>
</html> 
)""";

void setup() {
  Serial.begin(115200);

  // PINS
  pinMode(26, OUTPUT);

  // WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi connect failed! Rebooting ...\n");
  }
  Serial.printf("Connected to Network\n");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

  // TCP SERVER
  server.begin();
  Serial.print("TCP server started\n");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Получили клиента");
    boolean cLIB = true;
    int n = 0;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && cLIB) {
          client.print(httpResponseHeader);
          client.print(webPage);
          Serial.printf("[HTTP RESP SENT]\n");
          break;
        }
        if (c == '\n') {
          cLIB = true;
        }
        else if (c != '\r') {
          cLIB = false;
          if (c == '/' and n == 0) {
            c = client.read();
            Serial.write(c);
            c = client.read();
            Serial.write(c);
            if (c == 'n') {
              digitalWrite(26, HIGH);
            }
            else if (c == 'f') {
              digitalWrite(26, LOW);
            }
            n += 1;
          }
        }
      }
    }
    
    client.stop();
    Serial.println("Disconnected client\n");
  }
}
