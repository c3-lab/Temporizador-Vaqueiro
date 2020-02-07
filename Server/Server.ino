#include <ESP8266WiFi.h>

#define SSID "C3_LAB"
#define PASSWORD  "xupaxenga"
#define SERVER_PORT 5000

#define PIN_SENSOR 2

#define DATA_RECEIVED '9'

WiFiServer server(SERVER_PORT);

void setup(){

  pinMode(PIN_SENSOR, OUTPUT);
  digitalWrite(PIN_SENSOR, HIGH);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD, 1, true); // ssid, passoword, channel, hidden

  server.begin();
}

void loop(){

  WiFiClient client = server.available();

  if (client){
    while (client.connected()) {
      while (client.available() > 0) {
        if(client.read() == DATA_RECEIVED){
          digitalWrite(PIN_SENSOR, LOW);
          delay(200);
          digitalWrite(PIN_SENSOR, HIGH);
        }
      }

      delay(10);
    }

    client.stop();
  }
}


