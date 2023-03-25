#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";

WiFiMulti wifiMulti;

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(1000);
  }
  wifiMulti.addAP(ssid, pass);
}

void loop() {
  if((wifiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.println("[HTTP] begin...");
    http.begin("http://192.168.180.242:80");
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    }
    delay(5000);
}
