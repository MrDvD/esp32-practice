#include <Wire.h>

#define I2C_SDA 21
#define I2C_SCL 22

hw_timer_t *timer = timerBegin(0, 80, true);

// HANDLERS
TaskHandle_t sender;

boolean pressed = false;
void IRAM_ATTR button() {
   pressed = true;
}

// FUNCTIONS
void IRAM_ATTR get_humid() {
   std::string humid;
   Wire.requestFrom(8, 10); // slave #8, 10 bytes
   while (Wire.available()) {
      char c = Wire.read();
      humid.append(&c);
   }
   Serial.printf("Got humid: %s\n", humid.c_str());
}

void IRAM_ATTR send_button(void *unused) {
   if (pressed) {
      Wire.beginTransmission(5);
      Wire.write(pressed);
      Wire.endTransmission(5);
   }
}

void setup() {
   Serial.begin(115200);
   Wire.begin(I2C_SDA, I2C_SCL);
   timerAlarmWrite(timer, 5000, true);
   timerAlarmEnable(timer);

   // PINS

   // TASKS
   xTaskCreatePinnedToCore(send_button,  // function
                           "sendButtonState",  // name
                           100,                // size in words
                           NULL,               // params
                           0,                  // priority
                           &sender,            // handler
                           0);                 // core

   // INTERRUPTS
   attachInterrupt(25, button, RISING);
   timerAttachInterrupt(timer, &get_humid, true);
}

void loop() {
}