#include <TimerOne.h>

// --- Hardware map
#define SEG_A 13
#define SEG_B 12
#define SEG_C 11
#define SEG_D 10
#define SEG_E 9
#define SEG_F 8
#define SEG_G 7
#define SEG_DP 6

#define DIG_1 A2
#define DIG_2 A0
#define DIG_3 A3
#define DIG_4 A1
#define DIG_5 A4

#define BTN_RESET 4
#define BTN_INIT_TIMER 2
#define BTN_STOP_TIMER 3

// Settings
#define INCREMENT 0.01f
#define DIGITS 5

const int digitsPins[DIGITS] = {DIG_1, DIG_2, DIG_3, DIG_4, DIG_5};
const int segmentsPins[8] = {SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_DP};
const bool decimalPointStates[DIGITS] = {1, 0, 1, 0, 0};
const bool numbers[10][7] = {
// a, b, c, d, e, f, g
  {1, 1, 1, 1, 1, 1, 0},    // 0
  {0, 1, 1, 0, 0, 0, 0},    // 1
  {1, 1, 0, 1, 1, 0, 1},    // 2
  {1, 1, 1, 1, 0, 0, 1},    // 3
  {0, 1, 1, 0, 0, 1, 1},    // 4
  {1, 0, 1, 1, 0, 1, 1},    // 5
  {1, 0, 1, 1, 1, 1, 1},    // 6
  {1, 1, 1, 0, 0, 0, 0},    // 7
  {1, 1, 1, 1, 1, 1, 1},    // 8
  {1, 1, 1, 1, 0, 1, 1}     // 9
};

volatile int minutesValue = 0;
volatile int secondsValue = 0;
volatile int decimalValue = 0;

volatile bool countingEnabled = false;
volatile bool startEnabled = false;


void setup() {

  // Setup output pins
  for (int i = 0; i < DIGITS; i++) pinMode(digitsPins[i], OUTPUT);
  for (int i = 0; i < 8; i++) pinMode(segmentsPins[i], OUTPUT);

  // Setup input pins
  pinMode(BTN_RESET, INPUT_PULLUP);

  pinMode(BTN_INIT_TIMER, INPUT_PULLUP);
  pinMode(BTN_STOP_TIMER, INPUT);
  attachInterrupt(digitalPinToInterrupt(BTN_INIT_TIMER), initTimer, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_STOP_TIMER), stopTimer, FALLING);

  // Set Timer1 every 'INCREMENT' seconds
  Timer1.initialize(INCREMENT*1E6);
  Timer1.attachInterrupt(timerInterrupt);

}

void loop() {

  // Activate the start of the count manually
  if (!digitalRead(BTN_RESET)) {
    minutesValue = secondsValue = decimalValue = 0;
    startEnabled = true;
    countingEnabled = false;
  }


  display();

}

void timerInterrupt() {
  if(!countingEnabled)
    return;

  decimalValue++;
  secondsValue = decimalValue > 99 ? secondsValue+1 : secondsValue;
  minutesValue = secondsValue > 59 ? minutesValue+1 : minutesValue;

  if(decimalValue > 99) decimalValue = 0;
  if(secondsValue > 59) secondsValue = 0;
  if (minutesValue > 9) minutesValue = 0;
}

void display(){

  // Calculate the value of each digit
  int digitValue[DIGITS] = {
    minutesValue,
    secondsValue / 10,
    secondsValue % 10,
    decimalValue / 10,
    decimalValue % 10
  };

  // Loop to display time
  for (int dig = 0; dig < DIGITS; dig++) {
    
    // Turn on the decimal point
    digitalWrite(SEG_DP, decimalPointStates[dig]);

    // Turn on current digit
    digitalWrite(digitsPins[dig], 1);

    // Turn on the appropriate segments
    for (int seg = 0; seg < 7; seg++) {
      digitalWrite(segmentsPins[seg], numbers[digitValue[dig]][seg]);
    }

    // Turn off all segments
    for (int seg = 0; seg < 8; seg++) {
      digitalWrite(segmentsPins[seg],  0);
    }

    // Turn off current digit
    digitalWrite(digitsPins[dig], 0);
  }
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