#include <WiFi.h>

// Wi-Fi credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// I/O pins
const int ledPin = 2;      // Built-in LED on most ESP32 boards
const int buttonPin = 4;   // Push button connected to GPIO 4

void setup() {
  Serial.begin(115200);

  // Set pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Button to GND

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) { // Button pressed
    digitalWrite(ledPin, HIGH);
    Serial.println("Button pressed - LED ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("Button released - LED OFF");
  }

  delay(500);  // Small delay to avoid spamming serial monitor
}
