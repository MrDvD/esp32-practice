#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

// VARIABLES
const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";

String phrase = "POST request was successful";

AsyncWebServer server(80);

// EVENT HANDLERS
void send_webpage(AsyncWebServerRequest *request) {
   request->send(LittleFS, "/webpage.html", "text/html");
   Serial.println("[GET_WEBPAGE]");
}

void send_script(AsyncWebServerRequest *request) {
   request->send(LittleFS, "/script.js", "text/javascript");
   Serial.println("[GET_SCRIPT]");
}

void post_pwm(AsyncWebServerRequest *request) {
   String pwm_value = request->getParam("range_pwm", true)->value();
   std::string value = pwm_value.c_str();
   ledcWrite(0, std::stoi(value));
   Serial.printf("[POST_VALUE]: %s\n", value.c_str());
   request->send(200, "text/plain", phrase);
}

void setup() {
   Serial.begin(115200);
   
   // PINS
   ledcSetup(0, 1024, 10);
   ledcAttachPin(26, 0);

   // WIFI
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pass);
   if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("[WIFI_FAIL]");
      ESP.restart();
   } else {
      Serial.println("[WIFI_SUCCESS]");
      Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
   }
   
   if (!LittleFS.begin()) {
      Serial.println("[LITTLEFS_FAIL]");
      ESP.restart();
   } else {
      Serial.println("[LITTLEFS_SUCCESS]");
   }
   
   // HTTP SERVER
   server.on("/", HTTP_GET, send_webpage);
   server.on("/script.js", HTTP_GET, send_script);
   server.on("/led_pwm_ajax", HTTP_POST, post_pwm);
   
   server.begin();
}

void loop() {
}