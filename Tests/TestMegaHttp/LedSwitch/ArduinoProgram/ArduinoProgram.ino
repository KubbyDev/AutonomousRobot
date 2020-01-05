#define SERIAL Serial3
#define PIN 8

void setup() {
  pinMode(PIN, OUTPUT);
  SERIAL.begin(57600);
}

void loop() {
  if(SERIAL.available()) {
    char readChar = SERIAL.read();
    if(readChar == '0')
      digitalWrite(PIN, LOW);
    if(readChar == '1')
      digitalWrite(PIN, HIGH);
  }
}
