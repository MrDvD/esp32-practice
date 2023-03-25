// HANDLERS
TaskHandle_t led1_task;
TaskHandle_t led2_task;

boolean state1 = false;
boolean state2 = false;

void IRAM_ATTR change_state1() {
   state1 = !state1;
}

void IRAM_ATTR change_state2() {
   state2 = !state2;
}

// FUNCTIONS
void led1_blink(void *unused) {
   hw_timer_t *timer = timerBegin(0, 80, true);
   timerAttachInterrupt(timer, &change_state1, true);
   timerAlarmWrite(timer, 1000000, true);
   timerAlarmEnable(timer);
   
   pinMode(25, OUTPUT);
   while (true) {
      digitalWrite(25, state1);
   }
}

void led2_blink(void *unused) {
   hw_timer_t *timer = timerBegin(1, 40, true);
   timerAttachInterrupt(timer, &change_state2, true);
   timerAlarmWrite(timer, 1000000, true);
   timerAlarmEnable(timer);
   
   pinMode(26, OUTPUT);
   while (true) {
      digitalWrite(26, state2);
   }
}

void setup() {
   Serial.begin(115200);

   // TASKS
   xTaskCreatePinnedToCore(led1_blink,             // function
                           "led1Blinking",         // name
                           5000,                   // size in words
                           NULL,                   // params
                           1,                      // priority
                           &led1_task,             // handler
                           0);                     // core
   xTaskCreatePinnedToCore(led2_blink,             // function
                           "led2Blinking",         // name
                           5000,                   // size in words
                           NULL,                   // params
                           1,                      // priority
                           &led2_task,             // handler
                           1);                     // core
}

void loop() {
}