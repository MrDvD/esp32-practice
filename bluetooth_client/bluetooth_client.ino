#include <BluetoothSerial.h>

BluetoothSerial espBT;

uint8_t address[6]  = {0x74, 0xC6, 0x3B, 0x38, 0xA4, 0xA0};

void setup() {
   Serial.begin(115200);
   espBT.begin("ESP32test", true);
   if (!espBT.connect(address, 1)) {
      Serial.println("[BLUE_FAIL]");
      ESP.restart();
   } else {
      Serial.println("[BLUE_SUCCESS]");
   }
   
   // PINS
   pinMode(25, INPUT_PULLDOWN);
   pinMode(26, INPUT_PULLDOWN);
}

int but1_prev;
int but2_prev;
int count = 0;

void loop() {
   int but1_curr = digitalRead(25);
   int but2_curr = digitalRead(26);
   
   if (count >= 10) {
      char c = Serial.read();
      if (c == '1') {
         espBT.disconnect();
      }
   }
   
   if (but1_curr != but1_prev || but2_curr != but2_prev) {
      
      char but1_c = but1_curr + '0';
      char but2_c = but2_curr + '0';
      
      delay(20);
      char payload[3] = {but1_c, but2_c};

      // uint8_t payload_int = (uint8_t)std::stoi(payload);
      // Serial.println(payload_int);
      // Serial.println("===");
      // char *payload = std::to_string(but1_curr).c_str() + std::to_string(but2_curr).c_str();
      // Serial.println(payload_int);
      espBT.print(payload);
      Serial.println(payload);
      Serial.println("======");
      but1_prev = but1_curr;
      but2_prev = but2_curr;
      count++;
   }
}