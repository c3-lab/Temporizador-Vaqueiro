#include <ESP8266WiFi.h>

#define SSID "C3_LAB"
#define PASSWORD  "xupaxenga"
#define SERVER_PORT 5000

#define PIN_SENSOR 3
#define PIN_LED_STATUS 2

#define SENSOR_ACTIVATED 1
#define SENSOR_DISABLED 0

#define DATA_TO_SEND 9

bool dataSent = false;

WiFiClient client;

void setup() {

  pinMode(PIN_SENSOR, INPUT);
  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  waitConnection();
}

void loop() {

  if(!client.connect(WiFi.gatewayIP(), SERVER_PORT)){
    digitalWrite(PIN_LED_STATUS, LOW);
    return;
  }
  digitalWrite(PIN_LED_STATUS, HIGH);

  if(digitalRead(PIN_SENSOR) == SENSOR_ACTIVATED && !dataSent){
    client.print(DATA_TO_SEND);
    client.stop();

    dataSent = true;
  }

  if(digitalRead(PIN_SENSOR) == SENSOR_DISABLED && dataSent)
    dataSent = false;
}


void waitConnection(){
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      digitalWrite(PIN_LED_STATUS, !digitalRead(PIN_LED_STATUS));
  }

  digitalWrite(PIN_LED_STATUS, HIGH);
}