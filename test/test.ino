#include <WiFi.h>
#include <iostream>

const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // PINS
  ledcSetup(0, 1000, 8);
  ledcAttachPin(26, 0);

  // WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi connect failed! Rebooting ...\n");
  }
  Serial.printf("Connected\n");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

  // TCP SERVER
  server.begin();
  Serial.print("TCP server started\n");
}

String value;

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Получили");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c != ' ') {
          value += c;
        } else {
          ledcWrite(0, std::stoi(value.c_str()));
          value.clear();
        }
      }
    }
    client.stop();
    Serial.println("Disconnected client\n");
  }
}
