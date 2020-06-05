#include "Sonar.h"
#include "Data.h"

#include <Arduino.h>
#include <PinChangeInterrupt.h> //PinChangeInterrupt by NicoHood v1.2.6

#define SOUND_SPEED 0.03403f    // in cm/us

unsigned long lastTriggerTime = micros();

void updateSonar() {

    DBG Serial.println("Updating Sonar ------------------------");

    // If the time between this update and the last one is less than 25 ms, stops
    // 23.5 ms = 4m round trip
    // Also stops if the robot is turning
    if(micros() - lastTriggerTime < 25000 || fabsf(turnInput) > 0.1f)
        return;

    DBG Serial.println("Sending pulse");

    // Sends a 10 us pulse
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

    // Calculates the travel time
    // The -350 counteracts a constant error of ~13 cm I noticed 
    // (maybe it takes some time to trigger the interrupt)
    unsigned long travel = micros() - lastTriggerTime - 350;

    DBG Serial.println("SONAR ------ Received pulse");

    // Cancels the measurement if the robot is turning
    if(fabsf(turnInput) > 0.1f) {
        DBG Serial.println("Cancelled because robot is turning");
        return;
    }
  
    // Calculates the corresponding distance
    float distance = (travel/2) * SOUND_SPEED;

    // Saves the calculated distance if it is relevant (some measures are wrong)
    if(distance > 0.1f && distance < 400.0f)
        lastDistance = distance;

    DBG Serial.print("Distance (cm)= ");Serial.println(distance);
}

void initSonar() {

    pinMode(TRIGGER_PIN, OUTPUT);
    digitalWrite(TRIGGER_PIN, LOW);
    pinMode(ECHO_PIN, INPUT);

    attachPCINT(digitalPinToPCINT(ECHO_PIN), onEchoReceived, FALLING);
}
