//----------- HARDWARE MAP
#define PIN_INIT_TIMER 5
#define PORTREG_INIT_TIMER PORTD
#define BIT_INIT_TIMER PORTD5

#define PIN_US_TRIGGER 4
#define PORTREG_US_TRIGGER PORTD
#define BIT_US_TRIGGER PORTD4

#define PIN_US_ECHO 3
#define PINREG_US_ECHO PIND
#define BIT_US_ECHO PIND3

//----------- Ultrasonic variables
const int DISTANCE_THRESHOLD = 150;
volatile unsigned long ultrasonicStartTime;
volatile unsigned long ultrasonicDistance;
volatile bool detectedObject = false;


void setup() {

  // Serial.begin(9600);

  // --- Initializes pins
  pinMode(PIN_INIT_TIMER, OUTPUT);
  digitalWrite(PIN_INIT_TIMER, LOW);

  pinMode(PIN_US_TRIGGER, OUTPUT);
  digitalWrite(PIN_US_TRIGGER, LOW);
  pinMode(PIN_US_ECHO, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_US_ECHO), readUltrasonic, CHANGE);

  // --- Timer2 setup
  TCCR2A = 0x00;    // Timer2 in normal mode
  TCCR2B = 0x07;    // Prescaler 1:1024
  TCNT2 = 0;        // 16384us of overflow -> (256 - TCNT2) * Presc * 62.5E-9
  TIMSK2 = 0x01;    // Enables interruption of Timer2
}


void loop() {

  // Serial.println(ultrasonicDistance);

  if(detectedObject){
    bitSet(PORTREG_INIT_TIMER, BIT_INIT_TIMER);
    delay(100);
    bitClear(PORTREG_INIT_TIMER, BIT_INIT_TIMER);

    detectedObject = false;
  }

}


ISR(TIMER2_OVF_vect){
  TCNT2 = 0;

  if (!detectedObject)
    triggerUltrasonic();
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
    ultrasonicDistance = (now - ultrasonicStartTime) / 58.309;
    ultrasonicDistance = constrain(ultrasonicDistance, 0, 300);

    if (ultrasonicDistance <= DISTANCE_THRESHOLD)
      detectedObject = true;

  }
}