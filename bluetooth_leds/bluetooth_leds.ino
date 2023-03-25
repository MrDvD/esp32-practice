#include <BluetoothSerial.h>

BluetoothSerial espBT;

int LEDS[] = {18, 19, 23, 5, 13, 14, 27, 16, 17, 26};
uint8_t address[6]  = {0x74, 0xC6, 0x3B, 0x38, 0xA4, 0xA0};

void setup() {
   Serial.begin(115200);
   espBT.begin("ESP32test", true);
   for(int i = 0; i<sizeof(LEDS) / sizeof(int);++i){
      pinMode(LEDS[i], OUTPUT);
   }
   if (!espBT.connect(address, 1)) {
      Serial.println("[BLUE_FAIL]");
      ESP.restart();
   } else {
      Serial.println("[BLUE_SUCCESS]");
   }
   
   // PINS
   pinMode(25, INPUT_PULLDOWN);
}
std::string led;
int but1_prev = 0;
int count = 0;
int click=0;
char payload[3];
void loop() {
   int but1_curr = digitalRead(25);
   payload[0] = '\0';
   
   if (but1_curr != but1_prev) {
      if (but1_curr) {
         click=(click+1)%11;
         Serial.printf("click: %d\n", click);
         char klik = click + '0';
         Serial.printf("klik: %c\n", klik);
         strncat(payload, &klik, 1);
         Serial.printf("payload: %s\n", payload);
         espBT.print(payload);
         led = (espBT.readString()).c_str();
         Serial.println(led.c_str());
         for(int j=0; j<led.size();j++){
            int k = led[j] - '0';
            digitalWrite(LEDS[j], k);
         }
      }
      but1_prev = but1_curr;
   }
   
   // uint8_t payload_int = (uint8_t)std::stoi(payload);
   // Serial.println(payload_int);
   // Serial.println("===");
   // char *payload = std::to_string(but1_curr).c_str() + std::to_string(but2_curr).c_str();
   // Serial.println(payload_int);
}