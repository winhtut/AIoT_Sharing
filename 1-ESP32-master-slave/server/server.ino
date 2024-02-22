#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "NCC_InstituteOfScience";
const char* password = "CrazySci3ntist";

WebServer server(80);

const char* htmlContent = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h1>Hello from ESP32!</h1>
  <p>Enter a string:</p>
  <form action="/submit" method="get">
    <input type="text" name="inputString">
    <input type="submit" value="Submit">
  </form>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlContent); // Serve the HTML content
}

void handleSubmit() {
  String inputString = server.arg("inputString"); // Get the value of the input field
  Serial.println("Received string: " + inputString); // Print the string to the Serial Monitor
  
  // Serve a simple HTML page acknowledging the received data
  String responseHTML = "<!DOCTYPE HTML><html><head><title>ESP32 Input Received</title></head><body><h1>Input Received</h1><p>Received string: " + inputString + "</p></body></html>";
  server.send(200, "text/html", responseHTML);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/submit", HTTP_GET, handleSubmit); // Route to handle form submission

  server.begin();
}

void loop() {
  server.handleClient();
}
