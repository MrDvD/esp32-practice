#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>

const char ssdi[] = "Redmi Note 10S";
const char pass[] = "11111111";
const char *ip = "192.168.180.242";
const char *port = "7074";

void setup() {
   Serial.begin(115200);
   
   // WIFI
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

void loop() {
   WiFiClient clients;
   
   WiFiClient client;
   HTTPClient server;
   String url = "http://" + String(ip) + ":" + String(port) + "/update";
   server.begin(client, url);
   server.setTimeout(10000);
   server.addHeader("Content-Type", "text/plain");
   String payload = "temp=76348562378";
   int code = server.POST(payload);
   client.connect(ip, std::atoi(port) + 1);
   client.print(payload);
   client.stop();
   Serial.println(code);
   // Serial.println(server.getString().c_str());
   if (code > 0) {
      String response = server.getString();
      Serial.println(response);
   }
   server.end();
}