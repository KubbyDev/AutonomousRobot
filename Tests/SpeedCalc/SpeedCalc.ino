#define IN1  2
#define IN2  3 
#define IN3  5   
#define IN4  6
#define ENA  9    // Needs to be a PWM pin to be able to control motor speed ENA
#define ENB  10   // Needs to be a PWM pin to be able to control motor speed ENA

// Speed control ------------------------------------------

void clamp(int* x, int min, int max) {
  if(*x < min) *x = min;
  if(*x > max) *x = max;
}

//Sets the speed of each motor (an integer between -255 and 255)
void setSpeed(int leftSpeed, int rightSpeed) {

  //The speed should always be between -255 and 255
  clamp(&leftSpeed, -255, 255);
  clamp(&rightSpeed, -255, 255);

  //Sets the speed for each motor
  analogWrite(ENA, abs(rightSpeed));
  analogWrite(ENB, abs(leftSpeed));

  //Sets the direction of rotation for each motor
  digitalWrite(IN1, rightSpeed > 0);
  digitalWrite(IN2, rightSpeed < 0);
  digitalWrite(IN3, leftSpeed > 0);
  digitalWrite(IN4, leftSpeed < 0);
}

// High level control functions -----------------------------

#define LEFT_MOTOR_SPEED 65
#define RIGHT_MOTOR_SPEED 75

void forward() {
  setSpeed(   LEFT_MOTOR_SPEED,   RIGHT_MOTOR_SPEED);
}

void backward() {
  setSpeed( - LEFT_MOTOR_SPEED, - RIGHT_MOTOR_SPEED);
}

void turnLeft() {
  setSpeed(   LEFT_MOTOR_SPEED, - RIGHT_MOTOR_SPEED);
}

void turnRight() {
  setSpeed( - LEFT_MOTOR_SPEED,   RIGHT_MOTOR_SPEED);
}

void stop() {
    setSpeed(0, 0);
}

// Test function --------------------------------------------

void setup() {
  
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
  pinMode(ENA, OUTPUT);  
  pinMode(ENB, OUTPUT);
  
  
  setSpeed(0,0);
  delay(1000);
  forward();
  delay(3000);
  setSpeed(0,0);
  delay(1000);
  backward();
  delay(3000);
  setSpeed(0,0);
  delay(1000);
  turnLeft();
  delay(3000);
  setSpeed(0,0);
  delay(1000);
  turnRight();
  delay(3000);
  setSpeed(0,0);
}

void loop() {
  
}
