
#include <SPI.h>

/////////////////OLED制御のため
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCL  5
#define SDA  4
Adafruit_SSD1306 display(SDA, SCL);

//SSD1306でログを表示するための仕組み

#define LogLinePitch  8
#define LogFontHeight  8
int LogLine;

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID "tkdn_iPhone"
#define WLAN_PASS "pgu74lcqt4iq"

// Adafruit. io Setup
#define AIO_SERVER      "io.adafruit. com"
#define AIO_SERVERPORT  1883                    // use 8883 for SSL 
#define AIO_USERNAME    "kadonotakashi"
#define AIO_KEY         "6767b582214a4f90a3199cdbaecffd9d"    // Initialize DHT sensor DHT dht( DHTPIN, DHTTYPE, 15); 

// Create an ESP 8266 WiFiClient class to connect to the MQTT server. WiFiClient client;
// or... use WiFiFlientSecure for SSL
WiFiClientSecure client;

// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.

const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

// Setup feeds for Temperature
const char TEMPERATURE_FEED[] PROGMEM = AIO_USERNAME "/feeds/ Temperature";
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(& mqtt, TEMPERATURE_FEED);

void MQTT_connect();
//////////////////////////////////////
//setup
/////////////////////////////////////
void setup()   {
  Serial.begin(115200);
  //OLED setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextSize(1);  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("OLED init done.");
  display.display();

  //connect toWiFi
  Serial.println("connecting to");
  Serial.println(WLAN_SSID);

  WiFi.begin( WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
      delay( 500);
      Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address:");
  Serial.println( WiFi. localIP());

  MQTT_connect();
}


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print(" Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {  // connect will return 0 for connected
    Serial.println( mqtt.connectErrorString( ret));
    Serial.println(" Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay( 5000);   // wait 5 seconds retries--;
    if (retries == 0) { // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println(" MQTT Connected!");
}

void loop() {

}


