/*
==== Libraries ====
+ nRF24L01 -> https://github.com/nRF24/RF24

*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ==== Hardware map
#define PIN_NRF_CE 9
#define PIN_NRF_CSN 10

#define PIN_INFRA_SENSOR 2	

// ==== Macros
#define DEBUG(result, trueText, falseText) Serial.println(result ? trueText : falseText) 
#define LOG Serial.print
#define LOGln Serial.println

// ==== Instances
RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);

// ==== NRF24 variables
const byte address[6] = "c3lab";

// ==== Project variables
volatile bool flagDetectedObject = false;
const uint8_t sensorActivated = 43;	// any number

// ============================================
void setup() {
	Serial.begin(115200);

	// ---- Config Pins
	pinMode(PIN_INFRA_SENSOR, INPUT);
	attachInterrupt(digitalPinToInterrupt(PIN_INFRA_SENSOR), detectedObject, RISING);

	// ---- Config NRF24
	LOGln(F("--- Config NRF24 ---"));
	DEBUG(radio.begin(), F("begin() OK!"), F("begin() FAIL!"));
	DEBUG(radio.setDataRate(RF24_2MBPS), F("setDataRate() OK!"), F("setDataRate() FAIL!"));
	radio.setPALevel(RF24_PA_LOW);
	radio.setAutoAck(true);
	radio.setPayloadSize(1);
	radio.openWritingPipe(address);
	radio.stopListening();
}

// ============================================

void loop() {
	if(flagDetectedObject){
		LOGln("Object was detected!");
		bool sendingSuccess = false;
		do{
			sendingSuccess = radio.write(&sensorActivated, sizeof(uint8_t));
			DEBUG(sendingSuccess, F("write() OK!"), F("write() FAIL!"));
			delay(50);
		}while(!sendingSuccess);

		flagDetectedObject = false;
	}
}

// ============================================

void detectedObject(){
	flagDetectedObject = true;
}