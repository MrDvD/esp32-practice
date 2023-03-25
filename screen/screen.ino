#include <WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#include <regex>

const char ssdi[] = "Redmi Note 10S";
const char pass[] = "11111111";
const char *ip = "192.168.180.242";
const char *port = "7020";

String POST(String url, String content_type, String payload) {
   return "POST " + url + " HTTP/1.1\r\n" + "Content-Type: " + content_type + "\r\n\r\n" + payload;
}

String GET(String url) {
   return "GET " + url + " HTTP/1.1\r\n";
}

LiquidCrystal lcd(14, 27, 16, 17, 25, 26);
DHT dht(19, DHT11);

void setup() {
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
std::regex pattern("(.+)&(.+)");
std::regex parse_time("\\d\\d:\\d\\d");

void loop() {
   float curr_temp = dht.readTemperature();
   int curr_time = millis() / 1000 / 60;
   
   if (curr_time - prev_time >= 1) {
      prev_time = curr_time;
      
      WiFiClient client;
      
      String header = GET("/time");
      
      client.connect(ip, std::stoi(port));
      client.print(header);
      String response = client.readString();
      client.stop();
      
      if (std::regex_search(response.c_str(), output, parse_time)) {
         lcd.setCursor(11, 1);
         lcd.print(output[0].str().c_str());
      }
   }

   if (curr_temp != prev_temp) {
      prev_temp = curr_temp;
      
      WiFiClient client;

      String temp = String(std::to_string(curr_temp).c_str());
      String header = POST("/update", "text/plain", "temp=" + temp);

      client.connect(ip, std::stoi(port));
      client.print(header);
      String response = client.readString();
      client.stop();
      
      if (std::regex_search(response.c_str(), output, pattern)){
         lcd.setCursor(5, 0); // temperature
         lcd.print(output[1].str().c_str());
         lcd.setCursor(5, 1); // timeout
         lcd.print(output[2].str().c_str());
      }
   }
}