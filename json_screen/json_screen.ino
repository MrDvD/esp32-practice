#include <WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <regex>

const char ssdi[] = "Redmi Note 10S";
const char pass[] = "11111111";
const char *ip = "192.168.180.242";
const char *port = "7000";

String httpQuery(std::string type,
                 std::string url,
                 std::string content_type=" ",
                 std::string payload=" ") {
   std::string header = type + " " + url + " HTTP/1.1\r\n";
   if (type == "GET") {
      header += "Accept: " + content_type + "\r\n\r\n";
   } else {
      header += "Content-Type: " + content_type + "\r\n";
      header += "Content-Length: " + std::to_string(payload.length()) + "\r\n\r\n";
      header += payload;
   }
   return header.c_str();
}

LiquidCrystal lcd(14, 27, 16, 17, 25, 26);
DHT dht(19, DHT11);

void setup() {text/plain
   dht.begin();
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
   
   // LCD
   lcd.begin(16, 2);
   
   lcd.setCursor(0, 0);
   lcd.print("Temp:");
   lcd.setCursor(12, 0);
   lcd.print("Time");
   lcd.setCursor(0, 1);
   lcd.print("Refr:");
}

float prev_temp = 0;
int prev_time = -1;
std::cmatch output;
std::regex pattern("\r\n\r\n(.+)");
std::regex parse_time("\\d\\d:\\d\\d");

DynamicJsonDocument time_doc(64);
DynamicJsonDocument response_doc(64);

void loop() {
   float curr_temp = dht.readTemperature();
   int curr_time = millis() / 1000 / 60;
   
   if (curr_time - prev_time >= 1) {
      prev_time = curr_time;
      
      WiFiClient server;
      
      String header = httpQuery("GET", "/time", "text/plain");
      
      server.connect(ip, std::stoi(port));
      server.print(header);
      String response = server.readString();
      server.stop();
      
      if (std::regex_search(response.c_str(), output, parse_time)) {
         lcd.setCursor(11, 1);
         lcd.print(output[0].str().c_str());
      }
   }

   if (curr_temp != prev_temp) {
      prev_temp = curr_temp;
      
      WiFiClient server;

      std::string temp = std::to_string(curr_temp);
      String header = httpQuery("POST", "/update", "text/plain", "temp=" + temp);

      server.connect(ip, std::stoi(port));
      server.print(header);
      String response = server.readString();
      server.stop();
      
      Serial.println(response);
      
      if (std::regex_search(response.c_str(), output, pattern)){
         const std::string out = output[1];
         deserializeJson(response_doc, out);
         lcd.setCursor(5, 0); // temperature
         String temp_pars = response_doc["temp"]; 
         lcd.print(temp_pars.c_str());
         lcd.setCursor(5, 1); // timeout
         String time_pars = response_doc["timeout"];
         lcd.print(time_pars.c_str());
      }
   }
}