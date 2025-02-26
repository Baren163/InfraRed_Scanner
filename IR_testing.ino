
#define arraySize 500
#define OCR1A_val 2000

int timerStatus = 0;
int doubleCheck = 3;
volatile uint8_t voltage = 0;
volatile int j = 0;
uint8_t startOfTransmission = 0;
int endOfTransmission = 0;
uint8_t dataArray[48];  // 32 address bits, 16 data bits
uint8_t infraArray[arraySize];

bool addressComparison() {

  uint32_t myAddress = 1074004224;  // 32 bit address, 01000000000001000000000100000000

  for (int i = 0; i < 32; i++) {
    if (dataArray[i] != ((myAddress >> (31 - i)) & 1)) {
      return false;
    }
  }

  return true;
}

void formatInfraArray() {

  uint8_t leadingLogicChangeNum = 0;

  infraArray[0] = 0;
  infraArray[1] = 0;

  for (int i = 0; i < 500; i++) {

    if (infraArray[i] != infraArray[i - 1]) {
      leadingLogicChangeNum++;

      if (leadingLogicChangeNum == 2) {

        startOfTransmission = i;
        break;

      }

    }

  }

  for (int i = 498; i > 0; i--) {
    if (infraArray[i] != infraArray[i + 1]) {
      endOfTransmission = i;
      break;
    }
  }

}

void convertSignalToData() {

  formatInfraArray();

  uint8_t numLogicChange = 0;
  uint8_t numZeros = 0;
  uint8_t numOnes = 0;
  uint8_t p = 0;

  for (int i = startOfTransmission; i < endOfTransmission; i++) {

    if ((infraArray[i] != infraArray[i - 1]) && (i > startOfTransmission)) {
      numLogicChange++;
    }

    if (numLogicChange == 2) {
      if ((numOnes - numZeros) > 3) {
        dataArray[p] = 1;
        p++;
      } else {
        dataArray[p] = 0;
        p++;
      }

      numLogicChange = 0;
      numOnes = 0;
      numZeros = 0;

    }

    if (infraArray[i] == 1) {
      numOnes++;
    } else {
      numZeros++;
    }

  }

}

void printDataArray() {
  Serial.println(" ");
  for (int i = 0; i < 70; i++) {
    Serial.print(dataArray[i]);
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

  if (j < arraySize) {
    infraArray[j] = voltage;
    j++;
  }

}

void loop() {
  if (timerStatus == 0) {

    voltage = digitalRead(10);

    if (voltage == 0) {
      timerStatus = 1;
      OCR1A = OCR1A_val;
      TCCR1B = (1 << WGM12) | (1 << CS10);  // Start timer
    }

  }


  if ((j == arraySize) && (doubleCheck == 3)) {

    TCCR1B &= ~(1 << CS10); // Stop timer
    doubleCheck = 1;

    convertSignalToData();

    // printDataArray();

    if (addressComparison()) {
      Serial.println("Address matches");
    } else {
      Serial.println("No address match");
    }


    // for (int k = 0; k < arraySize; k++) {
    //   Serial.print(infraArray[k]);
    //   Serial.print(" ");
    // }

  }

}
