#include "kdn1331.h"

kdn1331::kdn1331()
{
}



void kdn1331::CmdWr(uint8_t cmd){
	digitalWrite(_dc,LOW);
#if (defined(ESP8266) || defined(ESP32))
	digitalWrite(_cs,LOW);
#endif
	SPI.transfer(cmd);
#if (defined(ESP8266) || defined(ESP32))
	digitalWrite(_cs,HIGH);
#endif
}
//    void CmdWrBytes(uint8_t cmd, uint16_t n);
void kdn1331::DataWr(uint8_t data){
	digitalWrite(_dc,HIGH);
#if (defined(ESP8266) || defined(ESP32))
	digitalWrite(_cs,LOW);
#endif
	SPI.transfer(data);
#if (defined(ESP8266) || defined(ESP32))
	digitalWrite(_cs,HIGH);
#endif
}
//    void DataWrBytes(uint8_t data, uint16_t n);
void kdn1331::Init1331(uint8_t miso,uint8_t mosi,uint8_t sck,uint8_t dc, uint8_t rst,uint8_t cs)
{
    _miso = miso;
    _mosi = mosi;
    _sck = sck;
    _cs = cs;
	_dc = dc;
	_rst = rst;

	FontNum=SetFontInf();

	pinMode(_rst,OUTPUT);
	pinMode(_dc,OUTPUT);
	pinMode(_cs,OUTPUT);

	//reset
	digitalWrite(_cs,HIGH);
	digitalWrite(_dc,HIGH);
	digitalWrite(_rst,LOW);
	delay(20);
	digitalWrite(_rst,HIGH);

#if (defined(ESP8266) || defined(ESP32))
	SPI.begin(_sck,_miso,_mosi,_cs);
    	
	SPI.setFrequency(4000000);
	SPI.setDataMode(SPI_MODE3);
#endif
 
	CmdWr(SSD1331_CMD_SetDspOff);
	CmdWr(SSD1331_CMD_SetRemap_DataFormat);	CmdWr(0x72);	//A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format
	CmdWr(SSD1331_CMD_SetDspStrtLine);		CmdWr(0x0);		//
	CmdWr(SSD1331_CMD_SetDspOffset);		CmdWr(0x0);		//
	CmdWr(SSD1331_CMD_SetDspModeNorm);
	CmdWr(SSD1331_CMD_SetMltplRatio);		CmdWr(63);		//
	CmdWr(SSD1331_CMD_SetMstrCnfg);			CmdWr(0x8e);	//
	CmdWr(SSD1331_CMD_PowerSave);			CmdWr(0x1A);	//
	CmdWr(SSD1331_CMD_P1_P2_ADJ);			CmdWr(0x74);	//
	CmdWr(SSD1331_CMD_SetDispCLK);			CmdWr(0xf0);	//
	CmdWr(SSD1331_CMD_SetChrgA);			CmdWr(0x81);	//
	CmdWr(SSD1331_CMD_SetChrgB);			CmdWr(0x82);	//
	CmdWr(SSD1331_CMD_SetChrgC);			CmdWr(0x83);	//
	CmdWr(SSD1331_CMD_SetPrChrgV);			CmdWr(0x3A);	//
	CmdWr(SSD1331_CMD_SetVcomhV);			CmdWr(0x3E);	//
	CmdWr(SSD1331_CMD_SetMstrCrrnt);		CmdWr(0x06);	//
	CmdWr(SSD1331_CMD_SetClmAddr);			CmdWr(0);	CmdWr(95);	//
	CmdWr(SSD1331_CMD_SetRawAddr);			CmdWr(0);	CmdWr(63);	//
	CmdWr(SSD1331_CMD_SetCntrstA);			CmdWr(0xff);	//
	CmdWr(SSD1331_CMD_SetCntrstB);			CmdWr(0xff);	//
	CmdWr(SSD1331_CMD_SetCntrstC);			CmdWr(0xfF);	//

	CmdWr(SSD1331_CMD_SetDspOn);			//
	delay(100);
	
}



