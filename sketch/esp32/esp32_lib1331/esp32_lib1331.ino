#include "ESP32_SSD1331.h"
 
const uint8_t SCLK_OLED =  18; //SCLK
const uint8_t MOSI_OLED =  23; //MOSI (Master Output Slave Input)
const uint8_t MISO_OLED =  19; //これは実際は使っていない。MISO (Master Input Slave Output)
const uint8_t CS_OLED = 5;
const uint8_t DC_OLED =  16; //OLED DC(Data/Command)
const uint8_t RST_OLED =  4; //OLED Reset
  
ESP32_SSD1331 ssd1331(SCLK_OLED, MISO_OLED, MOSI_OLED, CS_OLED, DC_OLED, RST_OLED);
 
void setup() {
  ssd1331.SSD1331_Init();
}
  
void loop() {
  int i, j;
  uint8_t R, G, B, Dot1, Dot2;
  
  ssd1331.Display_Clear(0, 0, 95, 63);
  
  ssd1331.CommandWrite(0xAE); //Set Display Off
  delay(1000);
  
  ssd1331.CommandWrite(0xA0); //Remap & Color Depth setting　
    ssd1331.CommandWrite(0b00110010); //A[7:6] = 00; 256 color.
  
  R=7; G=0; B=0; //256 color : R (0-7), G (0-7), B (0-3)
  Dot1 = (R << 5) | (G << 2) | B;
  for(j=0; j<64; j++){
    for(i=0; i<96; i++){
      ssd1331.DataWrite(Dot1);
    }
  }
  
  ssd1331.CommandWrite(0xAF); //Set Display On
  delay(110); //0xAFコマンド後最低100ms必要
  ssd1331.Brightness_FadeIn(4);
  delay(1000);
  ssd1331.Brightness_FadeOut(4);
  delay(1000);
  ssd1331.CommandWrite(0xAE); //Set Display Off
  delay(1000);
  
  ssd1331.Brightness_FadeIn(0);
  ssd1331.Display_Clear(0, 0, 95, 63);
  ssd1331.CommandWrite(0xAF); //Set Display On
  delay(110); //0xAFコマンド後最低100ms必要
  R=0; G=7; B=0; //256 color : R (0-7), G (0-7), B (0-3)
  Dot1 = (R << 5) | (G << 2) | B;
  for(j=0; j<64; j++){
    for(i=0; i<96; i++){
      ssd1331.DataWrite(Dot1);
    }
  }
  delay(2000);
  
  R=0; G=0; B=3; //256 color : R (0-7), G (0-7), B (0-3)
  Dot1 = (R << 5) | (G << 2) | B;
  for(j=0; j<64; j++){
    for(i=0; i<96; i++){
      ssd1331.DataWrite(Dot1);
    }
  }
  delay(2000);
  
  ssd1331.Display_Clear(0, 0, 95, 63);
  
  ssd1331.CommandWrite(0xA0); //Remap & Color Depth setting　
    ssd1331.CommandWrite(0b01110010); //A[7:6] = 01; 65k color format
  
  R=31; G=63; B=31; //65k color : R (0-31), G (0-63), B (0-31)
  uint8_t DotDot[2];
  DotDot[0] = (R << 3) | (G >> 3);
  DotDot[1] = (G << 5) | B;
  for(j=0; j<64; j++){
    for(i=0; i<96; i++){
      ssd1331.DataWriteBytes(DotDot, 2); //65k colorモードでは、２バイトデータを送る
    }
  }
  delay(2000);
  
  ssd1331.Display_Clear(0, 0, 95, 63);
  ssd1331.Drawing_Line(0, 0, 95, 63, 31, 0, 0); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  
  ssd1331.Drawing_Line(95, 0, 0, 63, 0, 31, 0); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  
  ssd1331.Drawing_Line(48, 0, 48, 63, 0, 0, 31); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  
  for(i=0; i<63; i=i+5){
    ssd1331.Drawing_Line(i, 63, 95, 63-i, 0, i, 31);
  }
  delay(2000);
  
  ssd1331.Display_Clear(0, 0, 95, 63);
  ssd1331.Drawing_Rectangle_Line(20, 20, 40, 40, 31, 0, 0); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  
  ssd1331.Drawing_Rectangle_Line(0, 0, 60, 60, 0, 31, 0); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  
  ssd1331.Drawing_Rectangle_Line(70, 10, 80, 63, 0, 0, 31); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  
  ssd1331.Display_Clear(0, 0, 95, 63);
  ssd1331.Drawing_Rectangle_Fill(0, 0, 60, 60, 0, 31, 0, 31, 0, 0); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  
  ssd1331.Drawing_Rectangle_Fill(20, 20, 40, 40, 0, 0, 31, 0, 31, 0); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  
  ssd1331.Drawing_Rectangle_Fill(70, 10, 80, 63, 31, 63, 31, 0, 0, 31); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  
  ssd1331.Display_Clear(0, 0, 95, 63);
 
  ssd1331.CommandWrite(0xA0); //Remap & Color Depth setting　
    ssd1331.CommandWrite(0b00110010); //A[7:6] = 00; 256 color.
  
  ssd1331.Drawing_Circle_Line_256color(31, 31, 31, 7, 0, 0); //Red(0-7), Green(0-7), Blue(0-3)
  delay(2000);
  ssd1331.Drawing_Circle_Line_256color(50, 31, 20, 0, 7, 0); //Red(0-7), Green(0-7), Blue(0-3)
  delay(2000);
  ssd1331.Drawing_Circle_Line_256color(70, 31, 10, 0, 0, 3); //Red(0-7), Green(0-7), Blue(0-3)
  delay(2000);
  
  ssd1331.Display_Clear(0, 0, 95, 63);
 
  ssd1331.Drawing_Circle_Fill(31, 31, 31, 31, 0, 0); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  ssd1331.Drawing_Circle_Fill(50, 31, 20, 0, 63, 0); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
  ssd1331.Drawing_Circle_Fill(70, 31, 10, 0, 0, 31); //Red(0-31), Green(0-63), Blue(0-31)
  delay(2000);
 
  ssd1331.Display_Clear(0, 0, 95, 63);
  ssd1331.CommandWrite(0xA0); //Remap & Color Depth setting　
   ssd1331.CommandWrite(0b01110010); //A[7:6] = 01; 65k color format
  
  ssd1331.Drawing_Circle_Line_65kColor(31, 31, 31, 0, 63, 31); //Red(0-31), Green(0-63), Blue(0-31)
  delay(1000);
  ssd1331.Drawing_Circle_Line_65kColor(50, 31, 20, 31, 0, 31); //Red(0-31), Green(0-63), Blue(0-31)
  delay(1000);
  ssd1331.Drawing_Circle_Line_65kColor(70, 31, 10, 31, 31, 0); //Red(0-31), Green(0-63), Blue(0-31)
  delay(1000);
  
  ssd1331.SSD1331_Copy(60, 21, 80, 41, 0, 0);
  ssd1331.SSD1331_Copy(60, 21, 80, 41, 75, 0);
  ssd1331.SSD1331_Copy(60, 21, 80, 41, 0, 43);
  
  for(i=0; i<76; i++){
    ssd1331.SSD1331_Copy(60, 21, 80, 41, 75-i, 43);
    if(i>0){
      ssd1331.Display_Clear(96-i, 43, 95, 63);
    }
    delay(20);
  }
  delay(2000);
}
