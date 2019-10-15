/* OLED(有機EL) SSD1306にフリーの日本語フォント、東雲フォント(16x16)を表示させるサンプルスケッチ
 *  予めSPIFFSファイルシステムやOTAを使ってフォントファイル等をフラッシュにアップロードしておくこと
 *  This library
 *  UTF8toSJIS.h
 *  OLED_SSD1306.h
 *  ShinonomeFONTread.h
 *  is what Mgo-tec has created.
 */
#include <Wire.h>
#include <UTF8toSJIS.h>
#include <OLED_SSD1306.h>
#include <ShinonomeFONTread.h>

UTF8toSJIS u8ts;
OLED_SSD1306 oled;
ShinonomeFONTread SFR;

const char* UTF8SJIS_file = "/Utf8Sjis.tbl"; //UTF8 Shift_JIS 変換テーブルファイル名を記載しておく
const char* ZenkakuFontFile = "/shnmk16.bdf"; //全角フォントファイル名を定義
const char* HalfFontFile = "/shnm8x16r.bdf"; //半角フォントファイル名を定義

const uint8_t OLED_Adress = 0x3C; //OLEDのI2Cアドレス

char* c[4] = {"フリー日本語漢字","16x16ﾄﾞｯﾄ です。","全角-半角混在OK!","半角ｶﾀｶﾅもOK！！"};
//Char型ならばポインタ配列にする必要あり。String型でも良い

uint8_t sj_txt[32]; //Shift_JISコード
uint16_t sj_length; //Shift_JISコードの長さ
uint8_t font_buf[32][16]; //東雲フォントデータ
uint8_t tmp_buf1[16],tmp_buf2[16]; //フォントデータ９０度回転後の一時データ

uint8_t cp; //ある一文字のShift_JISコードバイト数
uint16_t i, j;
int16_t Rotation = 90; //90度回転させる

void setup() {
  Wire.begin(); // I2C initialise the connection
  Wire.setClock(400000L); //クロックはMax 400kHz

  oled.Ini_OLED_SSD1306(OLED_Adress, 127); //Contrast 0-255 default 127

  delay(300);
  
  for(j=0; j<4; j++){
    u8ts.UTF8_to_SJIS_str_cnv(UTF8SJIS_file, c[j], sj_txt, &sj_length);
    i=0;
    while(i<sj_length){
      cp=SFR.SjisToShinonome16FontRead(ZenkakuFontFile, HalfFontFile, 0, 0, sj_txt[i], sj_txt[i+1], font_buf[i], font_buf[i+1]);
      i=i+cp;
    }
    for(i=0; i<16; i++){
      oled.Dot_8X16_Rotation(90, font_buf[i], tmp_buf1, tmp_buf2);
      oled.OLED_8x16_Display_Out(OLED_Adress, 120-8*i, 6-j*2, tmp_buf1, tmp_buf2);
    }
  }
}

void loop() {
}
