#define USE_WIFI

//for FreeRTOS
TaskHandle_t th[5];
xQueueHandle Queue_DispREQ;
xQueueHandle Queue_MqttREQ;

typedef struct {
  uint16_t index;
  uint16_t  temperature;
  uint16_t  humidity;
  uint16_t  pressure;
} SNS_VAL;


//for wifi
#include <WiFi.h>
#include <TimeLib.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/
//const char* ssid     = "Buffalo-G-6F1E";
//const char* password = "hjmgxi5tphvpi";
const char* ssid     = "tkdn_iPhone";
const char* password = "pgu74lcqt4iq";

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "kadonotakashi"
#define AIO_KEY         "6767b582214a4f90a3199cdbaecffd9d"


WiFiServer server(80);
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
Adafruit_MQTT_Publish tmpr = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/tmpr");
Adafruit_MQTT_Publish hmd = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hmd");
Adafruit_MQTT_Publish prss = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/press");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");
void MQTT_connect();

//for BME280
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
float temperature;
float pressure;
float humidity;

//for ILI9341 SPI
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 16
#define TFT_CS 5
#define TFT_CLK 18
#define TFT_MOSI 23
#define TFT_MISO 19
#define TFT_RST  4
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

//-------NTPサーバー定義----------------
IPAddress timeServer(52, 168, 138, 145); //time.windows.com
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
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);
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


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
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
//  setup
//////////////////////////////////////////////
//////////////////////////////////////////////
void setup()
{
  Serial.begin(115200);
  pinMode(5, OUTPUT);      // set the LED pin mode
  delay(10);

#ifdef  USE_WIFI
  //////////////////////////////////////////////
  // We start by connecting to a WiFi network
  //////////////////////////////////////////////
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  delay(500);
  mqtt.subscribe(&onoffbutton);
#endif

  //////////////////////////////////////////////
  // BME280 initialize
  //////////////////////////////////////////////
  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  Serial.println("-- BME280 initialize Success --");
  Serial.println();

  //////////////////////////////////////////////
  // LCD initialize
  //////////////////////////////////////////////
  tft.begin();
  tft.setRotation(1);

#ifdef  USE_WIFI
  //////////////////////////////////////////////
  //NTPサーバーから時刻を取得
  //////////////////////////////////////////////
  delay(2000); //ここの秒数が少ないとESP8266のWi-Fiが起動していないので十分余裕を持つこと
  setSyncProvider(getNtpTime);
  delay(1000);

  NTPlastTime = millis();
  MinuteTime = now();
#endif
  Queue_DispREQ = xQueueCreate(1, sizeof(SNS_VAL));
  Queue_MqttREQ = xQueueCreate(1, sizeof(SNS_VAL));

  xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL, 5, &th[1], 0); //Task2実行
  xTaskCreatePinnedToCore(TaskBME, "TaskBME", 4096, NULL, 5, &th[3], 0); //センサー
  xTaskCreatePinnedToCore(TaskTFT, "TaskTFT", 4096, NULL, 5, &th[4], 1); //表示

#ifdef  USE_WIFI
  xTaskCreatePinnedToCore(Task1, "Task1", 4096, NULL, 3, &th[0], 0); //Task1実行
//  xTaskCreatePinnedToCore(Task3, "Task3", 4096, NULL, 5, &th[2], 0); //Task3実行
#endif
}

//////////////////////////////////////////////
//////////////////////////////////////////////
//  loop
//////////////////////////////////////////////
//////////////////////////////////////////////
void loop() {
  //  do nothing
}

int value = 0;
void Task3(void *pvParameters) {
  Serial.println("create task3.");
  vTaskDelay(100); //freeRTOS用のディレイタイム実行

  while (1) {
    WiFiClient client = server.available();   // listen for incoming clients
    if (client) {                             // if you get a client,
      Serial.println("New Client.");           // print a message out the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              // the content of the HTTP response follows the header:
              client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
              client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            } else {    // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }

          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("GET /H")) {
            digitalWrite(5, HIGH);               // GET /H turns the LED on
          }
          if (currentLine.endsWith("GET /L")) {
            digitalWrite(5, LOW);                // GET /L turns the LED off
          }
        }
      }
      // close the connection:
      client.stop();
      Serial.println("Client Disconnected.");
    }
  }
}



