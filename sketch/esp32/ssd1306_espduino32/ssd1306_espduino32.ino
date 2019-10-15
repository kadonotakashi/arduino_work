#include "Wire.h"
 
const uint8_t ADDRES_OLED =  0x3C;
const int SDA_OLED =  21;
const int SCL_OLED =  22;
const uint32_t Frequensy_OLED = 400000; //Max=400kHz
 
uint8_t segment[128][8] = { 0 };
 
//************* setup **************************
void setup() {
  Serial.begin(115200);
 
  SSD1306_Init(); //OLED ssd1306 初期化
  delay(1000);
  Clear_Display_All();
}
//************** main loop *********************
void loop() {
  int i;
 
  Draw_Rectangle_Line(0,0,127,63, "white");
  Draw_Line(0, 0, 127, 63, "white");
  Draw_Line(127, 0, 0, 63, "white");
  delay(2000);
  Clear_Display_All();
 
  Draw_Rectangle_Fill(10,10,50,50, "white");
  Draw_Line(10,10,50,50, "black");
  Draw_Line(50,10,10,50, "black");
  Draw_Vertical_Line(30, 10, 50, "black");
  Draw_Horizontal_Line(10, 50, 30, "black");
  Draw_Rectangle_Line(64,10,104,50,"white");
  Draw_Line(64,10,104,50, "white");
  Draw_Line(104,10,64,50, "white");
  Draw_Vertical_Line(84, 10, 50, "white");
  Draw_Horizontal_Line(64, 104, 30, "white");
  delay(2000);
  Clear_Display_All();
 
  Draw_Circle_Fill(30, 32, 20, "white");
  Draw_Vertical_Line(30, 12, 52, "black");
  Draw_Horizontal_Line(10, 50, 32, "black");
  Draw_Circle_Line(84, 32, 20, "white");
  Draw_Vertical_Line(84, 12, 52, "white");
  Draw_Horizontal_Line(64, 104, 32, "white");
  delay(1000);
  Clear_Display_All();
 
  Draw_Rectangle_Fill(0,0,127,63, "white");
  Draw_Circle_Fill(64, 32, 20, "black");
  Draw_Vertical_Line(64, 12, 52, "white");
  Draw_Horizontal_Line(44, 84, 32, "white");
  delay(2000);
  Clear_Display_All();
 
  Draw_Rectangle_Fill(0,0,127,63, "white");
  Draw_Rectangle_Fill(44,12,84,52, "black");
  Draw_Vertical_Line(64, 12, 52, "white");
  Draw_Horizontal_Line(44, 84, 32, "white");
  delay(2000);
  Clear_Display_All();
 
  //---------LINE------------------------
  for(i=0; i<64; i=i+5){
    Draw_Line(0, i, i, 63, "white");
    Draw_Line(127-i, 0, 127, 63-i, "white");
    delay(100);
  }
  delay(2000);
  Clear_Display_All();
 
  for(i=0; i<64; i=i+5){
    Draw_Line(32, i, 32+i, 63, "white");
    Draw_Line(32+i, 0, 95, i, "white");
  }
  delay(2000);
  Clear_Display_All();
 
  for(i=0; i<64; i=i+5){
    Draw_Line(0, i, 63-i, 0, "white");
    Draw_Line(63+i, 63, 127, 63-i, "white");
  }
  delay(2000);
  Clear_Display_All();
 
  for(i=0; i<64; i=i+5){
    Draw_Line(32, i, 95-i, 0, "white");
    Draw_Line(32+i, 63, 95, 63-i, "white");
  }
  delay(2000);
  Clear_Display_All();
 
  for(i=0; i<64; i=i+5){
    Draw_Line(32, i, 32+i, 63, "white");
    Draw_Line(32+i, 0, 95, i, "white");
  }
  for(i=0; i<64; i=i+5){
    Draw_Line(32, i, 95-i, 0, "white");
    Draw_Line(32+i, 63, 95, 63-i, "white");
  }
  delay(2000);
  Clear_Display_All();
 
  for(i=0; i<64; i=i+5){
    Draw_Line(127-i, 0, 0, i, "white");
    Draw_Line(i, 63, 127, 63-i, "white");
  }
  for(i=65; i<128; i=i+5){
    Draw_Line(127-i, 0, 0, 63, "white");
    Draw_Line(i, 63, 127, 0, "white");
  }
  Draw_Circle_Fill(64, 32, 20, "white");
  Draw_Vertical_Line(64, 12, 52, "black");
  Draw_Horizontal_Line(44, 84, 32, "black");
  delay(2000);
  Clear_Display_All();
 
  Draw_Rectangle_Fill(0,0,127,63, "white");
  for(i=0; i<64; i=i+5){
    Draw_Line(127-i, 0, 0, i, "black");
    Draw_Line(i, 63, 127, 63-i, "black");
  }
  for(i=65; i<128; i=i+5){
    Draw_Line(127-i, 0, 0, 63, "black");
    Draw_Line(i, 63, 127, 0, "black");
  }
  Draw_Circle_Fill(64, 32, 20, "black");
  Draw_Vertical_Line(64, 12, 52, "white");
  Draw_Horizontal_Line(44, 84, 32, "white");
  delay(2000);
 
  //------Brightness Fade Out Fade In-------------- 
  for(i=255; i>=0; i--){
    Brightness(i);
    delay(10);
  }
  for(i=0; i<255; i++){
    Brightness(i);
    delay(10);
  }
  delay(1000);
  Clear_Display_All();
  Brightness(255);
 
  //--------Graph-----------------
  for(i=45; i>=0; i=i-8){
    Draw_Horizontal_Line(8, 10, i, "white");
  }
  Draw_Horizontal_Line(8, 127, 53, "white");
  Draw_Vertical_Line(10, 0, 55, "white");
  for(i=16; i<128; i=i+8){
    Draw_Vertical_Line(i, 53, 55, "white");
  }
 
  delay(3000);
  Clear_Display_All();
}
//******************************************
void SSD1306_Init(){
  Wire.begin(SDA_OLED, SCL_OLED);
  Wire.setClock(Frequensy_OLED);
  delay(100);
 
  Wire.beginTransmission(ADDRES_OLED);
    Wire.write(0b00000000); //control byte, Co bit = 0 (continue), D/C# = 0 (command) Max=31byte
      Wire.write(0xAE); //display off
    Wire.write(0b00000000);
      Wire.write(0xA8); //Set Multiplex Ratio  0xA8, 0x3F
        Wire.write(0b00111111); //64MUX
    Wire.write(0b00000000);    
      Wire.write(0xD3); //Set Display Offset 0xD3, 0x00
        Wire.write(0x00);
    Wire.write(0b00000000);
      Wire.write(0x40); //Set Display Start Line 0x40
    Wire.write(0b00000000);
      Wire.write(0xA1); //Set Segment re-map 0xA0/0xA1
    Wire.write(0b00000000);
      Wire.write(0xC8); //Set COM Output Scan Direction 0xC0,/0xC8
    Wire.write(0b00000000);
      Wire.write(0xDA); //Set COM Pins hardware configuration 0xDA, 0x02
        Wire.write(0b00010010);
    Wire.write(0b00000000);
      Wire.write(0x81); //Set Contrast Control 0x81, default=0x7F
        Wire.write(255); //0-255
    Wire.write(0b00000000);
      Wire.write(0xA4); //Disable Entire Display On
    Wire.write(0b00000000);
      Wire.write(0xA6); //Set Normal Display 0xA6, Inverse display 0xA7
    Wire.write(0b00000000);
      Wire.write(0xD5); //Set Display Clock Divide Ratio/Oscillator Frequency 0xD5, 0x80
        Wire.write(0b10000000);
    Wire.write(0b00000000);
      Wire.write(0x20); //Set Memory Addressing Mode
        Wire.write(0x10); //Page addressing mode
  Wire.endTransmission();
  Wire.beginTransmission(ADDRES_OLED);
    Wire.write(0b00000000);
      Wire.write(0x22); //Set Page Address
        Wire.write(0); //Start page set
        Wire.write(7); //End page set
    Wire.write(0b00000000);
      Wire.write(0x21); //set Column Address
        Wire.write(0); //Column Start Address
        Wire.write(127); //Column Stop Address
    Wire.write(0b00000000);
      Wire.write(0x8D); //Set Enable charge pump regulator 0x8D, 0x14
        Wire.write(0x14);
    Wire.write(0b00000000);
      Wire.write(0xAF); //Display On 0xAF
  Wire.endTransmission();
}
//**************************************************
void Clear_Display_All(){
  uint8_t i, j, k;
 
  for(i = 0; i < 8; i++){//Page(0-7)
    Column_Page_Set(0, 127, i);
 
    for(j = 0; j < 16; j++){//column = 8byte x 16
      Wire.beginTransmission(ADDRES_OLED);
      Wire.write(0b01000000); //control byte, Co bit = 0 (continue), D/C# = 1 (data)
      for(k = 0; k < 8; k++){ //continue to 31byte
        Wire.write(0x00);
      }
      Wire.endTransmission();
    }
    //delay(1);
  }
 
  for(i=0; i<128; i++){
    for(j=0; j<8; j++){
       segment[i][j] = 0;
    }
  }
}
//**************************************************
void Clear_Page(uint8_t x0, uint8_t p0, uint8_t x1, uint8_t p1){
  uint8_t i, j, k;
 
  for(i = p0; i < p1; i++){//Page(0-7)
    Column_Page_Set(x0, x1, i);
 
    for(j = 0; j < 16; j++){//column = 8byte x 16
      Wire.beginTransmission(ADDRES_OLED);
      Wire.write(0b01000000); //control byte, Co bit = 0 (continue), D/C# = 1 (data)
      for(k = 0; k < 8; k++){ //continue to 31byte
        Wire.write(0x00);
      }
      Wire.endTransmission();
    }
    //delay(1);
  }
 
  for(i=0; i<128; i++){
    for(j=0; j<8; j++){
       segment[i][j] = 0;
    }
  }
}
//*************************************************
void Brightness(uint8_t brightness){
  Wire.beginTransmission(ADDRES_OLED);
    Wire.write(0b00000000); //control byte, Co bit = 1 (One command only), D/C# = 0 (command)
      Wire.write(0x81); //Set Contrast Control 0x81, default=0x7F
        Wire.write(brightness); //0-255
  Wire.endTransmission();
}
//*************************************************
void Draw_Line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, String BW){
  if(x0 > 127) x0 = 127;
  if(x1 > 127) x1 = 127;
  if(y0 > 63) y0 = 63;
  if(y1 > 63) y1 = 63;
 
  if(x0 == x1 && y0 == y1) return;
 
  int i;
  int16_t X0 = x0, X1 = x1;
  int16_t Y0 = y0, Y1 = y1;
 
  int16_t Y = 0, X = 0;
  int16_t length_x = X1 - X0;
  int16_t length_y = Y1 - Y0;
 
  if(abs(length_x) > abs(length_y)){
    float degY = ((float)length_y) / ((float)length_x);
    if(X0<X1){
      for(i=X0; i<(X1+1); i++){
        Y = Y0 + round((i-X0) * degY);
        Draw_Pixel(i, Y, BW);
      }
    }else{
      for(i=X0; i>=X1; i--){
        Y = Y0 + round((i-X0) * degY);
        Draw_Pixel(i, Y, BW);
      }
    }
  }else{
    float degX = ((float)length_x) / ((float)length_y);
 
    if(Y0<Y1){
      for(i=Y0; i<(Y1+1); i++){
        X = X0 + round((i-Y0) * degX);
        Draw_Pixel(X, i, BW);
      }
    }else{
      for(i=Y0; i>=Y1; i--){
        X = X0 + round((i-Y0) * degX);
        Draw_Pixel(X, i, BW);
      }
    }
  }
  yield();
}
//***********************************************
void Draw_Circle_Fill(uint8_t x0, uint8_t y0, uint16_t r, String BW){  
  uint8_t x1, y1;
  int i;
 
  for(i=0; i<360; i++){
    x1 = round( (float)(x0 + (r * cos(radians(i)))) );
    y1 = round( (float)(y0 + (r * sin(radians(i)))) );
    Draw_Vertical_Line(x1, y0, y1, BW);
  }
}
//***********************************************
void Draw_Circle_Line(uint8_t x0, uint8_t y0, uint16_t r, String BW){
  int i;
  uint8_t x1, y1;
 
  for(i=0; i<360; i++){
    x1 = round( (float)(x0 + (r * cos(radians(i)))) );
    y1 = round( (float)(y0 + (r * sin(radians(i)))) );
    Draw_Pixel(x1, y1, BW);
  }
}
//************************************************
void Draw_Rectangle_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, String BW){
  Draw_Horizontal_Line(x0, x1, y0, BW);
  Draw_Horizontal_Line(x0, x1, y1, BW);
  Draw_Vertical_Line(x0, y0, y1, BW);
  Draw_Vertical_Line(x1, y0, y1, BW);
}
//************************************************
void Draw_Rectangle_Fill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, String BW){
  // x0 < x1, y0 < y1
  int i;
  for(i=x0; i<(x1+1); i++){
    Draw_Vertical_Line(i, y0, y1, BW);
  }
}
//***********************************************
void Draw_Pixel(uint8_t x0, uint8_t y0, String BW){
  if(x0 > 127) x0 = 127;
  if(y0 > 63) y0 = 63;
 
  uint8_t page = floor(y0 / 8);
 
  if(BW == "white"){
    segment[x0][page] = segment[x0][page] | (0x01 << (y0 % 8));
  }
  if(BW == "black"){
    segment[x0][page] = segment[x0][page] & ~(0x01 << (y0 % 8));
  }
 
  Column_Page_Set(x0, x0, page);
  Data_Send(segment[x0][page]);
}
//***********************************************
void Draw_Vertical_Line(uint8_t x0, uint8_t y0, uint8_t y1, String BW){
  if(x0 > 127) x0 = 127;
  if(y0 > 63) y0 = 63;
  if(y1 > 63) y1 = 63;
 
  if(y0 == y1) return;
 
  int i;
  uint8_t Y0 = y0, Y1 = y1;
 
  if(y1 < y0){
    Y0 = y1; Y1 = y0;
  }
 
  uint8_t pageA = floor(Y0 / 8);
  uint8_t pageB = floor(Y1 / 8);
 
  uint8_t pA = (uint8_t)(0xFF << (Y0 % 8));
  uint8_t pB = ~( (uint8_t)(0xFF << ((Y1 % 8)+1)) );
   
  if(pageA == pageB){
    if(BW == "black"){
      segment[x0][pageA] = segment[x0][pageA] & ~(pA & pB);
    }else{
      segment[x0][pageA] = segment[x0][pageA] | (pA & pB);
    }
 
    Column_Page_Set(x0, x0, pageA);
    Data_Send(segment[x0][pageA]);
  }else{
    if(BW == "black"){
      segment[x0][pageA] = segment[x0][pageA] & ~pA;
      segment[x0][pageB] = segment[x0][pageB] & ~pB;
    }else{
      segment[x0][pageA] = segment[x0][pageA] | pA;
      segment[x0][pageB] = segment[x0][pageB] | pB;
    }
 
    Column_Page_Set(x0, x0, pageA);
    Data_Send(segment[x0][pageA]);
 
    for(i=(pageA+1); i<pageB; i++){
      Column_Page_Set(x0, x0, i);
 
      if(BW == "black"){
        segment[x0][i] = 0x00;
      }else{
        segment[x0][i] = 0xFF;
      }
      Data_Send(segment[x0][i]);
    }
 
    Column_Page_Set(x0, x0, pageB);
    Data_Send(segment[x0][pageB]);
  }
  yield();
}
//********************************************
void Draw_Horizontal_Line(uint8_t x0, uint8_t x1, uint8_t y0, String BW){
  if(x0 > 127) x0 = 127;
  if(x1 > 127) x1 = 127;
  if(y0 > 63) y0 = 63;
 
  if(x0 == x1) return;
 
  int i;
  uint8_t X0 = x0, X1 = x1;
  uint8_t page = floor(y0 / 8);
  uint8_t b;
 
  if(x1 < x0){
    X0 = x1; X1 = x0;
  }
 
  b = 0x01 << (y0 % 8);
 
  Column_Page_Set(X0, X1, page);
 
  for(i=X0; i< (X1+1); i++){//column = 8byte x 16
    if(BW == "black"){
      segment[i][page] = segment[i][page] & ~b;
    }else{
      segment[i][page] = segment[i][page] | b;
    }
    Data_Send(segment[i][page]);
  }
  yield();
}
//******************************************
void Column_Page_Set(uint8_t x0, uint8_t x1, uint8_t page){
  Wire.beginTransmission(ADDRES_OLED);
    Wire.write(0b00000000); //control byte, Co bit = 0 (continue), D/C# = 0 (command) Max=31byte
      Wire.write(0xB0 | page ); //set page start address(B0～B7)
    Wire.write(0b00000000);
      Wire.write(0x21); //set Column Address
        Wire.write(x0); //Column Start Address(0-127)
        Wire.write(x1); //Column Stop Address(0-127)
  Wire.endTransmission();
}
//*****************************************
void Data_Send(uint8_t data){
  Wire.beginTransmission(ADDRES_OLED);
    Wire.write(0b01000000);
      Wire.write(data);
  Wire.endTransmission();
}
