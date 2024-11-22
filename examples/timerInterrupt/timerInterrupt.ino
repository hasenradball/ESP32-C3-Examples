#include <Arduino.h>

constexpr uint8_t LED_PIN {8U};

hw_timer_t * timer = NULL;
bool timer_fired {false};

// When Fired change Pin for LED 
void IRAM_ATTR timer_isr() {
   timer_fired = true;
   digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
void setup() {
   Serial.begin(115200);
   while (!Serial) {
      yield();
   }
   delay(1000);

   Serial.print("\n\n=== Timer Example ===\n\n");
   pinMode(LED_PIN, OUTPUT);
   
   byte timer_id = 0;
   // precaler 80 => microsenconds
   unsigned int prescaler = 80;
   // timer in microseconds => 30 s
   unsigned int limit = 30000000; // int is 4 bytes on the ESP32
   
   timer = timerBegin(timer_id, prescaler, true);
   timerAttachInterrupt(timer, &timer_isr, true);
   timerAlarmWrite(timer, limit, true);
   timerAlarmEnable(timer);
}


void loop() {
   if (timer_fired) {
      timer_fired = false;
      Serial.print("timer has fired!\n");
   }
   
}