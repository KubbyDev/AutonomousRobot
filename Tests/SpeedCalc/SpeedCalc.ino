// Execute copy.bat or copy.sh to get those files
#include "Data.h"
#include "Motors.h"

void setup() {
  
    initMotors();
    
    forwardInput = 0;
    turnInput = 0;
    updateMotors();
    
    delay(1000);
    forwardInput = 1;
    updateMotors();
    delay(3000);
    forwardInput = 0;
    updateMotors();
    delay(1000);
    forwardInput = -1;
    updateMotors();
    delay(3000);
    forwardInput = 0;
    updateMotors();
    delay(1000);
    turnInput = -1;
    updateMotors();
    delay(3000);
    turnInput = 0;
    updateMotors();
    delay(1000);
    turnInput = 1;
    updateMotors();
    delay(3000);
    turnInput = 0;
    updateMotors();
}

void loop() {
}
