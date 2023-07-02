#include<ESP8266WiFi.h>
#include <PubSubClient.h>
const int LED = D4;
const char* ssid="Redmi";
const char* password="faizarasool";
const char* mqtt_server="broker.mqtt-dashboard.com";
const int mqtt_port=1883;
WiFiClient espClient;
PubSubClient client(espClient);
void setup()
{
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("connected to wifi");
  Serial.println(WiFi.SSID());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(MQTTcallback);
  while(!client.connected())
  {
    Serial.println("connecting to MQTT...");
    if(client.connect("ESP8266"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed with state");
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.subscribe("Redmi");
}
void MQTTcallback(char* topic,byte* payload,unsigned int length)
{
  Serial.print("Message received in topic:");
  Serial.println(topic);
  Serial.print("Message");
  String message;
  for(int i=0;i<length;i++)
  {
    message=message+(char)payload[i];
  }
  Serial.print(message);
  if(message=="on")
  {
    digitalWrite(LED,HIGH);
  }
  else if (message=="off")
  {
    
    digitalWrite(LED,LOW);
  }
  Serial.println();
  Serial.println("------------------");
  }
void loop()
{
  if(digitalRead(D1)==0)
  {
    client.publish("rasool","hello from ESP8266");
    delay(1000);
  }
  else;
  {
    client.loop();
  }
}
      
