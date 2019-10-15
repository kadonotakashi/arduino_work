#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(0x53);

//OLED
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
const int SDA_OLED =  4;
const int SCL_OLED =  5;
#define OLED_RESET 4
Adafruit_SSD1306 display(SDA_OLED, SCL_OLED);

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    ");

  switch (accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 ");
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 ");
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 ");
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 ");
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 ");
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 ");
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 ");
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 ");
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 ");
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 ");
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 ");
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 ");
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 ");
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 ");
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 ");
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 ");
      break;
    default:
      Serial.print  ("???? ");
      break;
  }
  Serial.println(" Hz");
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- ");
  switch (accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 ");
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 ");
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 ");
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 ");
      break;
    default:
      Serial.print  ("?? ");
      break;
  }
  Serial.println(" g");
}

void InitSSD1306(void) {
  //OLED初期化
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}
void disp2OLED(float x_vect, float y_vect, float z_vect) {
  float sum, power;
  int xp, yp;
  char oled_str[5];

  //3成分の合成
  sum = pow(x_vect, 2.0) + pow(y_vect, 2.0) + pow(z_vect, 2.0);
  power = sqrt(sum);
  display.fillRect(0, 0, 127, 63, 0);

  //powerをbar graph
  //sumを作業用に使用
  //  yp = a * sum +b;
  //　以下の条件でa,bを求めると
  //  sum=0.0 -> yp = 63
  //  sum=9.8 -> yp = 32
  //  sum=19.6-> yp = 1
  // b=63,a=3.214
  sum = 63.0 - (power * 3.214);
  yp = (int)sum;
  //dtostrf(sum, 4, 2, oled_str);
  //display.setCursor(0, 0);  display.println(oled_str);
  display.drawRect(70, 0, 10, 63, 1);   //枠
  display.fillRect(70, yp - 2, 10, 4, 1);

  //xy成分をplot
  //  xp = a * x_vect +b;
  //　以下の条件でa,bを求めると
  //  x_vect=-9.8 -> xp = 1
  //  x_vect=0    -> xp = 32
  //  x_vect=9.8  -> yp = 63
  //  b=32,a=3.16
  sum = 3.16 * x_vect + 32;
  xp = (int)sum;
  // dtostrf(sum, 4, 2, oled_str);
  // display.setCursor(0, 16);  display.println(oled_str);

  sum = -3.16 * y_vect + 32;
  yp = (int)sum;
  //  dtostrf(sum, 4, 2, oled_str);
  //  display.setCursor(0,32);  display.println(oled_str);

  display.fillRect(xp - 2, yp - 2, 5, 5, 1);
  display.drawFastVLine(32, 0, 64, 1);
  display.drawFastHLine(0, 32, 64, 1);

  //各成分を文字表示
  dtostrf(x_vect, 5, 2, oled_str);
  display.setCursor(88, 8);  display.println(oled_str);
  display.setCursor(94, 0);  display.println("x\0");

  dtostrf(y_vect, 5, 2, oled_str);
  display.setCursor(88, 24);  display.println(oled_str);
  display.setCursor(94, 16);  display.println("y\0");

  dtostrf(z_vect, 5, 2, oled_str);
  display.setCursor(88, 40);  display.println(oled_str);
  display.setCursor(94, 32);  display.println("z\0");

  dtostrf(power, 5, 2, oled_str);
  display.setCursor(88, 56);  display.println(oled_str);
  display.setCursor(94, 48);  display.println("sum\0");
  display.display();
}


void setup(void)
{
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");

  InitSSD1306();  //OLED初期化

  /* Initialise the sensor */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }
  /* Set the range to whatever is appropriate for your project */
  //  accel.setRange(ADXL345_RANGE_16_G);
  accel.setRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_2_G);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("");
}

void loop(void)
{
  float x_vect, y_vect, z_vect;
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);
  x_vect = event.acceleration.x;
  y_vect = event.acceleration.y;
  z_vect = event.acceleration.z;

  disp2OLED(x_vect, y_vect, z_vect );

  /* Display the results (acceleration is measured in m/s^2) */
  //  Serial.print("X: "); Serial.print(x_vect); Serial.print("  ");
  //  Serial.print("Y: "); Serial.print(y_vect); Serial.print("  ");
  //  Serial.print("Z: "); Serial.print(z_vect); Serial.print("  ");  Serial.println("m/s^2 ");
  //  delay(100);
}



