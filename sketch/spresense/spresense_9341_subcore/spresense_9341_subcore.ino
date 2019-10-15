#include <MP.h>
#include "glcd_command.h"
#define LCD_SUBCORE 1

#define BUFSIZE 2*320*240 //150 kbyte



#if (SUBCORE == LCD_SUBCORE)
#include <kdn9341.h>

const uint8_t CS_LCD = 7;
const uint8_t DC_LCD =  9; //OLED DC(Data/Command)
const uint8_t RST_LCD =  8; //OLED Reset

kdn9341 TFT;
void setup() {
  FontList flist;
//  TFT.Init9341( DC_LCD, RST_LCD, CS_LCD);
//  TFT.DrawRectangle(0, 0, 319, 239, RGB565_BLACK);
  MP.begin();
//  MP.RecvTimeout(MP_RECV_BLOCKING);
  TFT.Init9341( DC_LCD, RST_LCD, CS_LCD);
  TFT.DrawRectangle(0, 0, 319, 239, RGB565_BLUE);
  

}

void loop() {

  GLCD_CMD  RecvCmd;
  int8_t msgid;
  void *addr;

  int      ret;
  uint32_t msgdata;

  /* Echo back */

  ret = MP.Recv(&msgid, &RecvCmd);
//  printf("command recieved\n");
  if (ret < 0) {
    errorLoop(3);
  }
  else{
    printf(RecvCmd.CMD);
  }

  ret = MP.Send(&msgid, &RecvCmd);
  if (ret < 0) {
    errorLoop(4);
  }
}

void errorLoop(int num)
{
  int i;

  while (1) {
    for (i = 0; i < num; i++) {
      ledOn(LED0);
      delay(300);
      ledOff(LED0);
      delay(300);
    }
    delay(1000);
  }
}

#endif



//
//動作確認用のmain core
//
//
#ifndef SUBCORE

int subcore = LCD_SUBCORE; /* Communication with SubCore1 */

void setup()
{
  int usedMem, freeMem, largestFreeMem;
  GLCD_CMD  SendCmd;
  int ret = 0;

  Serial.begin(115200);
  while (!Serial);

  /* Launch SubCore1 */
  ret = MP.begin(subcore);
  if (ret < 0) {
    printf("MP.begin error = %d\n", ret);
  }

  delay(100);
  uint8_t *RGB565BUF = (uint8_t *)MP.AllocSharedMemory(BUFSIZE);

  MP.GetMemoryInfo(usedMem, freeMem, largestFreeMem);
  printf("Used:%4d [KB] / Free:%4d [KB] (Largest:%4d [KB])\n",
         usedMem / 1024, freeMem / 1024, largestFreeMem / 1024);

  uint8_t sndid = LCD_SUBCORE;
  SendCmd.CMD = GLCDCMD_INIT;
  ret = MP.Send(sndid, &SendCmd, (int)LCD_SUBCORE);
  if (ret < 0) {
    printf("MP.Send error = %d\n", ret);
  }
  else{
    printf("MP.Send complete\n", ret);
  }
}

void loop()
{
//  printf("main core running\n");
  delay(1000);
}


#endif;
