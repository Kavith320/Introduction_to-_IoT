#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi settings
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT broker settings
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "university/iot/led";

// LED pin
const int ledPin = 2; // You can change this if needed

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Called when a message is received
void callback(char* topic, byte* message, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }

  Serial.print("Message received [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  // Control LED based on message
  if (msg == "ON") {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
  } else if (msg == "OFF") {
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF");
  }
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
}

void connectToMQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32_LED_Client")) {
      Serial.println("connected!");
      client.subscribe(mqttTopic); // Subscribe to LED topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);   // Set LED pin as output
  connectToWiFi();
  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();  // Reconnect if MQTT disconnected
  }
  client.loop();      // Handle incoming messages
}
