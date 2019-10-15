#include <Wire.h>

#include <OLED_SSD1306.h>
OLED_SSD1306 oled;
const uint8_t OLED_Adress = 0x3C; //OLEDのI2Cアドレス

void setup() {
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");

  pinMode(LED0, OUTPUT);
  digitalWrite(LED0, HIGH);
  Wire.begin(); // I2C initialise the connection
  Wire.setClock(400000L); //クロックはMax 400kHz
  oled.Ini_OLED_SSD1306(OLED_Adress, 127);


}

void loop() {
  delay(200);
  digitalWrite(LED0, HIGH);
  delay(200);
  digitalWrite(LED0, LOW);


}
