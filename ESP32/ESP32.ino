#include <WiFi.h>
#include <PubSubClient.h>

#define SENSOR_PIN 34
// WiFi credentials
const char* ssid = "";
const char* password = "";

// MQTT broker details
const char* mqtt_broker = "192.168.1.79"; 
const int mqtt_port = 1883;
const char* topic_temp ="temp";
const char* topic_state ="state";

// Static IP configuration
IPAddress local_IP(192, 168, 1, 50);  
IPAddress gateway(192, 168, 1, 1);    
IPAddress subnet(255, 255, 255, 0);   
IPAddress primaryDNS(8, 8, 8, 8);     
IPAddress secondaryDNS(8, 8, 4, 4);   

WiFiClient espClient;
PubSubClient client(espClient);

// Callback function to handle incoming messages
void callback(char* topic_state, byte* payload, unsigned int length) {
  Serial.print("\nMessage received on topic: ");
  Serial.println(topic_state);

  Serial.print("Message: ");
  String message = "";  

  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);  
    message += (char)payload[i];    
  }
  Serial.println();  

  Serial.print("Full message as String: ");
  Serial.println(message);
}

// Function to connect to WiFi
void setupWiFi() {
  delay(10);
  Serial.println("Configuring WiFi...");

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Failed to configure static IP. Proceeding with DHCP.");
  }

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

// Function to connect to the MQTT broker and subscribe to topics
void connectToMQTT() {
  Serial.println("Connecting to MQTT...");
  while (!client.connected()) {
    String clientId = "ESP32-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected to MQTT broker!");

      // Subscribe to multiple topics
      // client.subscribe("temp");
      client.subscribe("state");
      Serial.println("Subscribed to topics: state");

    } else {
      Serial.print("Failed to connect, retrying in 5 seconds. Error: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

// The setup function runs once when the ESP32 starts
void setup() {
  Serial.begin(115200);
  setupWiFi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);  // Register the callback function
  connectToMQTT();
  randomSeed(analogRead(0));  
}

void getTemperature(){
  int analogValue = analogRead(SENSOR_PIN);
  float voltage = analogValue * 3.3 / 4095.0;  
  float temperature = voltage * 100.0;  
  Serial.println("Temperature: " + String(temperature) + " °C");
  String temperatureStr = String(temperature, 2);  
  Serial.println("Temperature: " + temperatureStr + " °C");
  client.publish("temp", temperatureStr.c_str());
  delay(1000);  
}


void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();  

  getTemperature();
 
}
