#include <WiFi.h>
#include <iostream>
using namespace std;

const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";

void setup() {
  Serial.begin(115200);

  // PINS
  adcAttachPin(36);
  analogReadResolution(12);

  // WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi connect failed! Rebooting ...\n");
  }
  Serial.printf("Connected to Network\n");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
}

const char *ip = "192.168.180.242";

void loop() {
  WiFiClient client;

  string value;
  
  int voice = analogRead(36);
  delay(20);
  //Serial.println(voice);
  
  boolean cLIB = true;
  if (client.connect(ip, 9010)) {
    Serial.println("[SERVER CONNECTION]");
  } else {
    Serial.println("[CONNECTION FAILED]");
    return;
  }
  if (voice >= 1000) {
    value.clear();
  } else if (voice >= 100) {
    value += "0";
  } else if (voice >= 10) {
    value += "00";
  } else if (voice){
    value += "000";
  }
  value += to_string(voice);
  client.print(value.c_str());
  
  client.stop();
  Serial.println("[Disconnected]");
}
