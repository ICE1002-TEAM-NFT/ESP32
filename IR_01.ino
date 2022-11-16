#include <WiFi.h>
#include <PubSubClient.h>
#include <sstream>      // std::stringstream

//Sensor
int digitalPin = 12;
char IR_data[50];
int value;

//Wifi
const char* ssid = "이창용’s iPhone";
const char* password =  "qkdghk3122";

//MQTT Broker
const char* mqttServer = "34.125.39.205";
const int mqttPort = 1883;
const char* topic_sub = "esp32/IR/get";
const char* topic_pub = "esp32/IR";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic_sub);
 
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  // if (topic == topic_sub)
  // {
  //   Serial.println("IR_data");
  //   pubSensor();
  // }
  
  Serial.println("IR_data");
  pubSensor();
  
}

void pubSensor(){

  sprintf(IR_data, "%c", value);
  if(!value){
    mqttClient.publish(topic_pub, "Empty"); 
  }
  else mqttClient.publish(topic_pub, "Using"); 
    Serial.println("publish");
}


void setup() {

  //Sensor
  pinMode(digitalPin, INPUT);

  // Set software serial baud to 115200;
  Serial.begin(115200);

  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  //connecting to a mqtt broker
  mqttClient.setServer(mqttServer, mqttPort);
 
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }

  mqttClient.setCallback(callback);
  mqttClient.subscribe(topic_sub);
}

void loop() {
  value = digitalRead(digitalPin);
  //Serial.println(value);
  mqttClient.loop();
}
