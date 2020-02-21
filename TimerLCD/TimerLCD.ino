#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <PinChangeInterrupt.h>

//----------- HARDWARE MAP
#define PIN_RESET 5
#define PIN_INIT_TIMER 19 //A5

#define PIN_US_TRIGGER 4
#define PORTREG_US_TRIGGER PORTD
#define BIT_US_TRIGGER PORTD4

#define PIN_US_ECHO 3
#define PINREG_US_ECHO PIND
#define BIT_US_ECHO PIND3

#define LCD_RW    "GND"
#define LCD_RS    8
#define LCD_EN    9
#define LCD_D4    10
#define LCD_D5    11
#define LCD_D6    12
#define LCD_D7    13

#define INCREMENT 0.01f

//----------- LCD Definitions
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

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

//----------- Counter variables
volatile uint8_t minutesValue = 0;
volatile uint8_t secondsValue = 0;
volatile uint8_t decimalValue = 0;

volatile bool countingEnabled = false;
volatile bool startEnabled = false;

//----------- Ultrasonic variables
const uint8_t DISTANCE_THRESHOLD = 80;
volatile unsigned long ultrasonicStartTime;
volatile unsigned long ultrasonicDistance;
volatile bool triggerEnabled = true;

volatile uint8_t timerPrescaler = 0;


void setup() {

  // Serial.begin(9600);

  // --- Initializes pins
  pinMode(PIN_RESET, INPUT_PULLUP);
  pinMode(PIN_INIT_TIMER, INPUT);

  pinMode(PIN_US_TRIGGER, OUTPUT);
  digitalWrite(PIN_US_TRIGGER, LOW);
  pinMode(PIN_US_ECHO, INPUT);

  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(PIN_INIT_TIMER), initTimer, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_US_ECHO), readUltrasonic, CHANGE);

  // --- LCD Initialize
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
  lcd.print("Temporizador v.1");
  delay(5000);
  lcd.clear();

  lcd.setCursor(6, 0);
  lcd.print('.');
  lcd.setCursor(6, 1);
  lcd.print('.');

  // --- Set Timer1 every 'INCREMENT' seconds
  Timer1.initialize(INCREMENT*1E6);
  Timer1.attachInterrupt(timerInterrupt);

  // --- Timer2 setup
  TCCR2A = 0x00;    // Timer2 in normal mode
  TCCR2B = 0x07;    // Prescaler 1:1024
  TCNT2 = 61;       // 12480us of overflow -> (256 - TCNT2) * Presc * 62.5E-9
  TIMSK2 = 0x01;    // Enables interruption of Timer2

}


void loop() {

  // Activate the start of the count manually
  if (!digitalRead(PIN_RESET)) {
    minutesValue = secondsValue = decimalValue = 0;
    startEnabled = true;
    countingEnabled = false;
  }

  // Serial.println(ultrasonicDistance);

  display();
  updateStateInDisplay(startEnabled);

}


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

void initTimer(){
  if(!countingEnabled && startEnabled){
    minutesValue = secondsValue = decimalValue = 0;
    countingEnabled = true;
    startEnabled = false;
  }
}

void stopTimer(){
  countingEnabled = false;
}

ISR(TIMER2_OVF_vect){
  TCNT2 = 61;

  timerPrescaler++;

  if(timerPrescaler >= 2){
    if(triggerEnabled){
      triggerUltrasonic();
      timerPrescaler = 0;
      triggerEnabled = false;
    }

  }

}

void triggerUltrasonic(){
  bitSet(PORTREG_US_TRIGGER, BIT_US_TRIGGER);
  delayMicroseconds(10);
  bitClear(PORTREG_US_TRIGGER, BIT_US_TRIGGER);
}

void readUltrasonic(){
  unsigned long now = micros();

  if (bitRead(PINREG_US_ECHO, BIT_US_ECHO)){
    ultrasonicStartTime = now;
  }else{
    ultrasonicDistance = (now - ultrasonicStartTime) / 58;
    // ultrasonicDistance = constrain(ultrasonicDistance, 0, 200);
    
    // Serial.println(ultrasonicDistance);

    if (ultrasonicDistance <= DISTANCE_THRESHOLD && countingEnabled)
      stopTimer();

    triggerEnabled = true;
  }
}


void updateStateInDisplay(bool isEnabled){
  if (isEnabled){
    lcd.setCursor(15, 0);
    lcd.print("\"");
  }else{
    lcd.setCursor(15, 0);
    lcd.print(" ");
  }
}

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
