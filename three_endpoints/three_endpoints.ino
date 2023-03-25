#include <WiFi.h>
#include <WiFiClient.h>
#include <iostream>

// VARIABLES
const char ssid[] = "Redmi Note 10S";
const char pass[] = "11111111";
const char *ip = "192.168.180.242";

int button1;
int button2;

String post_but1 = "POST /buttons HTTP/1.1\r\nbutton1=";
String post_but2 = "&buttons2=";
String post_but3 = "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n";

String to_str(int val) {
   return String(std::to_string(val).c_str());
}

unsigned char hex[] = {LOW, HIGH};

void setup() {
   Serial.begin(115200);
   
   // PINS
   pinMode(16, INPUT_PULLDOWN);
   pinMode(17, INPUT_PULLDOWN);
   pinMode(26, OUTPUT);
   pinMode(25, OUTPUT);
   

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

void loop() {
   button1 = digitalRead(16);
   button2 = digitalRead(17);
   
   WiFiClient server;
   
   if (server.connect(ip, 7055)) {
      Serial.println("[SERVER CONNECTION]");
   } else {
      Serial.println("[CONNECTION FAILED]");
      ESP.restart();
   }
   
   String query = post_but1 + to_str(button1) + post_but2 + to_str(button2) + post_but3;
   Serial.println(query);
   server.print(query.c_str());
   
   if (!server) {
      return;
   }
   
   String value = server.readStringUntil('\n');
   
   const char *val = value.c_str();
   Serial.printf("LED1: %c, LED2: %c\n", val[0], val[1]);
   char c = val[0];
   char d = val[1];
   digitalWrite(25, hex[(int)c]);
   digitalWrite(26, hex[(int)d]);
   
   server.stop();
   
   // http.responseBody()
   delay(250);
}
