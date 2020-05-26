// Video demonstration:
// https://www.youtube.com/watch?v=WvEudefDwm0

#include <Servo.h>

#define IN1  4
#define IN2  5
#define IN3  6
#define IN4  7
#define ENA  9   // Needs to be a PWM pin (this one is a PWN on both the UNO and the MEGA)
#define ENB  10  // Needs to be a PWM pin (this one is a PWN on both the UNO and the MEGA)
#define SERVO_PIN 2

#define ARMS_UP 120
#define ARMS_MID 150
#define ARMS_DOWN 180

Servo servo;

void clamp(int* x, int min, int max) {
  if(*x < min) *x = min;
  if(*x > max) *x = max;
}

// Sets the speed of each motor (an integer between -255 and 255)
void setSpeed(int leftSpeed, int rightSpeed) {

    // The speed should always be between -255 and 255
    clamp(&leftSpeed, -255, 255);
    clamp(&rightSpeed, -255, 255);

    // Sets the direction of rotation for each motor
    digitalWrite(IN1, rightSpeed > 0);
    digitalWrite(IN2, rightSpeed < 0);
    digitalWrite(IN3, leftSpeed > 0);
    digitalWrite(IN4, leftSpeed < 0);

    if(leftSpeed == 0 && rightSpeed == 0) {
        // Brakes if the speed of both motors are 0
        digitalWrite(ENA, HIGH);
        digitalWrite(ENB, HIGH);
    }
    else {
        // Otherwise, sets the speed for each motor
        analogWrite(ENA, abs(rightSpeed));
        analogWrite(ENB, abs(leftSpeed));
    }
}

void setup() {

    pinMode(IN1, OUTPUT); 
    pinMode(IN2, OUTPUT); 
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT); 
    pinMode(ENA, OUTPUT);  
    pinMode(ENB, OUTPUT);
    servo.attach(SERVO_PIN);

    servo.write(ARMS_DOWN);
    delay(10000);    

    // Arms movements
    for(int i = 0; i < 3; i++) {
        delay(200);
        servo.write(ARMS_UP);
        delay(200);
        servo.write(ARMS_DOWN);
    }

    // Turns 180 degrees
    setSpeed(-255,255);
    delay(400);
    setSpeed(0,0);
    delay(1000);

    // Realisation
    servo.write(ARMS_UP);
    delay(500);

    // Excitement
    setSpeed(-255, 255);
    for(int i = 0; i < 10; i++) {
        servo.write(ARMS_UP);
        delay(200);
        servo.write(ARMS_DOWN);
        delay(200);
    }
    setSpeed(0, 0);
}

void loop() {
    while(1) {
    }
}
