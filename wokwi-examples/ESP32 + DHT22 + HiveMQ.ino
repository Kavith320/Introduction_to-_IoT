#include <WiFi.h>            // For Wi-Fi connection
#include <PubSubClient.h>    // For MQTT communication
#include <DHT.h>             // For DHT22 sensor

// Define DHT22 data pin and sensor type
#define DHTPIN 15            // DHT22 data pin connected to GPIO 15
#define DHTTYPE DHT22        // Specify sensor type

// Wi-Fi credentials
const char* ssid = "Wokwi-GUEST";     // Wi-Fi SSID
const char* password = "";            // Wi-Fi password (empty for Wokwi-GUEST)

// MQTT broker settings
const char* mqttServer = "broker.hivemq.com";  // HiveMQ public broker
const int mqttPort = 1883;                     // MQTT port (non-TLS)
const char* mqttTopic = "index/iot/dht22"; // Topic to publish sensor data

// Create WiFi and MQTT client instances
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);  // Create DHT sensor object

// Function to connect to Wi-Fi
void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
}

// Function to connect to the MQTT broker
void connectToMQTT() {
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32_DHT22_Client")) {
      Serial.println("connected.");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);  // Wait before retry
    }
  }
}

// Arduino setup function (runs once)
void setup() {
  Serial.begin(115200);   // Start serial monitor
  dht.begin();            // Initialize DHT sensor
  connectToWiFi();        // Connect to Wi-Fi
  connectToMQTT();        // Connect to MQTT broker
}

// Arduino main loop (runs continuously)
void loop() {
  // Reconnect to MQTT if disconnected
  if (!client.connected()) {
    connectToMQTT();
  }

  client.loop();  // Maintain MQTT connection

  // Read temperature and humidity from DHT22
  float temperature = dht.readTemperature(); // In Celsius
  float humidity = dht.readHumidity();

  // Check if sensor data is valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Format data into JSON string
  String payload = "{\"temperature\":" + String(temperature, 1) +
                   ",\"humidity\":" + String(humidity, 1) + "}";

  // Publish the JSON message to MQTT topic
  client.publish(mqttTopic, payload.c_str());

  // Print to Serial Monitor
  Serial.println("Published: " + payload);

  delay(5000);  // Wait 5 seconds before sending again
}
