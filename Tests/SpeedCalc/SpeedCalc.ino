// Execute copy.bat or copy.sh to get those files
#include "Data.h"
#include "Motors.h"

#define FORWARD_TIME 1000
#define BACKWARD_TIME 1000
#define RIGHT_TIME 1000
#define LEFT_TIME 1000

void setup() {
  
    initMotors();
    
    forwardInput = 0;
    turnInput = 0;
    updateMotors();
    
    delay(1000);
    forwardInput = 1;
    updateMotors();
    delay(FORWARD_TIME);
    forwardInput = 0;
    updateMotors();
    delay(1000);
    forwardInput = -1;
    updateMotors();
    delay(BACKWARD_TIME);
    forwardInput = 0;
    updateMotors();
    delay(1000);
    turnInput = 1;
    updateMotors();
    delay(LEFT_TIME);
    turnInput = 0;
    updateMotors();
    delay(1000);
    turnInput = -1;
    updateMotors();
    delay(RIGHT_TIME);
    turnInput = 0;
    updateMotors();
}

void loop() {
}
