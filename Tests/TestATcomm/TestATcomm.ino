#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(0, 1);      // RX, TX for ESP8266

void setup() {

  Serial.begin(115200);
  wifiSerial.begin(115200);
  delay(1000);
  Serial.println("Serial communication started");

  sendCommand("AT");
  sendCommand("AT");
}

void loop() {
  if(Serial.available() > 0) {
    String message = "";
    while(Serial.available() > 0)
      message += (char) Serial.read();
    sendCommand(message);
  }
}

void sendCommand(String command) {

  Serial.print("Command: ");
  Serial.println(command);

  wifiSerial.println(command);

  String response = "";
  while(wifiSerial.available())
    response += wifiSerial.read();

  Serial.print("Response: ");
  Serial.println(response);
}