void kdn1331::Init1331( uint8_t dc, uint8_t rst)
{
    //from argument
	_dc = dc;
	_rst = rst;

	FontNum=SetFontInf();

	pinMode(_rst,OUTPUT);
	pinMode(_dc,OUTPUT);
//	pinMode(_cs,OUTPUT);

	//reset
	digitalWrite(_rst,HIGH);
//	digitalWrite(_cs,HIGH);
	digitalWrite(_dc,HIGH);
	digitalWrite(_rst,LOW);
	delay(20);
	digitalWrite(_rst,HIGH);
	delay(10);

	SPI.begin();
    SPI.beginTransaction(SPISettings(20000000,MSBFIRST,SPI_MODE3));
 
	CmdWr(SSD1331_CMD_SetDspOff);
	CmdWr(SSD1331_CMD_SetRemap_DataFormat);	CmdWr(0x72);	//A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format
	CmdWr(SSD1331_CMD_SetDspStrtLine);		CmdWr(0x0);		//
	CmdWr(SSD1331_CMD_SetDspOffset);		CmdWr(0x0);		//
	CmdWr(SSD1331_CMD_SetDspModeNorm);
	CmdWr(SSD1331_CMD_SetMltplRatio);		CmdWr(63);		//
	CmdWr(SSD1331_CMD_SetMstrCnfg);			CmdWr(0x8e);	//
	CmdWr(SSD1331_CMD_PowerSave);			CmdWr(0x1A);	//
	CmdWr(SSD1331_CMD_P1_P2_ADJ);			CmdWr(0x74);	//
	CmdWr(SSD1331_CMD_SetDispCLK);			CmdWr(0xf0);	//
	CmdWr(SSD1331_CMD_SetChrgA);			CmdWr(0x81);	//
	CmdWr(SSD1331_CMD_SetChrgB);			CmdWr(0x82);	//
	CmdWr(SSD1331_CMD_SetChrgC);			CmdWr(0x83);	//
	CmdWr(SSD1331_CMD_SetPrChrgV);			CmdWr(0x3A);	//
	CmdWr(SSD1331_CMD_SetVcomhV);			CmdWr(0x3E);	//
	CmdWr(SSD1331_CMD_SetMstrCrrnt);		CmdWr(0x06);	//
	CmdWr(SSD1331_CMD_SetClmAddr);			CmdWr(0);	CmdWr(95);	//
	CmdWr(SSD1331_CMD_SetRawAddr);			CmdWr(0);	CmdWr(63);	//
	CmdWr(SSD1331_CMD_SetCntrstA);			CmdWr(0xff);	//
	CmdWr(SSD1331_CMD_SetCntrstB);			CmdWr(0xff);	//
	CmdWr(SSD1331_CMD_SetCntrstC);			CmdWr(0xfF);	//

	CmdWr(SSD1331_CMD_SetDspOn);			//
	delay(100);
	
}


void kdn1331::ClearScreen()
{
  delayMicroseconds(500); //クリアーコマンドは400μs 以上の休止期間が必要かも
	CmdWr(SSD1331_CMD_ClearWindow);
		CmdWr(0);	//X始点
		CmdWr(0);	//Y始点
		CmdWr(WIDTH_1331-1);	//X終点
		CmdWr(HEIGHT_1331-1);	//Y終点
  delayMicroseconds(500); //クリアーコマンドは400μs 以上の休止期間が必要かも
}


void kdn1331::DrawPix(    uint16_t x, 
                            uint16_t y, 
                            uint16_t color   )
{
	if(x>=WIDTH_1331-1){		return ;	}
	if(y>=HEIGHT_1331-1){	return ;	}

	CmdWr(SSD1331_CMD_SetClmAddr);		CmdWr(x);	CmdWr(x);
    CmdWr(SSD1331_CMD_SetRawAddr);		CmdWr(y);	CmdWr(y);
	DataWr( 0xff & (color>>8) );        DataWr( 0xff & (color) );
	return;

}

void kdn1331::DrawLine( uint8_t xs,	
                        uint8_t ys,	
                        uint8_t xe,	
                        uint8_t ye,	
                        uint16_t color	)
{
	if(xe>=WIDTH_1331-1){	return ;	}
	if(ye>=HEIGHT_1331-1){	return ;	}
	if(xe>=WIDTH_1331-1){	return ;	}
	if(ye>=HEIGHT_1331-1){	return ;	}

	CmdWr(SSD1331_CMD_DrawLine);
		CmdWr(xs);	CmdWr(ys);	CmdWr(xe);	CmdWr(ye);

		//color     R 5bit		bit 15-11
		//			G 6bit		bit 10- 5
		//			B 5bit		bit  4- 0
		CmdWr((color>>10) & 0x3e);	//R 5bit
		CmdWr((color>>5) & 0x3f);	//G 6bit
		CmdWr((color<<1)  & 0x3e);	//B 5bit
}

void kdn1331::DrawRectangle(  uint8_t xs,	
                                uint8_t ys,	
                                uint8_t xe,	
                                uint8_t ye,	
                                uint16_t color	)
{
	CmdWr(SSD1331_CMD_FillEnable);	CmdWr(0x00);	//塗りつぶし禁止

	CmdWr(SSD1331_CMD_DrawRectangle);
		CmdWr(xs);	CmdWr(ys);	CmdWr(xe);	CmdWr(ye);

		//color     R 5bit		bit 15-11
		//			G 6bit		bit 10- 5
		//			B 5bit		bit  4- 0
		CmdWr((color>>10) & 0x3e);	//R 5bit
		CmdWr((color>>5) & 0x3f);	//G 6bit
		CmdWr((color<<1)  & 0x3e);	//B 5bit
		CmdWr(0);	//R 5bit
		CmdWr(0);	//G 6bit
		CmdWr(0);	//B 5bit
	delay(100);

}

void kdn1331::DrawRectangleFill(  uint8_t xs,	
                                    uint8_t ys,	
                                    uint8_t xe,	
                                    uint8_t ye,	
                                    uint16_t line_color,
                            		uint16_t fill_color	)

