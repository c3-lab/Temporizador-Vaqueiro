#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <LiquidCrystal.h>

#define LCD_RW    "GND"
#define LCD_RS    A0
#define LCD_EN    A1
#define LCD_D4    A2
#define LCD_D5    A3
#define LCD_D6    A4
#define LCD_D7    A5

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

RF24 radio(8, 9);

const byte address[6] = "c3lab";

uint8_t counter = 0; 

void setup() {
  Serial.begin(115200);

  lcd.begin(16, 2);
  lcd.clear();

  Serial.println(radio.begin() ? "begin() OK!" : "begin() FALHOU!");
  Serial.println(radio.setDataRate(RF24_2MBPS) ? "setDataRate() OK!" : "setDataRate() FALHOU!");
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(true);
  radio.setPayloadSize(1);

  radio.openReadingPipe(0, address);
  radio.startListening();

}

void loop() {
  if (radio.available()) {
    radio.read(&counter, sizeof(uint8_t));
    Serial.println(counter);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(counter);
  }
}
