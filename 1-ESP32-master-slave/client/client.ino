#include <WiFi.h>
#include <HTTPClient.h>
//(client)
const char* ssid = "NCC_InstituteOfScience";
const char* password = "CrazySci3ntist";
const char* serverIP = "http://192.168.1.4/"; // Change to your ESP32 server IP

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    HTTPClient http;
    http.begin(serverIP);

    int httpCode = http.GET(); //Make the request

    if (httpCode > 0) { //Check for the returning code
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
    else {
      Serial.println("Error on HTTP request");
    }

    http.end(); //Free the resources
  }

  delay(10000);
}