{
	CmdWr(SSD1331_CMD_FillEnable);	CmdWr(0x01);	//塗りつぶし禁止

	CmdWr(SSD1331_CMD_DrawRectangle);
		CmdWr(xs);	CmdWr(ys);	CmdWr(xe);	CmdWr(ye);

		//color     R 5bit		bit 15-11
		//			G 6bit		bit 10- 5
		//			B 5bit		bit  4- 0
		CmdWr((line_color>>10) & 0x3e);	//R 5bit
		CmdWr((line_color>>5) & 0x3f);	//G 6bit
		CmdWr((line_color<<1)  & 0x3e);	//B 5bit
		CmdWr((fill_color>>10) & 0x3e);	//R 5bit
		CmdWr((fill_color>>5) & 0x3f);	//G 6bit
		CmdWr((fill_color<<1)  & 0x3e);	//B 5bit
	delayMicroseconds(400);
}


/*
 * Adafruit Font Library
 * */
int kdn1331::put_charPattern_Adafruit(char CharCode,uint16_t CHAR_COLOR,uint16_t xs,uint16_t ys,int FontSel)
{
	uint8_t FirstCode,char_offset;
	uint8_t *pFNT;

	int x,y;
	int FontWidth,FontHeight,FontDotCap,bitCnt;
	int xOffset,yOffset;
	uint8_t LinePattern;
	int BitCount;

	FirstCode = FntList[FontSel].pFnt->first;
	if ((CharCode > FntList[FontSel].pFnt->last)||(CharCode < FirstCode)){
		return -1;
	}

	char_offset=(CharCode - FirstCode);
	pFNT = FntList[FontSel].pFnt->bitmap;
	pFNT += FntList[FontSel].pFnt->glyph[char_offset].bitmapOffset;

	FontWidth = FntList[FontSel].pFnt->glyph[char_offset].width;
	FontHeight = FntList[FontSel].pFnt->glyph[char_offset].height;
	FontDotCap = FontWidth * FontHeight;

	xOffset = FntList[FontSel].pFnt->glyph[char_offset].xOffset;
	yOffset = FntList[FontSel].pFnt->glyph[char_offset].yOffset;

	x=0;
	y=0;
	BitCount=8;
	
	for(bitCnt=0;bitCnt<FontDotCap;bitCnt++){
		if (8==BitCount){
			LinePattern = *pFNT++;
			BitCount=0;
		}
		if((LinePattern & 0x80)!=0){
			DrawPix(x+xs+xOffset,y+ys+yOffset,CHAR_COLOR);
//			Serial.print("@");	

//			Serial.print(" x ");	
//			Serial.print(x+xs+xOffset);	
//			Serial.print(" y ");	
//			Serial.print(y+ys+yOffset);	

		}else{
//			Serial.print(".");	
		}

		LinePattern = LinePattern<<1;

		if(x>=FontWidth-1){
			x=0;	y++;
//			Serial.println();	
		}else{
			x++;
		}
		BitCount++;
	}
	return FntList[FontSel].pFnt->glyph[char_offset].xAdvance;
}

int kdn1331::PutStringAdafruit(int x,int y,char *string,uint16_t CharColor,int FontSel)
{
	int i,j,k;
	char *STR;

	STR = (char *)string;

	for(i=0,j=x;;i++,STR++){

		if (*STR==0){
			return 0;
		}

		k=put_charPattern_Adafruit(*STR,CharColor,j,y,FontSel);
		if(k<0){
			return -2;
		}else{
			j=j+k;
		}
	}
	return 0;
}



void kdn1331::GetFontInfo(int FontNo, FontList	*FntLst){
    FntLst->pFnt =FntList[FontNo].pFnt; 

    for(int i=0;i<16;i++){
        FntLst->FontName[i] = FntList[FontNo].FontName[i];
    }

//    strcpy(FntList->FontName,FntList[0].FontName);

}
int kdn1331::GetFontNum(void){
    return FontNum;
}

int kdn1331::SetFontInf()
{
    FntList[7].pFnt=(GFXfont *)0;
    FntList[6].pFnt=(GFXfont *)0;
    FntList[5].pFnt=(GFXfont *)0;

    FntList[4].pFnt=(GFXfont *)&TomThumb;
    strcpy(FntList[4].FontName,"TomThumb\0");

    FntList[3].pFnt=(GFXfont *)&FreeSerif9pt7b;
    strcpy(FntList[3].FontName,"Serif9pt7b\0");

    FntList[2].pFnt=(GFXfont *)&FreeSans9pt7b;
    strcpy(FntList[2].FontName,"Sans9pt7b\0");

    FntList[1].pFnt=(GFXfont *)&FreeMonoBold9pt7b;
    strcpy(FntList[1].FontName,"MonoBold9pt7b\0");

    FntList[0].pFnt=(GFXfont *)&FreeMono9pt7b;
    strcpy(FntList[0].FontName,"Mono9pt7b\0");

    return 5;
}



