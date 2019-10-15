#include <kdn1331.h>

//SPRESENSE
//SCLK =  13; //SCLK
//MOSI =  11; //MOSI (Master Output Slave Input)
//MISO =  12; //これは実際は使っていない。MISO (Master Input Slave Output)
//CS  = 10;

const uint8_t DC_OLED =  9; //OLED DC(Data/Command)
const uint8_t RST_OLED =  8; //OLED Reset


kdn1331 OLED;

void setup() {

  Serial.begin(115200);  
//  OLED.Init1331( MISO_OLED,MOSI_OLED,SCLK_OLED,DC_OLED, RST_OLED, CS_OLED);
  OLED.Init1331( DC_OLED, RST_OLED);
  OLED.ClearScreen();
  OLED.DrawRectangleFill(0, 0, 95, 63, RGB565_YELLOW, RGB565_BLACK);
  OLED.DrawRectangle(4, 4, 91, 60, RGB565_CYAN);
  OLED.DrawLine(4, 4, 91, 60, RGB565_BLUE);
  OLED.DrawPix(1, 1, RGB565_RED);

}

void loop() {
  FontList flist;
  
  delay(1000);
  OLED.GetFontInfo(0,&flist);
  OLED.DrawRectangleFill(0, 0, 95, 63, RGB565_YELLOW, RGB565_BLACK);
  OLED.PutStringAdafruit(5,20,flist.FontName,RGB565_WHITE,0);

  delay(1000);
  OLED.GetFontInfo(1,&flist);
  OLED.DrawRectangleFill(0, 0, 95, 63, RGB565_CYAN, RGB565_WHITE);
  OLED.PutStringAdafruit(5,20,flist.FontName,RGB565_BLACK,1);

  delay(1000);
  OLED.GetFontInfo(2,&flist);
  OLED.DrawRectangleFill(0, 0, 95, 63, RGB565_GREEN, RGB565_RED);
  OLED.PutStringAdafruit(5,20,flist.FontName,RGB565_GREEN,2);
  
  delay(1000);
  OLED.GetFontInfo(3,&flist);
  OLED.DrawRectangleFill(0, 0, 95, 63, RGB565_YELLOW, RGB565_BLUE);
  OLED.PutStringAdafruit(5,20,flist.FontName,RGB565_YELLOW,3);

  delay(1000);
  OLED.GetFontInfo(4,&flist);
  OLED.DrawRectangleFill(0, 0, 95, 63, RGB565_YELLOW, RGB565_RED);
  OLED.PutStringAdafruit(5,20,flist.FontName,RGB565_CYAN,4);
}
