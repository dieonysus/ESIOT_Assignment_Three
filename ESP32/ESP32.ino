#include <WiFi.h>
#include <PubSubClient.h>

// Define default variables
int TEMP_DELAY = 1000;

// Definition temperature sensor pin
#define TEMP_PIN 34

// Definition of LED pins
int GREEN_PIN = 26;
int RED_PIN = 32;

// Definition of WiFi credentials 
const char* ssid = "FASTWEB-F5FPEF";
const char* password = "";

// Variables to connect to MQTT
const char* mqtt_broker = "192.168.1.79"; 
const int mqtt_port = 1883;
const char* topic_temp ="temp";
const char* topic_state ="state";


// Create a WiFi client for network communication
WiFiClient espClient;


// Create an MQTT client using the WiFi connection
PubSubClient client(espClient);


// Function to recieve  message from topic state
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
  Serial.println("Configuring WiFi...");
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 5) {  
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Error: Failed to connect to WiFi"); 
  }
  else {
    Serial.println("\nConnected to WiFi!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
  }
}


//Function to Connect to MQTT and Subscribe to topic State
void connectToMQTT() {
  Serial.println("Connecting to MQTT...");
    if(!client.connected()){
      String clientId = "ESP32-" + String(random(0xffff), HEX);
      if (client.connect(clientId.c_str())) {
        Serial.println("Connected to MQTT broker!");
        client.subscribe("state");
        Serial.println("Subscribed to topics: state");
      } else {
        Serial.print("Error: Failed to connect to MQTT");
        Serial.println(client.state());
      }
    }  
}


// Main setup function 
void setup() {
  Serial.begin(115200);
  setupWiFi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);  
  connectToMQTT();
}

// Function to setup LED lights

void setLED(char* color) {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  if(color == "red"){
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
  }
  else if(color == "green"){
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
  }
}

// Function to calculate Temperature and publish to topic temp

void getTemperature(){
  int analogValue = analogRead(TEMP_PIN);
  float voltage = (analogValue / 4095.0) * 5.0;  
  float temperature = (voltage - 0.5) * 100;  
  Serial.println("Info: Raw ADC Value: " + String(analogValue));
  Serial.println("Info: Voltage: " + String(voltage, 3) + " V");
  Serial.println("Info: Temperature: " + String(temperature, 2) + " °C");
  client.publish("temp", String(temperature, 2).c_str());
  delay(TEMP_DELAY);
}

// Main loop function
void loop() {
  if (WiFi.status() != WL_CONNECTED && !client.connected()){
    setLED("red");
    setupWiFi();
    connectToMQTT();
  } else {
    setLED("green");
    client.loop();
    getTemperature();
  }
}
