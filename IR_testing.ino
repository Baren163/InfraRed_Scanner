
#define arraySize 500
#define OCR1A_val 2000

int timerStatus = 0;
int doubleCheck = 3;
int countDown = 50;
volatile int sensorValue;
volatile uint8_t voltage = 0;
volatile int j = 0;
volatile float counter = 0;
uint8_t dataArray[48];
uint8_t infraArray[arraySize];


void convertSignalToData(uint8_t signalArray[500]) {


  uint8_t p = 0;

  for (int i = 1; i < 500; i++) {

    if (signalArray[i] == 1) {
      numOnes++;
    } else {
      numZeros++
    }

    if (signalArray[i] != signalArray[i - 1]) {
      numLogicChange++;
    }

    if ((numLogicChange == 2) && (signalArray[i] != signalArray[i + 1])) {
      if ((numOnes - numZeros) > 3) {
        dataArray[p] = 1;
        p++;
      } else {
        dataArrat[p] = 0;
        p++;
      }

      numLogicChange = 0;
      numOnes = 0;
      numZeros = 0;

    }

  }

}

void timerInit() {
  TCCR1A = 0;

  OCR1A = OCR1A_val;

  TIMSK1 = (1 << OCIE1A);

  TCCR1B = (1 << WGM12);
}

void setup() {

  pinMode(11, OUTPUT);
  pinMode(10, INPUT);

  Serial.begin(9600);
  while (!Serial) ;

  timerInit();

  sei();

}

ISR(TIMER1_COMPA_vect) {

  voltage = digitalRead(10);
  // voltage = sensorValue * (5.0 / 1023.0);

  if (j < arraySize) {
    infraArray[j] = voltage;
    j++;
  }

}

void loop() {
  if (timerStatus == 0) {

    voltage = digitalRead(10);
    // voltage = sensorValue * (5.0 / 1023.0);

    if (voltage == 0) {
      timerStatus = 1;
      OCR1A = OCR1A_val;
      TCCR1B = (1 << WGM12) | (1 << CS10);  // Start timer
    }

  }

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

  if ((j == arraySize) && (doubleCheck == 3)) {

    TCCR1B &= ~(1 << CS10); // Stop timer
    doubleCheck = 1;

    for (int k = 0; k < arraySize; k++) {
      Serial.print(infraArray[k]);
      Serial.print(" ");
    }

  }

}
