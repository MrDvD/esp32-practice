#include <WiFi.h>
#include <iostream>
using namespace std;

const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";

WiFiServer server(80);

String httpResponseHeader =
"HTTP/1.1 200 OK\r\n" \
"Content-Type: text/html\r\n" \
"Connection: close\r\n" \
"\r\n";

int PINS[] = {25, 26};

const char *webPage = R"""(
<html>
  <head>
    <title>ESP32 Web Page</title>
  </head>
  <body>
    <p>Write LED State</p>
    <p>Led 1: <a href='/0N'>ON</a>/<a href='/0F'>OFF</a>; Led 2: <a href='/1N'>ON</a>/<a href='/1F'>OFF</a></p>
  </body>
</html> 
)""";

void setup() {
  Serial.begin(115200);

  // PINS
  pinMode(25, OUTPUT);
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
    String value;
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
          if (c == ' ') {
            c = client.read();
            Serial.write(c);
            if (c == '/') {
              char pin = client.read();
              Serial.write(pin);
              char cmd = client.read();
              Serial.write(cmd);
              if (cmd == 'N') {
                cmd = '1';
              } else {
                cmd = '0';
              }
              digitalWrite(PINS[static_cast<int>(pin) - '0'], static_cast<int>(cmd) - '0');
            }
          }
        }
      }
    }
    
    client.stop();
    Serial.println("Disconnected client\n");
  }
}
