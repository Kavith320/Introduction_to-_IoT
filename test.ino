#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char *ssid = "Wokwi-GUEST";
const char *password = "";

// HiveMQ public broker
const char *mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char *mqttTopic = "student";

// Student info
const char *studentName = "S.A.C.A. Senanayaka";
const char *studentIndex = "ICT/24/927";

// MQTT setup
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void connectToWiFi()
{
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
}

void connectToMQTT()
{
    mqttClient.setServer(mqttServer, mqttPort);
    while (!mqttClient.connected())
    {
        Serial.print("Connecting to MQTT...");
        if (mqttClient.connect("ESP32Client"))
        {
            Serial.println("connected!");
        }
        else
        {
            Serial.print("failed. Error: ");
            Serial.println(mqttClient.state());
            delay(1000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    connectToWiFi();
    connectToMQTT();
}

void loop()
{
    if (!mqttClient.connected())
    {
        connectToMQTT();
    }
    mqttClient.loop();

    // Create message
    String message = "{\"name\":\"" + String(studentName) + "\",\"index\":\"" + String(studentIndex) + "\"}";

    // Publish message
    mqttClient.publish(mqttTopic, message.c_str());
    Serial.println("Message sent: " + message);

    delay(5000); // Wait 5 seconds
}
