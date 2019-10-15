#include "kdn9341.h"
#include <string.h>

kdn9341::kdn9341()
{

}

void kdn9341::CmdWr(uint8_t cmd){
	digitalWrite(_dc,LOW);
	
	digitalWrite(_cs,LOW);
	SPI.transfer(cmd);
	digitalWrite(_cs,HIGH);

}
void kdn9341::DataWr(uint8_t data){
	digitalWrite(_cs,LOW);
	digitalWrite(_dc,HIGH);
	SPI.transfer(data);
	digitalWrite(_cs,HIGH);
}

/*
	ESP32,ESP8266
*/

void kdn9341::Init9341(uint8_t miso,uint8_t mosi,uint8_t sck,uint8_t dc, uint8_t rst,uint8_t cs)
{

    _miso = miso;
    _mosi = mosi;
    _sck = sck;
    _cs = cs;
	_dc = dc;
	_rst = rst;

    FontNum = SetFontInf();

	pinMode(_cs,OUTPUT);
	pinMode(_rst,OUTPUT);
	pinMode(_dc,OUTPUT);

	//reset
	digitalWrite(_cs,HIGH);
	digitalWrite(_dc,HIGH);
	digitalWrite(_rst,LOW);
	delay(100);
	digitalWrite(_rst,HIGH);
	
	//SPI initial
#if (defined(ESP8266) || defined(ESP32))
	//error occour in spresense mode
	SPI.begin(_sck,_miso,_mosi,_cs);
    	
	SPI.setFrequency(4000000);
	SPI.setDataMode(SPI_MODE3);
#endif

    CmdWr(0xEF);
    DataWr(0x03);
    DataWr(0x80);
    DataWr(0x02);

    CmdWr(0xCF);
    DataWr(0x00);
    DataWr(0XC1);
    DataWr(0X30);

    CmdWr(0xED);
    DataWr(0x64);
    DataWr(0x03);
    DataWr(0X12);
    DataWr(0X81);

    CmdWr(0xE8);
    DataWr(0x85);
    DataWr(0x00);
    DataWr(0x78);

    CmdWr(0xCB);
    DataWr(0x39);
    DataWr(0x2C);
    DataWr(0x00);
    DataWr(0x34);
    DataWr(0x02);

    CmdWr(0xF7);
    DataWr(0x20);

    CmdWr(0xEA);
    DataWr(0x00);
    DataWr(0x00);

    CmdWr(ILI9341_PWCTR1);    //Power control
    DataWr(0x23);   //VRH[5:0]

    CmdWr(ILI9341_PWCTR2);    //Power control
    DataWr(0x10);   //SAP[2:0];BT[3:0]

    CmdWr(ILI9341_VMCTR1);    //VCM control
    DataWr(0x3e);
    DataWr(0x28);

    CmdWr(ILI9341_VMCTR2);    //VCM control2
    DataWr(0x86);  //--

    CmdWr(ILI9341_MADCTL);    // Memory Access Control
    DataWr(MAC_PORTRAIT);

    CmdWr(ILI9341_VSCRSADD); // Vertical scroll
    DataWr(0);                 // Zero

    CmdWr(ILI9341_PIXFMT);
    DataWr(0x55);

    CmdWr(ILI9341_FRMCTR1);
    DataWr(0x00);
    DataWr(0x18);

    CmdWr(ILI9341_DFUNCTR);    // Display Function Control
    DataWr(0x08);
    DataWr(0x82);
    DataWr(0x27);

    CmdWr(0xF2);    // 3Gamma Function Disable
    DataWr(0x00);

    CmdWr(ILI9341_GAMMASET);    //Gamma curve selected
    DataWr(0x01);

    CmdWr(ILI9341_GMCTRP1);    //Set Gamma
    DataWr(0x0F);
    DataWr(0x31);
    DataWr(0x2B);
    DataWr(0x0C);
    DataWr(0x0E);
    DataWr(0x08);
    DataWr(0x4E);
    DataWr(0xF1);
    DataWr(0x37);
    DataWr(0x07);
    DataWr(0x10);
    DataWr(0x03);
    DataWr(0x0E);
    DataWr(0x09);
    DataWr(0x00);

    CmdWr(ILI9341_GMCTRN1);    //Set Gamma
    DataWr(0x00);
    DataWr(0x0E);
    DataWr(0x14);
    DataWr(0x03);
    DataWr(0x11);
    DataWr(0x07);
    DataWr(0x31);
    DataWr(0xC1);
    DataWr(0x48);
    DataWr(0x08);
    DataWr(0x0F);
    DataWr(0x0C);
    DataWr(0x31);
    DataWr(0x36);
    DataWr(0x0F);

    CmdWr(ILI9341_SLPOUT);    //Exit Sleep
	delay(50);
    CmdWr(ILI9341_DISPON);    //Display on
	delay(50);

}

