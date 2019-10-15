//  Spresense ILI9341 SPI library
//
//

#ifndef kdn1331_h
#define kdn1331_h

#include "Arduino.h"
#include <SPI.h>

#include "gfxfont.h"
#include "./fonts/FreeMono9pt7b.h"
#include "./fonts/FreeSans9pt7b.h"
#include "./fonts/FreeSerif9pt7b.h"
#include "./fonts/FreeMonoBold9pt7b.h"
#include "./fonts/TomThumb.h"


#define NumOfFont	8

typedef struct{
	GFXfont *pFnt;
	char FontName[16];
}FontList;


class kdn1331
{
  private:
    uint8_t _mosi;
    uint8_t _miso;
    uint8_t _sck;
    uint8_t _cs;
    uint8_t _dc;
    uint8_t _rst;

    //
    uint8_t CMode;
    uint8_t FontNo;
    uint16_t FontColor;
    uint16_t FontBackColor;

    FontList	FntList[NumOfFont];

    int FontNum;
 
    void CmdWr(uint8_t cmd);
    void DataWr(uint8_t data);

    int SetFontInf();
    int put_charPattern_Adafruit(char CharCode,uint16_t CHAR_COLOR,uint16_t xs,uint16_t ys,int FontSel);

  public:
    kdn1331();
    void Init1331(uint8_t miso,uint8_t mosi,uint8_t sck,uint8_t dc, uint8_t rst,uint8_t cs);
    void Init1331( uint8_t dc, uint8_t rst);
 
    void ClearScreen(void);
    void DrawPix( uint16_t x, uint16_t y, uint16_t color);
    void DrawLine( uint8_t xs, uint8_t ys,	uint8_t xe,uint8_t ye,uint16_t color);
    void DrawRectangle(  uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color);
    void DrawRectangleFill(  uint8_t xs,	uint8_t ys,uint8_t xe,uint8_t ye,uint16_t line_color,uint16_t fill_color);

    void GetFontInfo(int FontNo, FontList	*FntList);
    int GetFontNum(void);
    int PutStringAdafruit(int x,int y,char *string,uint16_t CharColor,int FontSel);

};


#define	WIDTH_1331	96
#define	HEIGHT_1331	64

#define ColorMode8bit 0
#define ColorMode16bit 1



//SSD1331 Command
#define	SSD1331_CMD_SetClmAddr		0x15
#define	SSD1331_CMD_SetRawAddr		0x75

#define	SSD1331_CMD_SetCntrstA		0x81
#define	SSD1331_CMD_SetCntrstB		0x82
#define	SSD1331_CMD_SetCntrstC		0x83

#define	SSD1331_CMD_SetMstrCrrnt	0x87
#define	SSD1331_CMD_SetPrChgSpeed	0x8A
#define	SSD1331_CMD_SetChrgA		0x8A
#define	SSD1331_CMD_SetChrgB		0x8B
#define	SSD1331_CMD_SetChrgC		0x8C

#define	SSD1331_CMD_SetRemap_DataFormat	0xA0

#define	SSD1331_CMD_SetDspStrtLine	0xA1
#define	SSD1331_CMD_SetDspOffset	0xA2
#define	SSD1331_CMD_SetDspModeNorm	0xA4
#define	SSD1331_CMD_SetDspModeDspOn	0xA5
#define	SSD1331_CMD_SetDspModeDspOff	0xA6
#define	SSD1331_CMD_SetDspModeInv	0xA7
#define	SSD1331_CMD_SetMltplRatio	0xA8
#define	SSD1331_CMD_DimModeSetting	0xAB
#define	SSD1331_CMD_SetMstrCnfg		0xAD

#define	SSD1331_CMD_SetDimModeOn	0xAC
#define	SSD1331_CMD_SetDspOff		0xAE
#define	SSD1331_CMD_SetDspOn		0xAF

#define	SSD1331_CMD_PowerSave		0xB0
#define	SSD1331_CMD_P1_P2_ADJ		0xB1
#define	SSD1331_CMD_SetDispCLK		0xB3
#define	SSD1331_CMD_SetGrayScale	0xB8
#define	SSD1331_CMD_EnLinearScale	0xB9
#define	SSD1331_CMD_SetPrChrgV		0xBB
#define	SSD1331_CMD_SetVcomhV		0xBE
#define	SSD1331_CMD_NOP			0xBC
#define	SSD1331_CMD_SetCmdLock		0xFD

#define	SSD1331_CMD_DrawLine		0x21
#define	SSD1331_CMD_DrawRectangle	0x22
#define	SSD1331_CMD_Copy		0x23
#define	SSD1331_CMD_DimWindow		0x24
#define	SSD1331_CMD_ClearWindow		0x25
#define	SSD1331_CMD_FillEnable		0x26
#define	SSD1331_CMD_SetScrol		0x27
#define	SSD1331_CMD_DeactiveScrol	0x2E
#define	SSD1331_CMD_ActiveScrol		0x2F


#define	RGB565_WHITE	(uint16_t)0xffff
#define	RGB565_BLACK	(uint16_t)0x0000
#define	RGB565_RED		(uint16_t)0xf800
#define	RGB565_GREEN	(uint16_t)0x07e0
#define	RGB565_BLUE		(uint16_t)0x001f
#define	RGB565_MAGENTA	(uint16_t)0xf81f
#define	RGB565_YELLOW	(uint16_t)0xffe0
#define	RGB565_CYAN		(uint16_t)0x07FF

#define	RGB332_WHITE	(uint8_t)0xff
#define	RGB332_BLACK	(uint8_t)0x00
#define	RGB332_RED		(uint8_t)0xe0
#define	RGB332_GREEN	(uint8_t)0x1c
#define	RGB332_BLUE		(uint8_t)0x03
#define	RGB332_MAGENTA	(uint8_t)0xe3
#define	RGB332_YELLOW	(uint8_t)0xfc
#define	RGB332_CYAN		(uint8_t)0x1f

#endif
