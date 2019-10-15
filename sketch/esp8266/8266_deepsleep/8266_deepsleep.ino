#define USE_WIFI

/*
  for BME280
*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C

/*
  for WIFI , MQTT
*/
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

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
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish temp2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp2");
Adafruit_MQTT_Publish humi2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humi2");
Adafruit_MQTT_Publish pres2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pres2");
Adafruit_MQTT_Publish vbat2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/vbat2");

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
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("wake up");

  bool status;
  /*
    initialize BME280
  */
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
#ifdef USE_WIFI
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  MQTT_connect();
#endif
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void loop() {
  ////////////////////////////////////////////
  //BatteryMonitor
  ////////////////////////////////////////////
  int adc_val;
  float Vbat;
  adc_val = analogRead(A0);
  Vbat = (float)adc_val / 320.0; //あとで実測で補正

  ////////////////////////////////////////////
  //BME280 sensing
  ////////////////////////////////////////////
  float  temp;
  float  humi;
  float  pres;
  temp = bme.readTemperature();
  pres = bme.readPressure() / 100.0F;
  humi = bme.readHumidity();

  ////////////////////////////////////////////
  // display sensing result0
  ////////////////////////////////////////////
  Serial.print("Vbat       = ");  Serial.print(Vbat);  Serial.println(" V");
  Serial.print("Temparature= ");  Serial.print(temp);  Serial.println("'C");
  Serial.print("Humidity   = ");  Serial.print(humi);  Serial.println(" %");
  Serial.print("Pressure   = ");  Serial.print(pres);  Serial.println("hPa");

#ifdef USE_WIFI

  if (! temp2.publish(temp)) {
    Serial.println(F("TEMPERATURE SEND Failed"));
  }
  else {
    Serial.println(F("TEMPERATURE SEND OK!"));
  }

  if (! humi2.publish(humi))  {
    Serial.println(F("HUMIDITY SEND Failed"));
  }
  else {
    Serial.println(F("HUMIDITY SEND OK!"));
  }

  if (! pres2.publish(pres))  {
    Serial.println(F("PRESSURE SEND Failed"));
  }
  else {
    Serial.println(F("PRESSURE SEND OK!"));
  }

  if (! vbat2.publish(Vbat))  {
    Serial.println(F("Battery Level SEND Failed"));
  }
  else {
    Serial.println(F("Battery Level SEND OK!"));
  }

#endif
  //DEEP SLEEPモード突入命令
  Serial.println("go to DEEP SLEEP!!");
  delay(10000);

  //1:μ秒での復帰までのタイマー時間設定  2:復帰するきっかけの設定（モード設定）
  ESP.deepSleep(30 * 1000 * 1000 , WAKE_RF_DEFAULT);
  //deepsleepモード移行までのダミー命令
  delay(1000);

  //実際にはこの行は実行されない
  Serial.println("DEEP SLEEPing....");

}
