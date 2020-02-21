#include <ESP8266WiFi.h>

#define SSID "C3_LAB"
#define PASSWORD  "xupaxenga"
#define SERVER_PORT 5000

#define PIN_SENSOR 3
#define PIN_LED_STATUS 2

#define SENSOR_ACTIVATED 1
#define SENSOR_DISABLED 0

#define SEND_SENSOR_ACTIVATED 9
#define SEND_CONECTION_STATUS 1

bool dataSent = false;

unsigned long lastTime;

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
    blinkLed(500, 2000);
    return;
  }
  digitalWrite(PIN_LED_STATUS, LOW);


  if(digitalRead(PIN_SENSOR) == SENSOR_ACTIVATED && !dataSent){
    client.print(SEND_SENSOR_ACTIVATED);
    client.stop();

    dataSent = true;
  }

  if(digitalRead(PIN_SENSOR) == SENSOR_DISABLED && dataSent)
    dataSent = false;


  // if(millis() - lastTime >= 2000){
  //   client.print(SEND_CONECTION_STATUS);
  //   client.stop();

  //   lastTime = millis();
  // }
}


void waitConnection(){
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    blinkLed(500, 2000);
  }

  digitalWrite(PIN_LED_STATUS, HIGH);
}

void blinkLed(int timeOn, int timeOff){
  static unsigned long lastTimeBlink = 0;
  static int blinkInterval;
  static bool ledStatusState;

  if(millis() - lastTimeBlink > blinkInterval){
    ledStatusState = !ledStatusState;

    blinkInterval = ledStatusState ? timeOn : timeOff;

    digitalWrite(PIN_LED_STATUS, ledStatusState);

    lastTimeBlink = millis();
  }
}
