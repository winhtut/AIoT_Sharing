#include "WiFi.h"

// Replace these with your desired credentials.
const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

void setup() {
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  // Setting the ESP32 as an Access Point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP to be open.
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void loop() {
  // Nothing to do here
}
