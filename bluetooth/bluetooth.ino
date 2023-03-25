#include <BluetoothSerial.h>

BluetoothSerial espBT;

void setup() {
   Serial.begin(115200);
   if (!espBT.begin("ESP32")) {
      Serial.println("[BLUE_FAILED]");
      ESP.restart();
   } else {
      Serial.println("[BLUE_SUCCESS]");
   }
}

std::string get_text() {
   char TEXT[1000];
   TEXT[0] = '\0';
   while (espBT.available()) {
      char c = espBT.read();
      strncat(TEXT, &c, 1);
   }
   return TEXT;
}

void loop() {
   if (espBT.available()) {
      Serial.println(get_text().c_str());
   }
   delay(100);
}