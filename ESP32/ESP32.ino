#include <WiFi.h>
#include <PubSubClient.h>

// Define default variables
int TEMP_DELAY = 1000;

// Define variables to check MQTT disconnection sooner
unsigned long lastMQTTPing = 0;
const int MQTT_CHECK_INTERVAL = 5000; 

//Definition of temperature sensor pin
#define TEMP_PIN 34

// Definition of LED pins
int GREEN_PIN = 26;
int RED_PIN = 32;

// Definition of WiFi credentials 
const char* SSID = "FASTWEB-F5FPEF";
const char* WIFI_PASSWORD = "";

// Variables to connect to MQTT
const char* MQTT_BROKER = "192.168.1.79"; 
const int MQTT_PORT = 1883;
const char* TOPIC_TEMP ="temp";
const char* TOPIC_STATE ="state";


// Create a WiFi client for network communication
WiFiClient espClient;


// Create an MQTT client using the WiFi connection
PubSubClient client(espClient);


// Function to recieve  message from topic state and change delay of sending temperature based on message recieved 
void callback(char* TOPIC_STATE, byte* payload, unsigned int length) {
  Serial.print("\nMessage received on topic: ");
  Serial.println(TOPIC_STATE);
  Serial.print("Message: ");
  String message = "";  
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);  
    message += (char)payload[i];    
  }
  Serial.println();  
  Serial.print("Full message as String: ");
  Serial.println(message);
   if (message == "NORMAL") {
      TEMP_DELAY = 5000;  
  } else if (message == "HOT") {
      TEMP_DELAY = 3000;  
  } else if (message == "TOO_HOT") {
      TEMP_DELAY = 1000;  
  }
    else if (message == "ALARM") {
      TEMP_DELAY = 1000;
  }
}


// Function to connect to WiFi
void setupWiFi() {
  Serial.println("Configuring WiFi...");
  Serial.println("Connecting to WiFi...");
  WiFi.begin(SSID, WIFI_PASSWORD);
  int CONNECTION_ATTEMPTS = 0;
  while (WiFi.status() != WL_CONNECTED && CONNECTION_ATTEMPTS < 5) {  
    delay(500);
    Serial.print(".");
    CONNECTION_ATTEMPTS++;
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
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);  
  client.setKeepAlive(5);
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
  if (WiFi.status() != WL_CONNECTED){
    setLED("red");
    setupWiFi();
    connectToMQTT();
  } 
  if (!client.connected()) {
    setLED("red"); 
    connectToMQTT();
  }
  else {
    setLED("green");
    client.loop();
    getTemperature();
  }

  if (millis() - lastMQTTPing > MQTT_CHECK_INTERVAL) {
    if (!client.publish("ping", "1")) {  
      Serial.println("Error: MQTT Disconnected! Reconnecting...");
      client.disconnect();  
    }
    lastMQTTPing = millis();
  }
}