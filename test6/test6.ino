#include <WiFi.h>
#include <iostream>
#include <DHT.h>
using namespace std;

const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";

WiFiServer server(80);

String httpResponseHeader =
"HTTP/1.1 200 OK\r\n" \
"Content-Type: text/html\r\n" \
"Connection: close\r\n" \
"\r\n";

const char *webpage1 = R"""(
<html>
  <head>
    <title>ESP32 Web Page</title>
    <meta http-equiv='refresh' content='4'>
  </head>
  <body>
    <p>Current Temp: )""";

const char *webpage2 = R"""(
</p>
    <p>Current Hum: )""";

const char *webpage3 = R"""(
    </p>
  </body>
</html>
)""";

DHT dht(17, DHT11);

void setup() {
  Serial.begin(115200);

  // PINS

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
  
  dht.begin();
}

void loop() {
  WiFiClient client = server.available();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
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
          client.print(webpage1);
          client.print(temp);
          client.print(webpage2);
          client.print(hum);
          client.print(webpage3);
          Serial.printf("[HTTP RESP SENT]\n");
          break;
        }
        if (c == '\n') {
          cLIB = true;
        }
        else if (c != '\r') {
          cLIB = false;
        }
      }
    }
    
    client.stop();
    Serial.println("Disconnected client\n");
  }
}
