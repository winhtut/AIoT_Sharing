#include <WiFi.h>
#include <DHT.h>

// WiFi network settings
const char* ssid = "ESP32-Access-Point"; // SSID of your WiFi network
const char* password = "123456789"; // Password of your WiFi network

// Server settings
const char* host = "192.168.4.1"; // IP address of the ESP32 server
const uint16_t port = 23; // Port the server is listening on

// DHT sensor settings
#define DHTPIN 4 // GPIO pin connected to the DHT11 sensor
#define DHTTYPE DHT11 // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200); // Initialize serial communication
  dht.begin(); // Initialize the DHT sensor
  WiFi.begin(ssid, password); // Connect to the WiFi network

  while (WiFi.status() != WL_CONNECTED) { // Wait until connected to WiFi
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Wait a few seconds between measurements
  delay(2000);

  // Reading temperature and humidity takes about 250 milliseconds
  float humidity = dht.readHumidity(); // Read humidity
  float temperature = dht.readTemperature(); // Read temperature in Celsius

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temp: ");
  Serial.print(temperature);
  Serial.println("C");

  // Connect to the TCP server
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("Connection failed");
    return;
  }

  // Send the temperature and humidity to the server
  String dataToSend = "Humidity: " + String(humidity) + "%, Temp: " + String(temperature) + "C";
  client.println(dataToSend);

  // Close the connection
  client.stop();
}
