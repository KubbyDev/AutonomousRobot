#include "Sonar.h"
#include "Data.h"

#include <Arduino.h>
#include <PinChangeInterrupt.h> //PinChangeInterrupt by NicoHood v1.2.6

#define SOUND_SPEED 0.03403f    // in cm/us

unsigned long lastTriggerTime = micros();

void updateSonar() {

    // If the time between this update and the last one is less than 25 ms, stops
    // 23.5 ms = 4m round trip
    // Also stops if the robot is turning
    if(micros() - lastTriggerTime < 25000 || fabsf(turnInput) > 0.1f)
        return;

    // Sends a 10 ms pulse
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(5);  
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Starts the measure of the travel time in microseconds
    lastTriggerTime = micros();
}

// Event called by the pin change interrupt when the echo is received
void onEchoReceived() {

    // Cancels the measurement if the robot is turning
    if(fabsf(turnInput) > 0.1f)
        return;

    // Calculates the travel time
    unsigned long travel = micros() - lastTriggerTime;
  
    // Calculates the corresponding distance
    float distance = (travel/2) * SOUND_SPEED;

    // Saves the calculated distance if it is relevant (some measures are wrong)
    if(distance > 0.1f && distance < 400.0f)
        lastDistance = distance;
}

void initSonar() {

    pinMode(TRIGGER_PIN, OUTPUT);
    digitalWrite(TRIGGER_PIN, LOW);
    pinMode(ECHO_PIN, INPUT);

    attachPCINT(digitalPinToPCINT(ECHO_PIN), onEchoReceived, FALLING);
}
