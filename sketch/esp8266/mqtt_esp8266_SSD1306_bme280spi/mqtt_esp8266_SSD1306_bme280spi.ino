#define USE_WIFI
//WIFI
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
const int SDA_OLED =  4;
const int SCL_OLED =  5;
#define OLED_RESET 4
Adafruit_SSD1306 display(SDA_OLED, SCL_OLED);

//BME280
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define BME_SCK 14
#define BME_MISO 12
#define BME_MOSI 13
#define BME_CS 16
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme(BME_CS); // hardware SPI

/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "tkdn_iPhone"
#define WLAN_PASS       "pgu74lcqt4iq"
//#define WLAN_SSID "Buffalo-G-6F1E"
//#define WLAN_PASS "hjmgxi5tphvpi"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "kadonotakashi"
#define AIO_KEY         "6767b582214a4f90a3199cdbaecffd9d"

/************ Global State (you don't need to change this!) ******************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish pressure = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pressure");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/
// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

//-------NTPサーバー定義----------------
static const char ntpServerName[] = "us.pool.ntp.org";
IPAddress timeServer(173, 71, 68, 101); //time.windows.com
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
const int timeZone = 9;     // Tokyo
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
uint32_t NTPlastTime = 0;

//-------時刻表示定義-------------------
String time_str, ymd_str;
const char* week_jp[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
uint8_t MinuteTime;

//*************************NTP Time 関数**************************************
time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}
//*************************NTP Time 関数**************************************
void sendNTPpacket(IPAddress &address)
{ // send an NTP request to the time server at the given address
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

//BME280
void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}

//////////////////////////////////////////////
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
//////////////////////////////////////////////
void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}

//////////////////////////////////////////////
//////////////////////////////////////////////
//  SETUP
//////////////////////////////////////////////
//////////////////////////////////////////////

void setup() {
  //open serial port
  Serial.begin(115200);
  delay(10);
  Serial.println(F("measure "));

  //OLED初期化
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("tmp");
  display.setCursor(10, 36);
  display.println("hmd");
  display.setCursor(10, 52);
  display.println("prs");
  display.display();

//////////////////////////////////////////////
// Connect to WiFi access point.
//////////////////////////////////////////////
#ifdef USE_WIFI
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  //////////////////////////////////////////////
  //NTPサーバーから時刻を取得
  //////////////////////////////////////////////
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  NTPlastTime = millis();
  MinuteTime = now();
#endif

  //////////////////////////////////////////////
  // BME280初期化
  //////////////////////////////////////////////
  bool status;
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

//////////////////////////////////////////////
//////////////////////////////////////////////
//  LOOP
//////////////////////////////////////////////
//////////////////////////////////////////////
void loop() {
  ////////////////////////////////////////////
  //アナログ温度センサ
  ////////////////////////////////////////////
  int adc_key_in;
  int tmp;
  float ADIN, SNSOUT;
  adc_key_in = analogRead(A0);
  SNSOUT = 1.289 * adc_key_in;  //実測で補正
  tmp = (SNSOUT - 600) / 10;
  //  Serial.println(adc_key_in);
  // Serial.println(tmp);

  ////////////////////////////////////////////
  //BME280
  ////////////////////////////////////////////
  float  temp;
  float  humi;
  float  pres;
  temp = bme.readTemperature();
  pres = bme.readPressure() / 100.0F;
  humi = bme.readHumidity();

  ////////////////////////////////////////////
  //OLEDへの表示
  ////////////////////////////////////////////
  char oled_str[6];
  display.fillRect(0, 0, 127, 15, 0);
  display.fillRect(48, 16, 111, 63, 0);
  display.setTextSize(2);
  dtostrf(temp, 6, 1, oled_str);
  display.setCursor(32, 16);  display.println(oled_str);
  dtostrf(humi, 6, 1, oled_str);
  display.setCursor(32, 32);  display.println(oled_str);
  dtostrf(pres, 6, 1, oled_str);
  display.setCursor(32, 48);  display.println(oled_str);

  display.setTextSize(1);
  display.setCursor(108, 20);
  display.println("'C");
  display.setCursor(108, 36);
  display.println("%");
  display.setCursor(108, 52);
  display.println("hPa");

  display.setCursor(20, 0);
  display.println(String(year()) + "/" +String(month()) + "/" + String(day()) + " " + String(hour()) + ":" + String(minute())+ ":" + String(second()));
  display.display();

#ifdef USE_WIFI
  ////////////////////////////////////////////
  //MQTT
  ////////////////////////////////////////////
  MQTT_connect();
  Serial.print(F("\nSending Sensor Value \n\r"));
  if (! temperature.publish(temp)) {    Serial.println(F("TEMPERATURE SEND Failed"));  }
  else {                                Serial.println(F("TEMPERATURE SEND OK!"));     }

  if (! humidity.publish(humi))  {    Serial.println(F("HUMIDITY SEND Failed"));  }
  else {                                Serial.println(F("HUMIDITY SEND OK!"));     }

  if (! pressure.publish(pres))  {    Serial.println(F("PRESSURE SEND Failed"));  }
  else {                                Serial.println(F("PRESSURE SEND OK!"));     }

#endif
  printValues();
    Serial.print("millis: ");
    Serial.print(millis());
    Serial.print("\r\n");
    Serial.print("NTPlasttime: ");
    Serial.print(NTPlastTime);
    Serial.print("\r\n");

    if (millis() - NTPlastTime > 7200000) { //2時間毎にNTP時刻補正
      setSyncProvider(getNtpTime);
      Serial.print("Get NTP Time ----------------");
      Serial.println(String(year()) + "/" + String(month()) + "/" + String(day()) + "/" + String(hour()) + ":" + String(minute()) + ":" + String(second()));
      NTPlastTime = millis();
    }
  delay(600000);  //minutes
}

