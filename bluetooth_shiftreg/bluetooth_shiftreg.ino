#include <BluetoothSerial.h>

#define bpin 25
#define lat 16
#define clock 27
#define data 17

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
   pinMode(bpin, INPUT_PULLDOWN);
   pinMode(lat, OUTPUT);
   pinMode(data, OUTPUT);
   pinMode(clock, OUTPUT);
}

int but1_prev = 0;
int count = 0;
int click=0;
char payload[3];
void loop() {
   int but1_curr = digitalRead(bpin);
   payload[0] = '\0';
   
   if (but1_curr != but1_prev) {
      if (but1_curr) {
            click=(click+1)%9;
            char klik = click + '0';
            strncat(payload, &klik, 1);
            espBT.print(payload);
            std::string re = (espBT.readString()).c_str();
            int bytes = std::stoi(re, nullptr, 2);
            
            digitalWrite(lat, LOW);
            shiftOut(data, clock, LSBFIRST, bytes);
            digitalWrite(lat, HIGH);
         }
      }
      but1_prev = but1_curr;
   }