#include <Arduino.h>

constexpr uint8_t LED {8U};
// put function declarations here:
void print_helloWorld();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial) {
    yield();
  }
  
  print_helloWorld();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED,!digitalRead(LED));
  print_helloWorld();
  delay(1000);

}

// put function definitions here:
void print_helloWorld() {
  Serial.println("Hello World");
}