/*
	spresense	
*/
void kdn9341::Init9341( uint8_t dc, uint8_t rst,uint8_t cs)
{
	_cs = cs;
	_dc = dc;
	_rst = rst;

    FontNum = SetFontInf();

	pinMode(_cs,OUTPUT);
	pinMode(_rst,OUTPUT);
	pinMode(_dc,OUTPUT);

	//reset
	digitalWrite(_cs,HIGH);
	digitalWrite(_dc,HIGH);
	digitalWrite(_rst,LOW);
	delay(100);
	digitalWrite(_rst,HIGH);
	
	//SPI initial
	SPI.begin();
    SPI.beginTransaction(SPISettings(2000000,MSBFIRST,SPI_MODE3));

    CmdWr(0xEF);
    DataWr(0x03);
    DataWr(0x80);
    DataWr(0x02);

    CmdWr(0xCF);
    DataWr(0x00);
    DataWr(0XC1);
    DataWr(0X30);

    CmdWr(0xED);
    DataWr(0x64);
    DataWr(0x03);
    DataWr(0X12);
    DataWr(0X81);

    CmdWr(0xE8);
    DataWr(0x85);
    DataWr(0x00);
    DataWr(0x78);

    CmdWr(0xCB);
    DataWr(0x39);
    DataWr(0x2C);
    DataWr(0x00);
    DataWr(0x34);
    DataWr(0x02);

    CmdWr(0xF7);
    DataWr(0x20);

    CmdWr(0xEA);
    DataWr(0x00);
    DataWr(0x00);

    CmdWr(ILI9341_PWCTR1);    //Power control
    DataWr(0x23);   //VRH[5:0]

    CmdWr(ILI9341_PWCTR2);    //Power control
    DataWr(0x10);   //SAP[2:0];BT[3:0]

    CmdWr(ILI9341_VMCTR1);    //VCM control
    DataWr(0x3e);
    DataWr(0x28);

    CmdWr(ILI9341_VMCTR2);    //VCM control2
    DataWr(0x86);  //--

    CmdWr(ILI9341_MADCTL);    // Memory Access Control
    DataWr(MAC_PORTRAIT);

    CmdWr(ILI9341_VSCRSADD); // Vertical scroll
    DataWr(0);                 // Zero

    CmdWr(ILI9341_PIXFMT);
    DataWr(0x55);

    CmdWr(ILI9341_FRMCTR1);
    DataWr(0x00);
    DataWr(0x18);

    CmdWr(ILI9341_DFUNCTR);    // Display Function Control
    DataWr(0x08);
    DataWr(0x82);
    DataWr(0x27);

    CmdWr(0xF2);    // 3Gamma Function Disable
    DataWr(0x00);

    CmdWr(ILI9341_GAMMASET);    //Gamma curve selected
    DataWr(0x01);

    CmdWr(ILI9341_GMCTRP1);    //Set Gamma
    DataWr(0x0F);
    DataWr(0x31);
    DataWr(0x2B);
    DataWr(0x0C);
    DataWr(0x0E);
    DataWr(0x08);
    DataWr(0x4E);
    DataWr(0xF1);
    DataWr(0x37);
    DataWr(0x07);
    DataWr(0x10);
    DataWr(0x03);
    DataWr(0x0E);
    DataWr(0x09);
    DataWr(0x00);

    CmdWr(ILI9341_GMCTRN1);    //Set Gamma
    DataWr(0x00);
    DataWr(0x0E);
    DataWr(0x14);
    DataWr(0x03);
    DataWr(0x11);
    DataWr(0x07);
    DataWr(0x31);
    DataWr(0xC1);
    DataWr(0x48);
    DataWr(0x08);
    DataWr(0x0F);
    DataWr(0x0C);
    DataWr(0x31);
    DataWr(0x36);
    DataWr(0x0F);

    CmdWr(ILI9341_SLPOUT);    //Exit Sleep
	delay(50);
    CmdWr(ILI9341_DISPON);    //Display on
	delay(50);
	
}

