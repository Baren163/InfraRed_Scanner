#include <Arduino_BuiltIn.h>


// #define arraySize 2000

// int timerStatus = 0;
// int doubleCheck = 3;
// int countDown = 50;
// volatile int sensorValue;
// volatile uint8_t voltage = 0;
// volatile int j = 0;
volatile float counter = 0;

// uint8_t infraArray[arraySize];

void timerInit() {
  TCCR1A = 0;

  OCR1A = 5000;

  TIMSK1 = (1 << OCIE1A);

  TCCR1B = (1 << WGM12) | (1 << CS10);
}

void setup() {

  pinMode(11, OUTPUT);

  Serial.begin(9600);
  while (!Serial) ;

  timerInit();

  sei();

}

ISR(TIMER1_COMPA_vect) {

  Serial.println(counter);
  // Serial.print("A");
  counter = 0;
  // sensorValue = analogRead(A0);
  // voltage = sensorValue * (5.0 / 1023.0);

  // if (j < arraySize) {
  //   infraArray[j] = voltage;
  //   j++;
  // }
  // Serial.print(voltage);
  // Serial.print(" ");

}

void loop() {
  digitalWrite(11, HIGH);
  counter += 0.01;
  // if (timerStatus == 0) {
  // counter += 0.01;
  //   sensorValue = analogRead(A0);
  //   voltage = sensorValue * (5.0 / 1023.0);


  //   if (voltage < 4) {
  //     timerStatus = 1;
  //     TCCR1B |= (1 << CS11);  // Start timer, prescaler 8
  //   }

  // }

  // if (number == 1) {

  //   if (voltage == 5) {
  //     countDown--;
  //   } else {
  //     countDown = 50;
  //   }

  //   if (countDown == 0) {
  //     TCCR1B &= ~(1 << CS10); // Stop timer
  //   }

  // }

  // if ((j == arraySize) && (doubleCheck == 3)) {

  //   TCCR1B &= ~(1 << CS11); // Stop timer
  //   doubleCheck = 1;

  //   for (int k = 0; k < arraySize; k++) {
  //     Serial.print(infraArray[k]);
  //     Serial.print(" ");
  //   }

  // }

}

  // // print out the value you read:
  // Serial.print(voltage);
  // Serial.print(" ");

  // if ((voltage < 5) && (coolDown == 0)) {

  //   if (doubleCheck == 0) {

  //     coolDown = 2000;

  //     if (digitalRead(12)) {
  //       digitalWrite(12, LOW);
  //     } else {
  //       digitalWrite(12, HIGH);
  //     }

  //     doubleCheck = 3;

  //   } else {
  //     doubleCheck--;
  //   }

  // }

  // if (coolDown > 0) {
  //   coolDown--;
  // }


  // digitalWrite(11, HIGH);


