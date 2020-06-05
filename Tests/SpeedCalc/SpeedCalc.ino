// Execute copy.bat or copy.sh to get those files
#include "Data.h"
#include "Motors.h"

#define MEASURE_TIME 1000

void setup() {
  
    initMotors();
    
    forwardInput = 0;
    turnInput = 0;
    updateMotors();
    
    delay(1000);
    forwardInput = 1;
    updateMotors();
    delay(MEASURE_TIME);
    forwardInput = 0;
    updateMotors();
    delay(1000);
    forwardInput = -1;
    updateMotors();
    delay(MEASURE_TIME);
    forwardInput = 0;
    updateMotors();
    delay(1000);
    turnInput = 1;
    updateMotors();
    delay(MEASURE_TIME);
    turnInput = 0;
    updateMotors();
    delay(1000);
    turnInput = -1;
    updateMotors();
    delay(MEASURE_TIME);
    turnInput = 0;
    updateMotors();
}

void loop() {
}