void Task1(void *pvParameters) {
  SNS_VAL  LOCAL_QUE;
  portBASE_TYPE result;

  Serial.println("create task1.");
  vTaskDelay(100);
  while (1) {

    result = xQueueReceive(Queue_MqttREQ, &LOCAL_QUE, 1);
    if (result == pdTRUE) {
      MQTT_connect();

      //recv
      Adafruit_MQTT_Subscribe *subscription;
      while ((subscription = mqtt.readSubscription(5000))) {
        if (subscription == &onoffbutton) {
          Serial.print(F("Got: "));
          Serial.println((char *)onoffbutton.lastread);
        }
      }

      //send
      Serial.print(F("\nSending Sensor Value"));
      Serial.print("...");

      if (! tmpr.publish(LOCAL_QUE.temperature)) {
        Serial.println(F("Failed"));
      } else {
        Serial.println(F("OK!"));
      }

      if (! hmd.publish(LOCAL_QUE.humidity)) {
        Serial.println(F("Failed"));
      } else {
        Serial.println(F("OK!"));
      }

      if (! prss.publish(LOCAL_QUE.pressure)) {
        Serial.println(F("Failed"));
      } else {
        Serial.println(F("OK!"));
      }

      // ping the server to keep the mqtt connection alive
      // NOT required if you are publishing once every KEEPALIVE seconds
      /*
        if(! mqtt.ping()) {
        mqtt.disconnect();
        }
      */
    }
  }
}

void Task2(void *pvParameters) {
  Serial.println("create task2.");
  vTaskDelay(100); //freeRTOS用のディレイタイム実行
  while (1) {
    vTaskDelay(5000); //freeRTOS用のディレイタイム実行
  }
}

void TaskBME(void *pvParameters) {
  SNS_VAL  LOCAL_QUE;
  vTaskDelay(100); //freeRTOS用のディレイタイム実行
  int position = 0;
  LOCAL_QUE.index = 0;
  LOCAL_QUE.temperature = bme.readTemperature();
  LOCAL_QUE.pressure = bme.readPressure() / 100;
  LOCAL_QUE.humidity = bme.readHumidity();
  while (1) {
#ifdef  USE_WIFI
    if (millis() - NTPlastTime > 1800000) { //30分毎にNTP時刻補正
      setSyncProvider(getNtpTime);
      Serial.print("Get NTP Time ----------------");
      Serial.println(String(year()) + "/" + String(month()) + "/" + String(day()) + "/" + String(hour()) + ":" + String(minute()) + ":" + String(second()));
      NTPlastTime = millis();
    }
#endif
    LOCAL_QUE.temperature = bme.readTemperature();
    LOCAL_QUE.pressure = bme.readPressure() / 100;
    LOCAL_QUE.humidity = bme.readHumidity();

    xQueueSendToBack(Queue_DispREQ, &LOCAL_QUE, portMAX_DELAY);
    xQueueSendToBack(Queue_MqttREQ, &LOCAL_QUE, portMAX_DELAY);
    LOCAL_QUE.index++;
    if ( LOCAL_QUE.index == 240) {
      LOCAL_QUE.index = 0;
    }
    vTaskDelay(1000); //freeRTOS用のディレイタイム実行
  }
}

