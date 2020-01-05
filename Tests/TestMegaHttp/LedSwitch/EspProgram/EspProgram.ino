#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "Mega";
const char *password = "12345678";

ESP8266WebServer server(80);

#define SERIAL Serial

void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void handleArgs() { 
  String message = "";
  if (server.arg("state") == ""){     //Parameter not found
    message = "state argument not found";
  }else{     //Parameter found
    message = "New state = ";
    message += server.arg("state");     //Gets the value of the query parameter
    SERIAL.write(server.arg("state")[0]);
  }
  server.send(200, "text/html", "<h1>You are connected</h1>" + message);          //Returns the HTTP response
}

void setup() {

  SERIAL.begin(57600);
  IPAddress local_IP(192,168,0,20);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("ESPsoftAP_01");
  WiFi.softAPIP();

  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.on("/args", handleArgs);
  server.begin();
}

void loop() {
  server.handleClient();
}
