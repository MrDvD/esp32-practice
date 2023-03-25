#include <WiFi.h>
#include <WiFiClient.h>
#include<regex>
#include <ArduinoJson.h>

#define port "7000"
#define ip "192.168.180.242"

const char ssid[] = "Redmi Note 10S";
const char pass[]= "11111111";

DynamicJsonDocument rec(64);
DynamicJsonDocument upload(64);

int an= 0;
int an1=0;
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
std::regex pattern("\r\n\r\n(.+)");
std::cmatch output;
int led=0;

String up;
String response;
// HANDLERS
TaskHandle_t photores;
TaskHandle_t ik;

// FUNCTIONS
void wifi_send(void *unused) {
   Serial.begin(115200);
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid,pass);
   if (WiFi.waitForConnectResult() != WL_CONNECTED){
    Serial.printf("WiFi connect failed! Rebooting ...\n");
  }
  Serial.printf("Connected to Network\n");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
  for(;;){
     if (an != an1){
        upload["analog"] = analogRead(34);
        serializeJson( upload, up);
         WiFiClient server;
         String header = httpQuery("POST", "/time", "application/json", up.c_str());
         server.connect(ip, std::stoi(port));
         server.print (header);
         String response = server.readString();
         server.stop();  
         if (std::regex_search(response.c_str(), output, pattern)){
         const std::string out = output[1];
         deserializeJson(rec, out);
         led = std::stoi(rec["led"]);
         an=an1;
               
        
     }}
  }
   
}

void wifi_get(void *unused) {
   Serial.begin(115200);
   analogReadResolution(10);
      ledcSetup(0,1024,10);
      ledcAttachPin(34,0);
   for(;;){
      an=analogRead(34);

      ledcWrite(0,led);
   
   }
   
   while (true) {
      Serial.printf("IK: %d\n", analogRead(36));
      delay(500);
   }
   
}

void setup() {
   // TASKS
   xTaskCreatePinnedToCore(wifi_send,             // function
                           "photoResistor",        // name
                           5000,                   // size in words
                           NULL,                   // params
                           1,                      // priority
                           &photores,              // handler
                           0);                     // core
   xTaskCreatePinnedToCore(wifi_get,                // function
                           "ikResistor",           // name
                           5000,                   // size in words
                           NULL,                   // params
                           1,                      // priority
                           &ik,                    // handler
                           1);                     // core
}

void loop() {
}