void kdn9341::DrawPix(uint16_t x, uint16_t y, uint16_t color)
{
	if(x>=ILI9341_IMG_WIDTH){		return ;	}
	if(y>=ILI9341_IMG_HEIGHT){	return ;	}

	CmdWr(ILI9341_CASET);
	DataWr((uint8_t)(x>>8));
	DataWr((uint8_t)(x&0xff));
	DataWr((uint8_t)(x>>8));
	DataWr((uint8_t)(x&0xff));

	CmdWr(ILI9341_PASET);
	DataWr((uint8_t)(y>>8));
	DataWr((uint8_t)(y&0xff));
	DataWr((uint8_t)(y>>8));
	DataWr((uint8_t)(y&0xff));

	CmdWr(ILI9341_RAMWR);

	DataWr((uint8_t)((color>>8)));
	DataWr((uint8_t)((color & 0xff)));
	return;

}
void kdn9341::DrawRectangle(uint16_t sx, uint16_t sy,uint16_t ex,  uint16_t ey,uint16_t color){
	int size;

	if((sx>ex) || (ex>=ILI9341_IMG_WIDTH)){	return ;	}
	if((sy>ey) || (ey>=ILI9341_IMG_HEIGHT)){	return ;	}

	CmdWr(ILI9341_CASET);
	DataWr((uint8_t)(sx>>8));
	DataWr((uint8_t)(sx&0xff));
	DataWr((uint8_t)(ex>>8));
	DataWr((uint8_t)(ex&0xff));

	CmdWr(ILI9341_PASET);
	DataWr((uint8_t)(sy>>8));
	DataWr((uint8_t)(sy&0xff));
	DataWr((uint8_t)(ey>>8));
	DataWr((uint8_t)(ey&0xff));

	CmdWr(ILI9341_RAMWR);

	size=(ex-sx+1)*(ey-sy+1);
	for(;size>0;size--){
		DataWr((uint8_t)(color>>8));
		DataWr((uint8_t)( color & 0xff));
	}
	return;

}
void kdn9341::DrawVline(uint16_t x,uint16_t y,uint16_t length,uint16_t color){
	int i;

	for(i=0;i<length;i++){
		DrawPix(x, y+i,color);
	}
}
void kdn9341::DrawHline(uint16_t x,uint16_t y,uint16_t length,uint16_t color){
	int i;

	for(i=0;i<length;i++){
		DrawPix(x+i, y,color);
	}

}

void kdn9341::ClearScreen(uint16_t color){
    DrawRectangle(0,0,ILI9341_IMG_WIDTH-1 ,ILI9341_IMG_HEIGHT-1,color);
}


/*
 * Adafruit Font Library
 * */
int kdn9341::put_charPattern_Adafruit(char CharCode,uint16_t CHAR_COLOR,uint16_t xs,uint16_t ys,int FontSel)
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

int kdn9341::PutStringAdafruit(int x,int y,char *string,uint16_t CharColor,int FontSel)
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


void kdn9341::GetFontInfo(int FontNo, FontList	*FntLst){
    FntLst->pFnt =FntList[FontNo].pFnt; 

    for(int i=0;i<FontNameSize;i++){
        FntLst->FontName[i] = FntList[FontNo].FontName[i];
    }

//    strcpy(FntList->FontName,FntList[0].FontName);

}
int kdn9341::GetFontNum(void){
    return FontNum;
}

