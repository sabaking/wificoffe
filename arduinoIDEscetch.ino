#include <ESP8266WiFi.h>

const char* ssid = "zhdanya";
const char* password = "Qwe12345";

// start web server
WiFiServer server(80);

void setup() {
  delay(10);

  // prepare GPIO0
  pinMode(0, OUTPUT);
  digitalWrite(0, 0); // D3 on NodeMCU
  
  // prepare GPIO2
  pinMode(2, OUTPUT); //  D4 on NodeMCU
  digitalWrite(2, 0);
  

  
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
  client.flush();
  // Match the request
  if (req.indexOf("/makecoffee") != -1){
      // Set GPIO2 & GPIO0 according to the request 
      digitalWrite(0, 1); // push power button on coffe machine
      delay(5000);
      digitalWrite(0, 0); // unpush power button on coffe machine 
      delay(10000); // some delay beetween push the buttons
      digitalWrite(2, 1); // push start button on cm
      delay(5000);
      digitalWrite(2, 0); // unpush start button on cm
  }
  else if (req.indexOf("/stop") != -1){
      // set alert stop , poweroof & push stop button on coffeemachine - 
      digitalWrite(2, 0);
      digitalWrite(0, 0);
      delay(500);
      digitalWrite(2, 0); 
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nCoffeServer v0.1";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);

}
