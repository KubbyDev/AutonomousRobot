#include "Servo.h"
#include "Data.h"

#include <Arduino.h>
#include <Servo.h>

Servo servo;
unsigned long lastChangeTime; // The last arms position change moment (ms)

void initServo() {
    servo.attach(SERVO_PIN);
    servo.write(SERVO_MAX);
    lastChangeTime = millis();
}

void updateServo() {

    // Does nothing if the position already changed shortly before
    if(millis() - lastChangeTime < SERVO_PERIOD)
        return;

    lastChangeTime = millis();

    // If the robot is not going forward, the arms will just go down
    if(forwardInput < 0.1f) {
        servo.write(SERVO_MAX);
        return;
    }
    
    // Else the arms will move up and down
    // Goes to the min angle if the current angle is higher than the middle, max otherwise
    int current = servo.read();
    if(current > (SERVO_MIN+SERVO_MAX)/2)
        servo.write(SERVO_MIN);
    else
        servo.write(SERVO_MAX);
}
