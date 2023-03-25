// HANDLERS
TaskHandle_t led1_task;
TaskHandle_t led2_task;

// TIMER
hw_timer_t *timerBegin(0, 80, true);

// FUNCTIONS
void led1_blink(void *unused) {
   pinMode(25, OUTPUT);
   while (true) {
      digitalWrite(25, HIGH);
      delay(500);
      digitalWrite(25, LOW);
      delay(500);
   }
}

void led2_blink(void *unused) {
   pinMode(26, OUTPUT);
   while (true) {
      digitalWrite(26, HIGH);
      delay(250);
      digitalWrite(26, LOW);
      delay(250);
   }
}

void setup() {
   Serial.begin(115200);

   // TASKS
   xTaskCreatePinnedToCore(led1_blink,             // function
                           "led1Blinking",         // name
                           1000,                   // size in words
                           NULL,                   // params
                           1,                      // priority
                           &led1_task,             // handler
                           0);                     // core
   xTaskCreatePinnedToCore(led2_blink,             // function
                           "led2Blinking",         // name
                           1000,                   // size in words
                           NULL,                   // params
                           1,                      // priority
                           &led2_task,             // handler
                           1);                     // core
}

void loop() {
}