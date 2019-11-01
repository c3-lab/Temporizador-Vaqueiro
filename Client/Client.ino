#include <ESP8266WiFi.h>

#define SSID "C3_LAB"
#define PASSWORD  "xupaxenga"
#define SERVER_PORT 5000

#define PIN_SENSOR 3
#define PIN_LED_STATUS 2

bool sent = false;

WiFiClient client;

void setup() {

  Serial.begin(115200);

  pinMode(PIN_SENSOR, INPUT_PULLUP);
  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, 0);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  waitConnection();
}

void loop() {

  if(!client.connect(WiFi.gatewayIP(), SERVER_PORT)){
    digitalWrite(PIN_LED_STATUS, 0);
    return;
  }
  digitalWrite(PIN_LED_STATUS, 1);

  if(!digitalRead(PIN_SENSOR) && !sent){
    client.print(9);
    client.stop();

    sent = true;
  }

  if(digitalRead(PIN_SENSOR) && sent)
    sent = false;
}


void waitConnection(){
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      digitalWrite(PIN_LED_STATUS, !digitalRead(PIN_LED_STATUS));
  }

  digitalWrite(PIN_LED_STATUS, 1);
}