#include <WiFi.h>
#include <PubSubClient.h>

constexpr int TEMP_PIN = 34;
constexpr int GREEN_PIN = 26;
constexpr int RED_PIN = 32;
constexpr int MQTT_CHECK_INTERVAL = 5000;
constexpr int CONNECTION_ATTEMPTS_LIMIT = 5;
constexpr char SSID[] = "FASTWEB-F5FPEF";
constexpr char WIFI_PASSWORD[] = "9PQT4DFSUS";
constexpr char MQTT_BROKER[] = "192.168.1.79";
// constexpr char MQTT_BROKER[] = "test.mosquitto.org";
constexpr int MQTT_PORT = 1883;
constexpr char TOPIC_TEMP[] = "temp";
constexpr char TOPIC_STATE[] = "state";

int tempDelay = 1000;
WiFiClient espClient;
PubSubClient client(espClient);

bool connectToWiFi() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(SSID, WIFI_PASSWORD);
    int connectionAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && connectionAttempts++ < CONNECTION_ATTEMPTS_LIMIT){
        delay(500);
        Serial.print(".");
    }
      return (WiFi.status() == WL_CONNECTED)  
        ? (Serial.printf("\nConnected! IP Address: %s\n", WiFi.localIP().toString().c_str()), true)  
        : (Serial.println("\nError: Failed to connect to WiFi"), false);
}

bool connectToMQTT() {
    // Serial.println("Connecting to MQTT...");
     String clientId = "ESP32-" + String(random(0xFFFF), HEX);
        return (client.connect(clientId.c_str())  
        ? (Serial.println("Connected to MQTT broker!"),  
           client.subscribe(TOPIC_STATE),  
           Serial.printf("Subscribed to topic: %s\n", TOPIC_STATE),  
           true)  
        : (Serial.printf("Error: Failed to connect to MQTT (State: %d)\n", client.state()), false));
}

void setupLEDs() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
}

void setLED(bool isConnected) {
    digitalWrite(GREEN_PIN, isConnected ? HIGH : LOW);
    digitalWrite(RED_PIN, isConnected ? LOW : HIGH);
}

void checkWiFiAndMQTT() {
    (WiFi.status() == WL_CONNECTED || connectToWiFi()) 
        && (client.connected() || connectToMQTT()) 
        ? setLED(true) 
        : setLED(false);
}


void publishTemperature() {
    int analogValue = analogRead(TEMP_PIN);
    float voltage = (analogValue / 4095.0) * 5.0;
    float temperature = (voltage - 0.5) * 100;
    
    Serial.printf("Info: Raw ADC Value: %d\nInfo: Voltage: %.3f V\nInfo: Temperature: %.2f °C\n", 
                  analogValue, voltage, temperature);
    
    client.publish(TOPIC_TEMP, String(temperature, 2).c_str());
    delay(tempDelay);
}

void messageCallback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.printf("\nMessage received on topic: %s\nMessage: %s\n", topic, message.c_str());
    
    if (message == "NORMAL") tempDelay = 5000;
    else if (message == "HOT") tempDelay = 3000;
    else if (message == "TOO_HOT" || message == "ALARM") tempDelay = 1000;
}

void setup() {
    Serial.begin(115200);
    setupLEDs();
    connectToWiFi();
    client.setServer(MQTT_BROKER, MQTT_PORT);
    client.setCallback(messageCallback);
    client.setKeepAlive(30);
    connectToMQTT();
}

void loop() {
    checkWiFiAndMQTT();
    client.loop();
    publishTemperature();
}