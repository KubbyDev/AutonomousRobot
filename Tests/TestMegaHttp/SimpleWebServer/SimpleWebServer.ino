#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "Mega";
const char *password = "12345678";

ESP8266WebServer server(80);

/* Just a little test message.  Go to the IP address you setup below in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void handleArgs() { 
  String message = "";
  if (server.arg("Temperature") == ""){     //Parameter not found
    message = "Temperature Argument not found";
  }else{     //Parameter found
    message = "Temperature Argument = ";
    message += server.arg("Temperature");     //Gets the value of the query parameter
  }
  server.send(200, "text/html", "<h1>You are connected</h1>" + message);          //Returns the HTTP response
}

void setup() {
  delay(1000);
  Serial.begin(57600);
  Serial.println();
  Serial.print("Configuring access point...");
  //--------------------------------------------

  IPAddress local_IP(192,168,0,20);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  //--------------------------------------------
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  // IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  //Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/args", handleArgs);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
