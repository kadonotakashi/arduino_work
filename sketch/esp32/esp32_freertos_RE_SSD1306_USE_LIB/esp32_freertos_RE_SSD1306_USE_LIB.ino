#include <gfxfont.h>
#include <kdn1331.h>

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

QueueHandle_t QueueOLED;


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

void loop(){

}

void taskStepper(void *arg) {

  stepper28byj  STPR(19,5,18,17);
  static int Pos=0;
  int CurrentPos;
  while (1) {
    CurrentPos = STPR.getCurrentPos();
//    Serial.printf("\n%d => %d", CurrentPos,CurrentPos+Pos);
    STPR.moveStep(Pos);
    Pos++;

    delay(100);
    CurrentPos = STPR.getCurrentPos();
//    Serial.printf("\n%d => %d", CurrentPos,CurrentPos-Pos);
    STPR.moveStep(-Pos);
    Pos++;
    delay(100);
  }
}

void taskOLED(void *arg) {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  // Show image buffer on the display hardware.
//  display.display();
//  delay(1000);
  display.clearDisplay();
//display.display();

  display.fillRect(0,0,127,8,WHITE);
  display.fillRect(0,32,127,8,WHITE);

  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(8,0);
  display.print("Encoder Position");
  display.setCursor(8,32);
  display.print("Stepper Position");
  display.display();


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


