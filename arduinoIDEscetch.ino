/* Change your ssid & password */
#include <ESP8266WiFi.h>

const char* ssid = "wifilogin";
const char* password = "wifipassword";

// start web server
WiFiServer server(80);

void setup() {
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT); //  D4 on NodeMCU
  digitalWrite(2, 0);
  
  // prepare GPIO0
  pinMode(0, OUTPUT);
  digitalWrite(0, 0); // D3 on NodeMCU
  
  // Connect to WiFi 
  WiFi.begin(ssid, password);
  
// wait connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val_makecoffe;
  int val_stop;
  if (req.indexOf("/makecoffee") != -1)
    val_makecoffe = 1;
  else if (req.indexOf("/stop") != -1)
    val_stop = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  // set alert stop 
  digitalWrite(2, val_stop); 
  // Set GPIO2 & GPIO0 according to the request 
  digitalWrite(0, val_makecoffe); // push power button on coffe machine
  delay(15); // some delay beetween push the buttons
  digitalWrite(2, val_makecoffe); // push start button on cm
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nCoffeServer v0.1";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);

}