int kdn9341::SetFontInf()
{
    FntList[7].pFnt=(GFXfont *)&FreeSerifBold18pt7b;
    strcpy(FntList[7].FontName,"SerifBold18pt7b\0");

    FntList[6].pFnt=(GFXfont *)&FreeSansBold18pt7b;
    strcpy(FntList[6].FontName,"SansBold18pt7b\0");

    FntList[5].pFnt=(GFXfont *)&FreeMonoBold18pt7b;
    strcpy(FntList[5].FontName,"MonoBold18pt7b\0");

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

    return 8;
}

void kdn9341::Drawline(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
	float a,b;
	int x,y;

	//2�_�͗L���̈�ɂ��邱�Ƃɂ���
	if(sx>=ILI9341_IMG_WIDTH)	return;
	if(sy>=ILI9341_IMG_HEIGHT)	return;
	if(ex>=ILI9341_IMG_WIDTH)	return;
	if(ey>=ILI9341_IMG_HEIGHT)	return;

	if(sx==ex){
		if(sy<ey){
			DrawVline(sx,sy,(ey-sy+1),color);
		}
		else if(ey<sy){
			DrawVline(sx,ey,(sy-ey+1),color);
		}
		else{
			DrawPix(sx,sy,color);
		}
	}

	if(sy==ey){
		if(sx<ex){
			DrawHline(sx,sy,(ex-sx+1),color);
		}
		else if(ex<sx){
			DrawHline(ex,sy,(sx-ex+1),color);
		}
		else{
			DrawPix(sx,sy,color);
		}
	}

	if(abs(ex-sx)>abs(ey-sy))	//x���W����ω������Ȃ��炙���W�����߂�
	{	//y = ax + b
		b=(float)(sy*ex-ey*sx)/(float)(ex-sx);
		if(ex!=0){
			a=((float)ey-b)/(float)ex;
		}else{
			a=((float)sy-b)/(float)sx;
		}

		if (sx<ex){
			for(x=sx;x<=ex;x++){
				y=(int)(((x*a)+b)+0.5);
				DrawPix(x,y,color);
			}
		}
		else{
			for(x=ex;x<=sx;x++){
				y=(int)(((x*a)+b)+0.5);
				DrawPix(x,y,color);
			}

		}
	}else{						//y���W����ω������Ȃ��炙���W�����߂�
		//x = ay + b
		b=(float)(sx*ey-ex*sy)/(float)(ey-sy);
		if(ey!=0){
			a=((float)ex-b)/(float)ey;
		}else{
			a=((float)sx-b)/(float)sy;
		}
		if (sy<ey){
			for(y=sy;y<=ey;y++){
				x=(int)(((y*a)+b)+0.5);
				DrawPix(x,y,color);
			}
		}
		else{
			for(y=ey;y<=sy;y++){
				x=(int)(((y*a)+b)+0.5);
				DrawPix(x,y,color);
			}
		}
	}
	return;
}

void kdn9341::BitBlt(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey,uint16_t *data) {

	int i;
	uint16_t *sp;

	sp=data;
	CmdWr(ILI9341_CASET);
	DataWr((uint8_t)(sx>>8));
	DataWr((uint8_t)(sx&0xff));
	DataWr((uint8_t)(ex>>8));
	DataWr((uint8_t)(ex&0xff));


	CmdWr(ILI9341_PASET);
	DataWr((uint8_t)(sy>>8));
	DataWr((uint8_t)(sy&0xff));
	DataWr((uint8_t)(ey>>8));
	DataWr((uint8_t)(ey&0xff));

	CmdWr(ILI9341_RAMWR);

	for(i=0;i<(ex-sx+1)*(ey-sy+1);i++,sp++){

		digitalWrite(_dc,HIGH);
		digitalWrite(_cs,LOW);
		SPI.transfer((uint8_t)(*sp>>8));
		SPI.transfer((uint8_t)( (*sp) & 0xff));
		digitalWrite(_cs,HIGH);

	}
}

