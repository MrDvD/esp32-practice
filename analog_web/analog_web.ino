#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <vector>

DHT dht(14, DHT11);
const char ssdi[] = "Redmi Note 10S";
const char pass[] = "11111111";
const char *ip = "192.168.180.242";
std::vector<int> LEDS = { 26, 25, 17, 16, 27, 13, 5, 23, 19, 18 };

void setup() {
   dht.begin();
   Serial.begin(115200);
   for (int i = 0; i < LEDS.size(); ++i) {
      pinMode(LEDS[i], OUTPUT);
   }
   
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssdi, pass);
   if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("[WIFI_FAIL]");
      ESP.restart();
   } else {
      Serial.println("[WIFI_SUCCESS]");
      Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
   }
}

int led = 5;
float tempo;

void loop() {
   float temp = dht.readTemperature();
   if (temp != tempo) {
      HTTPClient server;
      String url = "http://" + String(ip) + ":7000/update";
      server.begin(url);
      String state = String(std::to_string(temp).c_str());
      server.addHeader("State", state);
      int code = server.POST("");
      if (code > 0) {
         tempo = temp;
         String resp = server.getString();
         led = std::stoi(resp.c_str());
         Serial.printf("LED VAL: %d\n", led);
         for (int j = 0; j < LEDS.size(); ++j) {
            digitalWrite(LEDS[j], LOW);
         }
         for (int i = led; i >= 1; --i) {
            digitalWrite(LEDS[i - 1], HIGH);
         }
      }
      server.end();
   }
}