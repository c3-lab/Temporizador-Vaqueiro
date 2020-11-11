/*
==== Libraries ====
+ nRF24L01 -> Arduino Library Manager (github.com/nRF24/RF24)
+ LiquidCrystal -> Default library
+ TimerOne -> Arduino Library Manager

*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>

// ==== Hardware map
#define PIN_NRF_CE 9
#define PIN_NRF_CSN 10

#define PIN_INFRA_SENSOR 2

#define PIN_RESET_BUTTON 7

#define LCD_RW    "GND"
#define LCD_RS    A0
#define LCD_EN    A1
#define LCD_D4    A2
#define LCD_D5    A3
#define LCD_D6    A4
#define LCD_D7    A5


// ==== Macros
#define LOG Serial.print
#define LOGln Serial.println
#define DEBUG(result, trueText, falseText) LOGln(result ? trueText : falseText) 

// ==== LCD Definitions
#define CUSTOM_A (byte) 0
#define CUSTOM_B (byte) 1
#define CUSTOM_C (byte) 2
#define CUSTOM_D (byte) 3
#define CUSTOM_E (byte) 4
#define CUSTOM_F (byte) 5
#define CUSTOM_G (byte) 6
#define CUSTOM_H (byte) 7

byte a[8] = {B00011, B01111, B11111, B11111, B11111, B11111, B11111, B11111};
byte b[8] = {B11000, B11110, B11111, B11111, B11111, B11111, B11111, B11111};
byte c[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B01111, B00011};
byte d[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11110, B11000};
byte e[8] = {B00000, B00000, B00000, B00000, B00000, B11111, B11111, B11111};
byte f[8] = {B11111, B11111, B11111, B00000, B00000, B00000, B00000, B00000};
byte g[8] = {B11111, B11111, B11111, B00000, B00000, B00000, B11111, B11111};
byte h[8] = {B11111, B00000, B00000, B00000, B00000, B11111, B11111, B11111};

void number0(int col);
void number1(int col);
void number2(int col);
void number3(int col);
void number4(int col);
void number5(int col);
void number6(int col);
void number7(int col);
void number8(int col);
void number9(int col);

typedef void (*Functions)(int);

Functions numbers[] = {
  number0, number1, number2, number3, number4,
  number5, number6, number7, number8, number9
};

// ==== Instances
RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// ==== NRF24 variables
const byte address[6] = "c3lab";

// ==== Project variables
#define INCREMENT 0.01f
volatile uint8_t minutesValue = 0;
volatile uint8_t secondsValue = 0;
volatile uint8_t decimalValue = 0;

volatile bool countingEnabled = false;
volatile bool startEnabled = false;

// ============================================
void setup() {
	Serial.begin(115200);

	// ---- Config Pins
	pinMode(PIN_RESET_BUTTON, INPUT_PULLUP);
	pinMode(PIN_INFRA_SENSOR, INPUT);
	attachInterrupt(digitalPinToInterrupt(PIN_INFRA_SENSOR), stopTimer, RISING);

	// ---- Config NRF24
	LOGln(F("--- Config NRF24 ---"));
	DEBUG(radio.begin(), F("begin() OK!"), F("begin() FAIL!"));
	DEBUG(radio.setDataRate(RF24_2MBPS), F("setDataRate() OK!"), F("setDataRate() FAIL!"));
	radio.setPALevel(RF24_PA_LOW);
	radio.setAutoAck(true);
	radio.setPayloadSize(1);
	radio.openReadingPipe(0, address);
	radio.startListening();

	// ---- Config LCD
	lcd.begin(16, 2);
	lcd.clear();

	lcd.createChar(CUSTOM_A, a);
	lcd.createChar(CUSTOM_B, b);
	lcd.createChar(CUSTOM_C, c);
	lcd.createChar(CUSTOM_D, d);
	lcd.createChar(CUSTOM_E, e);
	lcd.createChar(CUSTOM_F, f);
	lcd.createChar(CUSTOM_G, g);
	lcd.createChar(CUSTOM_H, h);

	lcd.setCursor(5, 0);
	lcd.print("C3 LAB");
	lcd.setCursor(0, 1);
	lcd.print("Temporizador v.3");
	delay(5000);
	lcd.clear();

	lcd.setCursor(6, 0);
	lcd.print('.');
	lcd.setCursor(6, 1);
	lcd.print('.');

	// ---- Config Timer
	Timer1.initialize(INCREMENT*1E6);
	Timer1.attachInterrupt(timerInterrupt);
}

// ============================================

void loop() {

	// ---- Activate count start manually
	if (!digitalRead(PIN_RESET_BUTTON)) {
		minutesValue = secondsValue = decimalValue = 0;
		startEnabled = true;
		countingEnabled = false;
	}

	// ---- Checks available radio data
	if (radio.available()) {
		uint8_t data;
		radio.read(&data, sizeof(uint8_t));
		LOG("Received value: ");
		LOGln(data);

		if(data == 43){
			if(!countingEnabled && startEnabled){
				minutesValue = secondsValue = decimalValue = 0;
				countingEnabled = true;
				startEnabled = false;
			}
		}
	}

	// ---- Update data on the LCD
	display();
	updateStateInDisplay(startEnabled);
}

// ============================================

void stopTimer(){
	countingEnabled = false;
}

// ============================================

void timerInterrupt() {
  if(!countingEnabled)
    return;

  decimalValue++;
  secondsValue = decimalValue > 99 ? secondsValue+1 : secondsValue;
  minutesValue = secondsValue > 59 ? minutesValue+1 : minutesValue;

  if(decimalValue > 99) decimalValue = 0;
  if(secondsValue > 59) secondsValue = 0;
  if (minutesValue > 99) minutesValue = 0;
}

// ============================================

void updateStateInDisplay(bool isEnabled){
  if (isEnabled){
    lcd.setCursor(15, 0);
    lcd.print("\"");
  }else{
    lcd.setCursor(15, 0);
    lcd.print(" ");
  }
}

// ============================================

void display(){
  int digitValue[] = {
    decimalValue % 10,
    decimalValue / 10,
    secondsValue % 10,
    secondsValue / 10,
    minutesValue % 10,
    minutesValue / 10
  };

  lcd.setCursor(14, 1);
  lcd.print(digitValue[1]);
  lcd.setCursor(15, 1);
  lcd.print(digitValue[0]);
  numbers[digitValue[2]](10);
  numbers[digitValue[3]](7);
  numbers[digitValue[4]](3);
  numbers[digitValue[5]](0);
}

// ============================================

void number0(int col){
  lcd.setCursor(col, 0);
  lcd.write(CUSTOM_A);
  lcd.write(CUSTOM_F);
  lcd.write(CUSTOM_B);
  lcd.setCursor(col, 1);
  lcd.write(CUSTOM_C);
  lcd.write(CUSTOM_E);
  lcd.write(CUSTOM_D);
}

void number1(int col){
  lcd.setCursor(col, 0);
  lcd.write(CUSTOM_F);
  lcd.write(CUSTOM_B);
  lcd.print(' ');
  lcd.setCursor(col, 1);
  lcd.print(' ');
  lcd.write(CUSTOM_D);
  lcd.print(' ');
}

void number2(int col){
  lcd.setCursor(col, 0);
  lcd.write(CUSTOM_G);  
  lcd.write(CUSTOM_G);  
  lcd.write(CUSTOM_B);  
  lcd.setCursor(col, 1);  
  lcd.write(CUSTOM_C);  
  lcd.write(CUSTOM_H);
  lcd.write(CUSTOM_H);
}

void number3(int col){
  lcd.setCursor(col, 0);
  lcd.write(CUSTOM_G);
  lcd.write(CUSTOM_G);
  lcd.write(CUSTOM_B);
  lcd.setCursor(col, 1);
  lcd.write(CUSTOM_H);
  lcd.write(CUSTOM_H);
  lcd.write(CUSTOM_D);
}

void number4(int col){
  lcd.setCursor(col, 0);
  lcd.write(CUSTOM_C);
  lcd.write(CUSTOM_E);
  lcd.write(CUSTOM_B);
  lcd.setCursor(col, 1);
  lcd.print(' ');
  lcd.print(' ');
  lcd.write(CUSTOM_D);
}

void number5(int col){
  lcd.setCursor(col, 0);
  lcd.write(CUSTOM_A);
  lcd.write(CUSTOM_G);
  lcd.write(CUSTOM_G);
  lcd.setCursor(col, 1);
  lcd.write(CUSTOM_H);
  lcd.write(CUSTOM_H);
  lcd.write(CUSTOM_D);
}

void number6(int col){
  lcd.setCursor(col, 0);
  lcd.write(CUSTOM_A);
  lcd.write(CUSTOM_G);
  lcd.write(CUSTOM_G);
  lcd.setCursor(col, 1);
  lcd.write(CUSTOM_C);
  lcd.write(CUSTOM_H);
  lcd.write(CUSTOM_D);
}

void number7(int col){
  lcd.setCursor(col, 0);
  lcd.write(CUSTOM_F);
  lcd.write(CUSTOM_F);
  lcd.write(CUSTOM_B);
  lcd.setCursor(col, 1);
  lcd.print(' ');
  lcd.write(CUSTOM_A);
  lcd.print(' ');
}

void number8(int col){
  lcd.setCursor(col, 0);
  lcd.write(CUSTOM_A);
  lcd.write(CUSTOM_G);
  lcd.write(CUSTOM_B);
  lcd.setCursor(col, 1);
  lcd.write(CUSTOM_C);
  lcd.write(CUSTOM_H);
  lcd.write(CUSTOM_D);
}

void number9(int col){
  lcd.setCursor(col, 0);
  lcd.write(CUSTOM_A);
  lcd.write(CUSTOM_G);
  lcd.write(CUSTOM_B);
  lcd.setCursor(col, 1);
  lcd.print(' ');
  lcd.print(' ');
  lcd.write(CUSTOM_D);
}

