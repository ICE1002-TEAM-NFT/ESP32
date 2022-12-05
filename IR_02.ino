#include <WiFi.h>
#include <PubSubClient.h>
#include <sstream>      // std::stringstream

TaskHandle_t Task1;
TaskHandle_t Task2;

//Sensor
int digitalPin = 12;
char IR_data[50];
int value;

//Wifi
const char* ssid = "이창용’s iPhone";
const char* password =  "";

//MQTT Broker
const char* mqttServer = "";
const int mqttPort = 1883;
const char* topic_sub = "esp32/IR/get";
const char* topic_pub = "esp32/IR";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic_sub);
 
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) 
  {
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
  if(!value)
  {
    mqttClient.publish(topic_pub, "435, Empty"); 
  }
  else mqttClient.publish(topic_pub, "435, Using"); 
    Serial.println("publish_1");
}


void setup() 
{

  //Sensor
  pinMode(digitalPin, INPUT);

  // Set software serial baud to 115200;
  Serial.begin(115200);

  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  //connecting to a mqtt broker
  mqttClient.setServer(mqttServer, mqttPort);
 
  while (!mqttClient.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect("ESP32Client")) 
    {
      Serial.println("connected");
    } else 
    {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }

  mqttClient.setCallback(callback);
  mqttClient.subscribe(topic_sub);


  //for multithreading
  xTaskCreatePinnedToCore(
    pubLoop,         // 태스크 함수
    "Task2",           // 테스크 이름
    10000,             // 스택 크기(워드단위)
    NULL,              // 태스크 파라미터
    1,                 // 태스크 우선순위
    &Task1,            // 태스크 핸들
    0);                // 실행될 코어
  
  xTaskCreatePinnedToCore(
    clientLoop,         // 태스크 함수
    "Task1",           // 테스크 이름
    10000,             // 스택 크기(워드단위)
    NULL,              // 태스크 파라미터
    1,                 // 태스크 우선순위
    &Task2,            // 태스크 핸들
    1);             // 실행될 코어
}

void clientLoop ( void *param )
{
  Serial.print("# Task 1 running on core ");
  Serial.println(xPortGetCoreID());

  while (1)
  {
    mqttClient.loop();
  }
}

void pubLoop ( void *param )
{
  Serial.print("# Task 2 running on core ");
  Serial.println(xPortGetCoreID());

  while (1)
  {
  value = digitalRead(digitalPin);
    
  pubSensor();
  // mqttClient.publish(topic_pub, "435, Empty");
  Serial.println("publish_0");
  vTaskDelay(10000);
  }
}

void loop() 
{
  
}
