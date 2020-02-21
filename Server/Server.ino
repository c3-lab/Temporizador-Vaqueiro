#include <ESP8266WiFi.h>

#define SSID "C3_LAB"
#define PASSWORD  "xupaxenga"
#define SERVER_PORT 5000

#define PIN_SENSOR 2
#define PIN_LED_STATUS 1

#define RECEIVED_SENSOR_ACTIVATED '9'
#define RECEIVED_CONECTION_STATUS '1'


WiFiServer server(SERVER_PORT);

void setup(){

  pinMode(PIN_SENSOR, OUTPUT);
  digitalWrite(PIN_SENSOR, LOW);

  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, LOW);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD, 1, true); // ssid, passoword, channel, hidden

  server.begin();
}

void loop(){

  WiFiClient client = server.available();

   if (client){
    while (client.connected()) {
      while (client.available() > 0) {
        char data = client.read();
        if(data == RECEIVED_SENSOR_ACTIVATED){
          digitalWrite(PIN_SENSOR, HIGH);
          delay(50);
          digitalWrite(PIN_SENSOR, LOW);
        }
        // else if(data == RECEIVED_CONECTION_STATUS){
        //   digitalWrite(PIN_LED_STATUS, HIGH);
        //   delay(50);
        //   digitalWrite(PIN_LED_STATUS, LOW);
        // }
      }

      delay(10);
    }

    client.stop();
  }

}


