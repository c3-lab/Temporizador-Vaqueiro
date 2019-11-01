#include <ESP8266WiFi.h>

#define SSID "C3_LAB"
#define PASSWORD  "xupaxenga"
#define SERVER_PORT 5000

#define PIN_SENSOR 2

WiFiServer server(SERVER_PORT);

void setup(){

  pinMode(PIN_SENSOR, OUTPUT);
  digitalWrite(PIN_SENSOR, 1);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD, 1, true);

  server.begin();
}

void loop(){

  WiFiClient client = server.available();

  if (client){
    while (client.connected()) {
      while (client.available() > 0) {
        if(client.read() == '9'){
          digitalWrite(PIN_SENSOR, 0);
          delay(200);
          digitalWrite(PIN_SENSOR, 1);
        }
      }

      delay(10);
    }

    client.stop();
  }
}


