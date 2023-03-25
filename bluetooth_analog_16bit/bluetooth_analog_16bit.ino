#include <BluetoothSerial.h>
#include <ArduinoJson.h>

#define bpin 25
#define lat 16
#define clock 27
#define data 17
#define lat2 19
#define clock2 18
#define data2 23

BluetoothSerial espBT;

uint8_t address[6] = { 0x74, 0xC6, 0x3B, 0x38, 0xA4, 0xA0 };

void setup() {
   Serial.begin(115200);
   espBT.begin("ESP32test", true);

   analogReadResolution(10);
   if (!espBT.connect(address, 1)) {
      Serial.println("[BLUE_FAIL]");
      ESP.restart();
   } else {
      Serial.println("[BLUE_SUCCESS]");
   }

   // PINS
   pinMode(bpin, INPUT_PULLDOWN);
   pinMode(lat, OUTPUT);
   pinMode(data, OUTPUT);
   pinMode(clock, OUTPUT);
   pinMode(lat2, OUTPUT);
   pinMode(data2, OUTPUT);
   pinMode(clock2, OUTPUT);
   }

int an = 0;
int an1;
char payload[5];
int bold = 0;

DynamicJsonDocument response_doc(64);

void loop() {
   int but1_curr = digitalRead(bpin);
   payload[0] = '\0';

   if (but1_curr != bold) {
      if (but1_curr) {
         an1 = analogRead(34);
         Serial.println(an1);
         if (an != an1) {
         std::string klik = std::to_string(an1);
         strncat(payload, klik.c_str(), klik.size());
         espBT.print(payload);
         std::string re = (espBT.readString()).c_str();
         
            deserializeJson(response_doc, re);
            
            std::string HC1 = response_doc["led1"];
            std::string HC2 = response_doc["led2"];
         
         int bytes1 = std::stoi(HC1, nullptr, 2);
         int bytes2 = std::stoi(HC2, nullptr, 2);
         Serial.printf("bytes1: %d\n", bytes1);
         Serial.printf("bytes2: %d\n", bytes2);

         digitalWrite(lat, LOW);
         shiftOut(data, clock, LSBFIRST, bytes1);
         digitalWrite(lat, HIGH);
         
         digitalWrite(lat2, LOW);
         shiftOut(data2, clock2, LSBFIRST, bytes2);
         digitalWrite(lat2, HIGH);
         }
      }
   }
   an = an1;
}