void TaskTFT(void *pvParameters) {
  portBASE_TYPE result;
  SNS_VAL  LOCAL_QUE;

  char tft_str[16];

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_CYAN);
  tft.setCursor(20, 10);
  tft.println("Temp:              C");
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(20, 30);
  tft.println("Hum :              %");
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(20, 50);
  tft.println("Pres:              hPa");

  //チャートのフレームを描いておく
  tft.fillRect(59, 69, 242, 162 , ILI9341_NAVY);
  tft.drawRect(59, 69, 242, 162 , ILI9341_WHITE);
  tft.drawFastHLine(60, 110 , 240, ILI9341_LIGHTGREY);
  tft.drawFastHLine(60, 150 , 240, ILI9341_LIGHTGREY);
  tft.drawFastHLine(60, 190 , 240, ILI9341_LIGHTGREY);
  tft.drawFastVLine(120, 70, 160, ILI9341_LIGHTGREY);
  tft.drawFastVLine(180, 70, 160, ILI9341_LIGHTGREY);
  tft.drawFastVLine(240, 70, 160, ILI9341_LIGHTGREY);

  tft.setTextSize(0);
  tft.setTextColor(ILI9341_CYAN);
  tft.setCursor(20, 65);
  tft.println("40");
  tft.setCursor(20, 105);
  tft.println("27");
  tft.setCursor(20, 145);
  tft.println("15");
  tft.setCursor(26, 185);
  tft.println("2");
  tft.setCursor(15, 220);
  tft.println("-10");

  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(35, 65);
  tft.println("100");
  tft.setCursor(40, 105);
  tft.println("75");
  tft.setCursor(40, 145);
  tft.println("50");
  tft.setCursor(40, 185);
  tft.println("25");
  tft.setCursor(46, 220);
  tft.println("0");

  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(25, 75);
  tft.println("1040");
  tft.setCursor(25, 115);
  tft.println("1020");
  tft.setCursor(25, 155);
  tft.println("1000");
  tft.setCursor(25, 195);
  tft.println(" 980");
  tft.setCursor(25, 230);
  tft.println(" 960");

  while (1) {
    result = xQueueReceive(Queue_DispREQ, &LOCAL_QUE, 1);
    if (result == pdTRUE) {
      //      tft.fillRect(200, 0, 270, 64, ILI9341_RED);
      tft.fillRect(200, 0, 270, 64, ILI9341_BLACK);
      tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
      tft.setCursor(200, 10);
      tft.println(String(year()) + "/" + String(month()) + "/" + String(day()) );
      tft.setCursor(210, 30);
      tft.println( String(hour()) + ":" + String(minute()) + ":" + String(second()));

      //      tft.fillRect(50, 0, 76, 64, ILI9341_BLUE);
      tft.fillRect(50, 0, 76, 64, ILI9341_BLACK);
      dtostrf(LOCAL_QUE.temperature, 6, 1, tft_str);
      tft.setTextColor(ILI9341_CYAN);  tft.setTextSize(2);
      tft.setCursor(50, 5);
      tft.println(tft_str);

      dtostrf(LOCAL_QUE.humidity, 6, 1, tft_str);
      tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
      tft.setCursor(50, 25);
      tft.println(tft_str);

      dtostrf((LOCAL_QUE.pressure), 6, 1, tft_str);
      tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
      tft.setCursor(50, 45);
      tft.println(tft_str);
      plot_chart(LOCAL_QUE);
    }
    vTaskDelay(1); //freeRTOS用のディレイタイム実行
  }
}


void plot_chart(SNS_VAL QUE)
{
  int16_t x, y;
  float work;
  int16_t color;

  x = (QUE.index % 240) + 60;
  if ((0 == QUE.index % 60) && (QUE.index > 0)) {
    color = ILI9341_LIGHTGREY;
    tft.drawFastVLine(x, 70, 160, color);
  }
  else {
    color = ILI9341_NAVY;
    tft.drawFastVLine(x, 70, 160, color);
    color = ILI9341_LIGHTGREY;
    tft.drawPixel(x, 110, color);
    tft.drawPixel(x, 150, color);
    tft.drawPixel(x, 190, color);
  }
  if (QUE.index == 239) {
    tft.drawFastVLine(60, 70, 160, ILI9341_GREENYELLOW);
  }
  else {
    tft.drawFastVLine(x + 1, 70, 160, ILI9341_GREENYELLOW);
  }

  //temperature
  if ((QUE.temperature < 40.0) && (QUE.temperature > -10)) {
    work = QUE.temperature;
    work = (work + 10) * (160.0 / 50.0);
    y = 230 - (int)work;
    tft.drawPixel(x, y, ILI9341_CYAN);
  }
  else { //はみ出している
  }

  //humidity
  if ((QUE.humidity < 100.0) && (QUE.humidity > 0.0)) {
    work = QUE.humidity;
    work = work * (160.0 / 100.0);
    y = 230 - (int)work;
    tft.drawPixel(x, y, ILI9341_YELLOW);
  }
  else { //はみ出している
  }

  //pressure
  if ((QUE.pressure < 1040.0) && (QUE.pressure > 960.0)) {
    work = QUE.pressure;
    work = (work - 960) * (160.0 / 80.0);
    y = 230 - (int)work;
    tft.drawPixel(x, y, ILI9341_GREEN);
  }
  else { //はみ出している
  }
}

