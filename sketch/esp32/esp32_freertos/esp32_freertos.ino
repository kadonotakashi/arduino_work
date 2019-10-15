#include "freertos.h"
#include "freertos/task.h"
#include "time.h"
 
TaskHandle_t taskHandle3;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf("setup() runs on core %d\n", xPortGetCoreID());
 
  xTaskCreate(task, "task1", 4096, NULL, 10, NULL);
  xTaskCreatePinnedToCore(task, "task3", 4096, NULL, 30, &taskHandle3, 1);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  delay(2500);
  vTaskSuspend(taskHandle3);
 
  delay(3000);
  vTaskResume(taskHandle3);
 
  while (1) {
    delay(1000);
  }
}
 
void task(void *arg) {
  time_t now;
  struct tm tmNow;
 
  while (1) {
    time(&now);
    localtime_r(&now, &tmNow);
 
    Serial.printf("%02d:%02d %s runs on core %d\n", tmNow.tm_min, tmNow.tm_sec, pcTaskGetTaskName(NULL), xPortGetCoreID());
    delay(1000);
  }
}
