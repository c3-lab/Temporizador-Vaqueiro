#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(8, 9); // CE, CSN

const byte address[6] = "c3lab";

uint8_t counter = 0;
bool sendSuccess = false;

void setup() {
  Serial.begin(115200);

  Serial.println(radio.begin() ? "begin() OK!" : "begin() FALHOU!");
  Serial.println(radio.setDataRate(RF24_2MBPS) ? "setDataRate() OK!" : "setDataRate() FALHOU!");
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(true);
  radio.setPayloadSize(1);

  radio.openWritingPipe(address);
  radio.stopListening();

}

void loop() {
  counter++;
  do{
    sendSuccess = radio.write(&counter, sizeof(uint8_t));
    Serial.print(sendSuccess ? "write() OK!" : "write() FALHOU!");
    Serial.print(" --- ");
    Serial.println(counter);
    delay(50);
  }while(!sendSuccess);
  
  delay(500);
}
