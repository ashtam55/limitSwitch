#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
const char *mqtt_server = "bot.akriya.co.in";
WiFiClient wifiClient;
PubSubClient mqttClient(mqtt_server, 1883, wifiClient);
int buttonPin = 23;
int lastState = 0;
int currentState = 0;
const char* ssid     = "wendor";
const char* password = "";
int buttonState = 0;
void mqtt(int state){
if (state == 1){
      mqttClient.publish("wendorEvents/dlf", "CLOSE");
}
else if(state == 0){
        mqttClient.publish("wendorEvents/dlf", "OPEN");

}

}

void reconnect()
{

  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str()))
    {
      Serial.println("connected");

      mqttClient.publish("wendorEvents/dlf", "Ready!");
    
    }

    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    pinMode(buttonPin,INPUT);

    Serial.print("Connecting to ");
    Serial.println(ssid);
  WiFi.mode(WIFI_STA);
    WiFi.begin(ssid);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  mqttClient.setServer(mqtt_server, 1883);
}

void loop() {
  // put your main code here, to run repeatedly:
      if (WiFi.status() == WL_CONNECTED)
  {

    if (!mqttClient.connected())
    {
      reconnect();
    }
  }

  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      mqtt(1);
      Serial.println("on");
    } else {
      mqtt(0);
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(100);
  }
  // save the current state as the last state, for next time through the loop
  lastState = buttonState;

      mqttClient.loop(); 
}
