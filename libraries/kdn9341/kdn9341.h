//  Spresense ILI9341 SPI library
//
//

#ifndef kdn9341_h
#define kdn9341_h

#include "Arduino.h"
#include "SPI.h"

#include "gfxfont.h"

#include "./fonts/FreeMono9pt7b.h"
#include "./fonts/FreeMonoBold9pt7b.h"
#include "./fonts/FreeSans9pt7b.h"
#include "./fonts/FreeSerif9pt7b.h"

#include "./fonts/FreeMonoBold18pt7b.h"
#include "./fonts/FreeSansBold18pt7b.h"
#include "./fonts/FreeSerifBold18pt7b.h"
#include "./fonts/TomThumb.h"

#define NumOfFont	8
#define FontNameSize  32
typedef struct{
	GFXfont *pFnt;
	char FontName[FontNameSize];
}FontList;


#define ILI9341_TFTWIDTH   240
#define ILI9341_TFTHEIGHT  320

#define ILI9341_NOP        0x00
#define ILI9341_SWRESET    0x01
#define ILI9341_RDDID      0x04
#define ILI9341_RDDST      0x09

#define ILI9341_SLPIN      0x10
#define ILI9341_SLPOUT     0x11
#define ILI9341_PTLON      0x12
#define ILI9341_NORON      0x13

#define ILI9341_RDMODE     0x0A
#define ILI9341_RDMADCTL   0x0B
#define ILI9341_RDPIXFMT   0x0C
#define ILI9341_RDIMGFMT   0x0D
#define ILI9341_RDSELFDIAG 0x0F

#define ILI9341_INVOFF     0x20
#define ILI9341_INVON      0x21
#define ILI9341_GAMMASET   0x26
#define ILI9341_DISPOFF    0x28
#define ILI9341_DISPON     0x29

#define ILI9341_CASET      0x2A
#define ILI9341_PASET      0x2B
#define ILI9341_RAMWR      0x2C
#define ILI9341_RAMRD      0x2E

#define ILI9341_PTLAR      0x30
#define ILI9341_MADCTL     0x36
#define ILI9341_VSCRSADD   0x37
#define ILI9341_PIXFMT     0x3A

#define ILI9341_FRMCTR1    0xB1
#define ILI9341_FRMCTR2    0xB2
#define ILI9341_FRMCTR3    0xB3
#define ILI9341_INVCTR     0xB4
#define ILI9341_DFUNCTR    0xB6

#define ILI9341_PWCTR1     0xC0
#define ILI9341_PWCTR2     0xC1
#define ILI9341_PWCTR3     0xC2
#define ILI9341_PWCTR4     0xC3
#define ILI9341_PWCTR5     0xC4
#define ILI9341_VMCTR1     0xC5
#define ILI9341_VMCTR2     0xC7

#define ILI9341_RDID1      0xDA
#define ILI9341_RDID2      0xDB
#define ILI9341_RDID3      0xDC
#define ILI9341_RDID4      0xDD

#define ILI9341_GMCTRP1    0xE0
#define ILI9341_GMCTRN1    0xE1

#define MAC_PORTRAIT 0xe8
#define MAC_LANDSCAPE 0x48
#define COLMOD_16BIT 0x55
#define COLMOD_18BIT 0x66

#define ILI9341_IMG_WIDTH 320
#define ILI9341_IMG_HEIGHT 240

#define	RGB565_WHITE	0xffff
#define	RGB565_BLACK	0x0000
#define	RGB565_RED		0xf800
#define	RGB565_GREEN	0x07e0
#define	RGB565_BLUE		0x001f
#define	RGB565_MAGENTA	0xf81f
#define	RGB565_YELLOW	0xffe0
#define	RGB565_CYAN		0x07FF

class kdn9341
{
  private:
    uint8_t _mosi;
    uint8_t _miso;
    uint8_t _sck;
    uint8_t _cs;
    uint8_t _dc;
    uint8_t _rst;

   FontList	FntList[NumOfFont];
    int FontNum; 
    int SetFontInf(void);
    int put_charPattern_Adafruit(char CharCode,uint16_t CHAR_COLOR,uint16_t xs,uint16_t ys,int FontSel);

  public:
    kdn9341();
    void Init9341(uint8_t miso,uint8_t mosi,uint8_t sck,uint8_t dc, uint8_t rst,uint8_t cs);
    void Init9341( uint8_t dc, uint8_t rst,uint8_t cs);
    void CmdWr(uint8_t cmd);
    void DataWr(uint8_t data);
    void DrawPix(uint16_t x, uint16_t y, uint16_t color);
    void DrawRectangle(uint16_t sx,uint16_t sy, uint16_t ex,  uint16_t ey,uint16_t color);
    void Drawline(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
    void DrawVline(uint16_t x,uint16_t y,uint16_t length,uint16_t color);
    void DrawHline(uint16_t x,uint16_t y,uint16_t length,uint16_t color);
    void ClearScreen(uint16_t color);
    void GetFontInfo(int FontNo, FontList	*FntList);
    int GetFontNum(void);
    int PutStringAdafruit(int x,int y,char *string,uint16_t CharColor,int FontSel);
    void BitBlt(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey,uint16_t *data);

};




#endif
