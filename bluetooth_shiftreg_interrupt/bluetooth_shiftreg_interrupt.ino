#include <BluetoothSerial.h>
#include <ArduinoJson.h>

#define button_pin 25
#define latch_pin 19
#define clock_pin 18
#define data_pin 23

uint8_t address[6] = { 0x74, 0xC6, 0x3B, 0x38, 0xA4, 0xA0 };

BluetoothSerial blue;

// HANDLER
boolean pressed = false;
void IRAM_ATTR button() {
   pressed = true;
}

void setup() {
  Serial.begin(115200);
  blue.begin("ESP32test", true);
  
  // BLUETOOTH
  if (!blue.connect(address, 1)) {
    Serial.println("[BLUE_FAIL]");
    ESP.restart();
  } else {
    Serial.println("[BLUE_SUCCESS]");
  }

  // PINS
  analogReadResolution(10);
  pinMode(button_pin, INPUT_PULLDOWN);
  pinMode(latch_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  
  // INTERRUPT
  attachInterrupt(25, button, RISING);
}

DynamicJsonDocument response_doc(64);
int prev_reostat;

void loop() {
   if (pressed) {
      pressed = false;
      int curr_rheostat = analogRead(34);
      
      if (prev_reostat != curr_rheostat) {
         prev_reostat = curr_rheostat;
         std::string payload = std::to_string(curr_rheostat);
         blue.print(payload.c_str());
         
         std::string re = (blue.readString()).c_str();
         deserializeJson(response_doc, re);
         std::string bytes_str = response_doc["led1"];

         int bytes = std::stoi(bytes_str, nullptr, 2);

         digitalWrite(latch_pin, LOW);
         shiftOut(data_pin, clock_pin, LSBFIRST, bytes);
         digitalWrite(latch_pin, HIGH);
      }
   }
}
