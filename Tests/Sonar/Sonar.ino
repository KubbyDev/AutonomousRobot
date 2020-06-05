#include <PinChangeInterrupt.h>

#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define SOUND_SPEED 0.03403f    // in cm/us

unsigned long lastTriggerTime = micros();

void setup() {

  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(115200);

  attachPCINT(digitalPinToPCINT(ECHO_PIN), printDistance, FALLING);
}

void loop() {

  // Sends a 10 us pulse
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(5);  
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Starts the measure of the travel time in microseconds
  lastTriggerTime = micros();
  Serial.write("Start measure  ");

  delay(25); // 23.5 ms = 4m round trip
}

void printDistance() {

  // Calculates the travel time
  unsigned long travel = micros() - lastTriggerTime - 400;
  
  // Calculates the corresponding distance
  float distance = (travel/2) * SOUND_SPEED;
  
  Serial.println(distance);
}
