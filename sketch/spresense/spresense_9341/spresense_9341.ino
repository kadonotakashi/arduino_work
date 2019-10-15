#include <kdn9341.h>
#include <MP.h>

//SPRESENSE
//const uint8_t SCLK_LCD =  13; //SCLK
//const uint8_t MISO_LCD =  12; //これは実際は使っていない。MISO (Master Input Slave Output)
//const uint8_t MOSI_LCD =  11; //MOSI (Master Output Slave Input)
//const uint8_t CS_OLED = 10;
const uint8_t CS_LCD = 7;
const uint8_t DC_LCD =  9; //OLED DC(Data/Command)
const uint8_t RST_LCD =  8; //OLED Reset

#define IMG_SIZE_X  256
#define IMG_SIZE_y  240 


kdn9341 TFT;
FontList flist;
uint16_t *vram;


void setup() {

  Serial.begin(115200);  
  TFT.Init9341( DC_LCD, RST_LCD, CS_LCD);

  vram = (uint16_t *)MP.AllocSharedMemory( IMG_SIZE_X * IMG_SIZE_y );
  uint16_t pixel; 
  printf("SharedMemory Address=@%08x\n", (uint32_t)vram);
  
  for (int y=0 ; y < IMG_SIZE_y ; y++ ){
    for (int x=0 ; x < IMG_SIZE_X ; x++ ){
      pixel = (((y/4) & 0x1f)<<11) + (((x/2) & 0x3f)<<5) + (((x+y)/4)&0x1f);
      *(vram + y * 256 + x) = pixel;
      
//      printf(": %04x", pixel );
    }
    printf("\n");
  }
}

void loop() {
  TFT.DrawRectangle(160, 120, 319, 239, RGB565_BLACK);
  TFT.DrawRectangle(160, 0, 319, 119, RGB565_GREEN);
  TFT.DrawRectangle(0, 0, 159, 119, RGB565_BLUE);
  TFT.DrawRectangle(0, 120, 159, 239, RGB565_RED);

  TFT.BitBlt(32,287,0, 239,vram);
  printf("SharedMemory Address=@%08x\n", (uint32_t)vram);


  TFT.Drawline(0, 0, 40, 239, RGB565_WHITE);
  TFT.Drawline(0, 0, 80, 239, RGB565_YELLOW);
  TFT.Drawline(0, 0, 120, 239, RGB565_CYAN);
  TFT.Drawline(0, 0, 160, 239, RGB565_MAGENTA);
  TFT.Drawline(0, 0, 200, 239, RGB565_GREEN);
  TFT.Drawline(0, 0, 240, 239, RGB565_YELLOW);
  TFT.Drawline(0, 0, 280, 239, RGB565_WHITE);

  TFT.Drawline(319, 0, 40, 239, RGB565_WHITE);
  TFT.Drawline(319, 0, 80, 239, RGB565_YELLOW);
  TFT.Drawline(319, 0, 120, 239, RGB565_CYAN);
  TFT.Drawline(319, 0, 160, 239, RGB565_MAGENTA);
  TFT.Drawline(319, 0, 200, 239, RGB565_GREEN);
  TFT.Drawline(319, 0, 240, 239, RGB565_YELLOW);
  TFT.Drawline(319, 0, 280, 239, RGB565_WHITE);

  TFT.DrawVline( 40, 0, 240, RGB565_BLACK);
  TFT.DrawVline( 80, 0, 240, RGB565_BLACK);
  TFT.DrawVline( 120, 0, 240, RGB565_BLACK);
  TFT.DrawVline( 160, 0, 240, RGB565_BLACK);
  TFT.DrawVline( 200, 0, 240, RGB565_WHITE);
  TFT.DrawVline( 240, 0, 240, RGB565_WHITE);
  TFT.DrawVline( 280, 0, 240, RGB565_WHITE);


  TFT.GetFontInfo(0,&flist);
  TFT.DrawHline( 0, 15, 320, RGB565_BLACK);
  TFT.PutStringAdafruit(5,15,flist.FontName,RGB565_WHITE,0);

  TFT.GetFontInfo(1,&flist);
  TFT.DrawHline( 0, 40, 320, RGB565_BLACK);
  TFT.PutStringAdafruit(5,40,flist.FontName,RGB565_YELLOW,1);

  TFT.GetFontInfo(2,&flist);
  TFT.DrawHline( 0, 65, 320, RGB565_BLACK);
  TFT.PutStringAdafruit(5,65,flist.FontName,RGB565_GREEN,2);
  
  TFT.GetFontInfo(3,&flist);
  TFT.DrawHline( 0, 90, 320, RGB565_BLACK);
  TFT.PutStringAdafruit(5,90,flist.FontName,RGB565_CYAN,3);

  TFT.GetFontInfo(4,&flist);
  TFT.DrawHline( 0, 110, 320, RGB565_BLACK);
  TFT.PutStringAdafruit(5,110,flist.FontName,RGB565_WHITE,4);

  TFT.GetFontInfo(5,&flist);
  TFT.DrawHline( 0, 150, 320, RGB565_BLACK);
  TFT.PutStringAdafruit(5,150,flist.FontName,RGB565_YELLOW,5);

  TFT.GetFontInfo(6,&flist);
  TFT.DrawHline( 0, 190, 320, RGB565_BLACK);
  TFT.PutStringAdafruit(5,190,flist.FontName,RGB565_GREEN,6);

  TFT.GetFontInfo(7,&flist);
  TFT.DrawHline( 0, 230, 320, RGB565_BLACK);
  TFT.PutStringAdafruit(5,230,flist.FontName,RGB565_WHITE,7);


}
