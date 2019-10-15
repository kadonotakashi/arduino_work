#include <kdn9341.h>

//ESP32
const uint8_t SCLK_OLED =  18; //SCLK
const uint8_t MOSI_OLED =  23; //MOSI (Master Output Slave Input)
const uint8_t MISO_OLED =  19; //これは実際は使っていない。MISO (Master Input Slave Output)
const uint8_t CS_OLED = 5;
const uint8_t DC_OLED =  16; //OLED DC(Data/Command)
const uint8_t RST_OLED =  4; //OLED Reset


kdn9341 TFT;

void setup() {
  int i;
  uint8_t x;

  Serial.begin(115200);  
  TFT.Init9341( MISO_OLED,MOSI_OLED,SCLK_OLED,DC_OLED, RST_OLED, CS_OLED);
  TFT.ClearScreen(RGB565_BLACK);
//  TFT.DrawRectangleFill(0, 0, 95, 63, RGB565_YELLOW, RGB565_BLACK);
  TFT.DrawRectangle(4, 4, 91, 60, RGB565_CYAN);
//  TFT.DrawLine(4, 4, 91, 60, RGB565_BLUE);
  TFT.DrawPix(1, 1, RGB565_RED);

}

void loop() {
  FontList flist;
  
  delay(1000);
  TFT.GetFontInfo(0,&flist);
  TFT.DrawRectangle(0, 0, 95, 63, RGB565_BLACK);
  TFT.PutStringAdafruit(5,20,flist.FontName,RGB565_WHITE,0);

  delay(1000);
  TFT.GetFontInfo(1,&flist);
  TFT.DrawRectangle(0, 0, 95, 63, RGB565_WHITE);
  TFT.PutStringAdafruit(5,20,flist.FontName,RGB565_BLACK,1);

  delay(1000);
  TFT.GetFontInfo(2,&flist);
  TFT.DrawRectangle(0, 0, 95, 63, RGB565_RED);
  TFT.PutStringAdafruit(5,20,flist.FontName,RGB565_GREEN,2);
  
  delay(1000);
  TFT.GetFontInfo(3,&flist);
  TFT.DrawRectangle(0, 0, 95, 63,RGB565_BLUE);
  TFT.PutStringAdafruit(5,20,flist.FontName,RGB565_YELLOW,3);

  delay(1000);
  TFT.GetFontInfo(4,&flist);
  TFT.DrawRectangle(0, 0, 95, 63, RGB565_RED);
  TFT.PutStringAdafruit(5,20,flist.FontName,RGB565_CYAN,4);
}
