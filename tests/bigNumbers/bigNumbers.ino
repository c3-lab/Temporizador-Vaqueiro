#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

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

void setup() {
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

}

void loop() {

  for (int i = 0; i < 10; ++i){
    numbers[i](0);
    delay(1000);
  }

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