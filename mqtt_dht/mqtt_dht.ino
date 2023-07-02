#include<ESP8266WiFi.h>
#include<PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);
#include "DHT.h"
#define DHTTYPE DHT11
#define dht_dpin D2
DHT dht(dht_dpin,DHTTYPE);
const char* ssid ="Redmi";
const char* password="faizarasool";
const char* mqtt_server ="broker.mqtt-dashboard.com";
long lastMsg=0;
char msg[50];
int value=0;
String message ="";
char val[50];
String top;

void setup_wifi()
{
  delay(10);


  Serial.println();
  Serial.print("connecting to");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }


  randomSeed(micros());


  Serial.print("");
  Serial.println("WiFi connected");
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());
  
  
  }

  void callback(char* topic,byte* payload,unsigned int length)
  {
    top=topic;
    for(int i=0;i<length;i++) {
      message += (char)payload[i];
    }
  message +='\0';
  Serial.println(message);
  Serial.println();
    }
    void reconnect(){
      while(!client.connected()){
        Serial.print("Attempting MQTT connection...");
        String clientId="ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        if(client.connect(clientId.c_str())){
          Serial.println("connected");
          client.subscribe("node");
        }
    else {
      Serial.print("failed,rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 second");
      delay(5000);
    } 
   }
}
void setup(){
  Serial.begin(9600);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server,1883);
}

void loop(){
  client.setCallback(callback);
  if(!client.connected()){
    reconnect();
  }
  client.loop();
    float h=dht.readHumidity();
    float t=dht.readTemperature();
    Serial.print("Current humidity=");
    Serial.print(h);
    Serial.print("%");
    Serial.print("tempertaure=");
    Serial.print(t);
    Serial.println("c ");
    delay(1000);
        char a[20];
          char b[20];
            char c[20];
   String data = "HUMIDITY:"+String(h);
   String data1 = "TEMPERATURE:"+String(t);
          data.toCharArray(a,20);
         client.publish("node",a);
            data1.toCharArray(b,20);
         client.publish("node",b);
        int sensor = analogRead(A0);
        Serial.println("GAS:"+String(sensor));
        delay(1000);
          String data3="GAS:"+String(sensor);
             data3.toCharArray(c,20);
             client.publish("node",c);
if(t>20)
{
  client.publish("node","High temperature");
}
if(t<20)
{
  client.publish("node","NORMAL temperature");
}
}
 
