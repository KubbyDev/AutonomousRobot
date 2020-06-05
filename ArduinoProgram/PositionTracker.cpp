#include "Data.h"
#include "Tools.h"

#include <Arduino.h>
#include <stdlib.h>

unsigned long lastUpdateTime = 0;

// Returns a number between 0 and 1 representing the fraction of the max speed
// the robot is supposed to reach on average during the given interval
// Multiply by ROBOT_SPEED to get the average velocity of the robot during this interval
// Multiply by ROBOT_TURN_RATE to get the average angular velocity during this interval
// timeAfterStart in us, deltaTime in us
float getVelocityFractionForInterval(unsigned long timeAfterStart, unsigned long deltaTime) {

    // In this function all the times are relative to the moment when the robot started moving
    // so ACCEL_DURATION is the moment when the acceleration phase ends
    // The velocity fraction at a given point can be computed like this: 
    // timeAfterStart < ACCEL_DURATION ? (float)timeAfterStart/ACCEL_DURATION : 1

    unsigned long lowBound = timeAfterStart - deltaTime;
    unsigned long highBound = timeAfterStart;

    // If the whole interval is after the acceleration phase
    if(lowBound > ACCEL_DURATION)
        return 1.0f;

    // If the whole interval is during the acceleration phase
    if(highBound < ACCEL_DURATION)
        return (float)(lowBound+highBound) / (float)(2*ACCEL_DURATION);

    // If the interval is on both phases
    unsigned long accelTime = ACCEL_DURATION - lowBound;
    unsigned long constantTime = highBound - ACCEL_DURATION;
    float accelPart = (float)(lowBound+ACCEL_DURATION) / (float)(2*ACCEL_DURATION);
    return (float)(accelPart*accelTime + constantTime) / (float)deltaTime;
}

void updatePosition() {

    unsigned long now = micros();

    float velocityFraction = getVelocityFractionForInterval(now - navStateChangeTime, now - lastUpdateTime);
    float deltaTimeSec = (float)(now - lastUpdateTime) * 1e-6;
    lastUpdateTime = now;
    
    float speed = forwardInput * (ROBOT_SPEED/PIXEL_LENGTH) * deltaTimeSec * velocityFraction;
    position->x += cos(rotation) * speed;
    position->y += sin(rotation) * speed;
    rotation += turnInput * ROBOT_TURN_RATE * deltaTimeSec * velocityFraction;
    clampAngle(&rotation);
}

// Called after a waiting phase to avoid having a delta time that contains all the waiting time
void resetLastUpdateTime() {
    lastUpdateTime = micros();
}
