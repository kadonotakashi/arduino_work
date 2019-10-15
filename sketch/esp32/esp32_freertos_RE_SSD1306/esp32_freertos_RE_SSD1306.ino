#include "freertos/task.h"
#include "time.h"

//RotaryEncoder
#include "kdn_RotaryEncoder.h"
kdn_RotaryEncoder RE(32,33,25);
int RE_POSITION;
int ButtonFlag;

//Stepping Motor
#include "stepper28byj.h"


////////////////////////////////////////
//OLED
////////////////////////////////////////
//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128,64);
TaskHandle_t taskHandleStepper;
TaskHandle_t taskHandleOLED;
TaskHandle_t taskHandleRotaryEncoder;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf("setup() runs on core %d\n", xPortGetCoreID());
  RE_POSITION = 0;
  ButtonFlag = 0;
  xTaskCreatePinnedToCore(taskStepper, "taskStepper", 4096, NULL, 30, &taskHandleStepper, 0);
  xTaskCreatePinnedToCore(taskOLED, "taskOLED", 4096, NULL, 30, &taskHandleOLED, 1);
  xTaskCreatePinnedToCore(taskRotaryEncoder, "taskRotaryEncoder", 4096, NULL, 30, &taskHandleRotaryEncoder, 1);
}

void loop() {

}

void taskStepper(void *arg) {

  stepper28byj  STPR(19,5,18,17);
  static int Pos=0;
  int CurrentPos;
  while (1) {
    CurrentPos = STPR.getCurrentPos();
    Serial.printf("\n%d => %d", CurrentPos,CurrentPos+Pos);
    STPR.moveStep(Pos);
    Pos++;

    delay(100);
    CurrentPos = STPR.getCurrentPos();
    Serial.printf("\n%d => %d", CurrentPos,CurrentPos-Pos);
    STPR.moveStep(-Pos);
    Pos++;
    delay(100);
  }
}

void taskOLED(void *arg) {
  time_t now;

 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Show image buffer on the display hardware.
  display.display();
  delay(500);
  display.clearDisplay();
  display.display();

  struct tm tmNow;
  localtime_r(&now, &tmNow);
 // Serial.printf("%02d:%02d %s runs on core %d\n", tmNow.tm_min, tmNow.tm_sec, pcTaskGetTaskName(NULL), xPortGetCoreID());

  while (1) {
    delay(10);
  }
}

void taskRotaryEncoder(void *arg) {
  while (1) {
    RE_POSITION += RE.CheckRotaryEncoder();
    ButtonFlag = RE.CheckButton();
    if (ButtonFlag==1){
      Serial.printf("ButtonPushed\n");
      Serial.printf("Position is  %d\n", RE_POSITION);
    }
    delay(1);
  }
}


