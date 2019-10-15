#include <kdn_RotaryEncoder.h>

#include "freertos/task.h"
#include "time.h"


TaskHandle_t taskHandleRE;
xQueueHandle Queue_DispREQ;

kdn_RotaryEncoder RE(32,33,25);
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf("setup() runs on core %d\n", xPortGetCoreID());

  xTaskCreatePinnedToCore(taskRE, "taskRE", 4096, NULL, 30, &taskHandleRE, 1);
}
 
void loop() {
   while (1) {
    delay(1);
  }
}
 
void taskRE(void *arg) {
  int ButtonFlag;
  int REpos = 0;
  while (1) {
    REpos += RE.CheckRotaryEncoder();
    ButtonFlag = RE.CheckButton();
    if (ButtonFlag==1){
      Serial.printf("ButtonPushed\n");
      Serial.printf("Position is  %d\n", REpos);
    }
    delay(1);
  }
}
