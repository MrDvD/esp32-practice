#include <Wire.h>
#include <DHT.h>

DHT dht(19, DHT11);

// HANDLERS
TaskHandle_t callback_humid;

// FUNCTIONS
void IRAM_ATTR send_humid(void *unused) {
   while true {
      float humidity = dht.readHumidity();
      std::string humid_str = std::to_string(humidity);
      Wire.write(humid_str.c_str());
   }
}

void setup() {
   Serial.begin(115200);
   Wire.begin(5);
   dht.begin();

   // PINS
   pinMode(19, INPUT);

   // TASKS
   xTaskCreatePinnedToCore(send_humid,         // function
                           "sendHumidity",     // name
                           100,                // size in words
                           NULL,               // params
                           0,                  // priority
                           &callback_humid,    // handler
                           0);                 // core
   
}

void loop() {
}