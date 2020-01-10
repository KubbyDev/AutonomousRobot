#define PIN A0

void setup() {

    Serial.begin(115200);
    pinMode(PIN, INPUT);
    
}

void loop() {

    Serial.println(analogRead(PIN));
    Serial.flush();

}
