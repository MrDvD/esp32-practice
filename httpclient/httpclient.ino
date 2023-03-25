#include <WiFi.h>
#include <HTTPClient.h>
#include <vector>

// VARIABLES
const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";
const char *ip = "192.168.180.242";
String port = "7005";

std::vector<int> IN_PIN = {16, 17, 14};
std::vector<int> OUT_PIN = {25, 26, 27};

void setup() {
   Serial.begin(115200);
   
   // PINS
   for (int i = 0; i < IN_PIN.size(); ++i) {
      pinMode(IN_PIN[i], INPUT_PULLDOWN);
   }
   for (int i = 0; i < OUT_PIN.size(); ++i) {
      pinMode(OUT_PIN[i], OUTPUT);
   }

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
}

std::vector<int> PREV_STATE;

void handle_buttons(std::vector<int> PINS, std::function<void(std::vector<int>)> func) {
   std::vector<int> CURR_STATE;
   for (int i = 0; i < PINS.size(); ++i) {
      CURR_STATE.push_back(digitalRead(PINS[i]));
   }
   // for (int i: CURR_STATE) {
   //    Serial.print(i);
   // }
   // Serial.println("[CURR_STATE]");
   // for (int i: PREV_STATE) {
   //    Serial.print(i);
   // }
   // Serial.println("[PREV_STATE]");
   if (PREV_STATE != CURR_STATE) {
      func(CURR_STATE);
      PREV_STATE = CURR_STATE;
   }
}

void send_state(std::vector<int> CURR_STATE) {
   HTTPClient server;
   String url = "http://" + String(ip) + ":" + port + "/buttons_update";
   server.begin(url);
   String bin_state = format_state(CURR_STATE);
   server.addHeader("State", bin_state);
   server.POST(".");
   
   String response = server.getString();
   server.end();

   set_led(response);
}

String format_state(std::vector<int> CURR_STATE) {
   String bin_state;
   for (int i = 0; i < CURR_STATE.size(); ++i) {
      bin_state += String(std::to_string(CURR_STATE[i]).c_str());
   }
   return bin_state;
}

void set_led(String response) {
   for (int i = 0; i < response.length(); ++i) { 
      int state = response[i] - '0';
      digitalWrite(OUT_PIN[i], state);
   }
}

void loop() {
   handle_buttons(IN_PIN, send_state);
}