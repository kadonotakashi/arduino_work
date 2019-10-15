//volatile int interruptCounter;
volatile int RE_POSITION;
volatile int ButtonFlag;
int totalInterruptCounter;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
//  portENTER_CRITICAL_ISR(&timerMux);
  RE_POSITION += CheckRotaryEncoder();
//  portEXIT_CRITICAL_ISR(&timerMux);
}


//Rotary Encoder
#define EncoderA 32
#define EncoderB 33
#define EncoderButton 25

//call from timer interrupt
int CheckRotaryEncoder() {
  static uint8_t  crntRESTS;
  static uint8_t  prevRESTS;
  static uint8_t  validflag;

  prevRESTS = crntRESTS;
  crntRESTS = digitalRead(EncoderA) << 1 | digitalRead(EncoderB);

  if (crntRESTS == 0x00) {
    validflag = 1;
  }

  if ((prevRESTS == 0x01 ) && (crntRESTS == 0x03) && (validflag == 1)) {
    validflag = 0;
    return -1;
  }
  else if ((prevRESTS == 0x02 ) && (crntRESTS == 0x03) && (validflag == 1)) {
    validflag = 0;
    return 1;
  }
  else {
    return 0;
  }
}

void ButtonPush() {
  ButtonFlag = 1;
}


void setup() {
  pinMode(EncoderA, INPUT_PULLUP);
  pinMode(EncoderB, INPUT_PULLUP);
  pinMode(EncoderButton, INPUT_PULLUP);

  Serial.begin(115200);
  RE_POSITION = 0;
  ButtonFlag = 0;
  timer = timerBegin(0, 80, true);  //1us
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);//1ms
  timerAlarmEnable(timer);

  attachInterrupt(EncoderButton, ButtonPush, RISING);
}

void loop() {
  static int prevPosition;
  if (prevPosition != RE_POSITION) {
    Serial.print("Rotary Encoder Position is: ");
    Serial.println(RE_POSITION);
    prevPosition = RE_POSITION;
  }
  if (ButtonFlag != 0) {
    Serial.println("Button Pusshed");
    ButtonFlag = 0;
  }

  delay(10);
}

