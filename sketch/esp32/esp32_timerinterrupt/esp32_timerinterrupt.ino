int CheckRotaryEncoder();

volatile int interruptCounter;
volatile int RE_POSITION;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  RE_POSITION += CheckRotaryEncoder();
  portEXIT_CRITICAL_ISR(&timerMux);
}


//Rotary Encoder
const int EncoderA = 32;
const int EncoderB = 33;

int CheckRotaryEncoder(){
  static uint8_t  crntRESTS;
  static uint8_t  prevRESTS;
  static uint8_t  validflag;

  prevRESTS = crntRESTS;

  crntRESTS = digitalRead(EncoderA)<<1 | digitalRead(EncoderB);

  if (crntRESTS == 0x00){
    validflag=1;
  }


  
  if ((prevRESTS == 0x01 ) && (crntRESTS == 0x03) && (validflag==1)){
    validflag=0;
    return -1;
  }
  else if ((prevRESTS == 0x02 ) && (crntRESTS == 0x03) && (validflag==1)){
    validflag=0;
    return 1;
  }
  else{
    return 0;
  }
}

 
void setup() {
  pinMode(EncoderA, INPUT_PULLUP);
  pinMode(EncoderB, INPUT_PULLUP);

  Serial.begin(115200);
  RE_POSITION = 0;

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
 
}
 
void loop() {
    Serial.print("Rotary Encoder Position is: ");
    Serial.println(RE_POSITION);
    delay(500);
}

