/*
  WiFi Web Server LED Blink

  A simple web server that lets you blink an LED via the web.
  This sketch will print the IP address of your WiFi Shield (once connected)
  to the Serial monitor. From there, you can open that address in a web browser
  to turn on and off the LED on pin 5.

  If the IP address of your shield is yourAddress:
  http://yourAddress/H turns the LED on
  http://yourAddress/L turns it off

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.

  Circuit:
   WiFi shield attached
   LED attached to pin 5

  created for arduino 25 Nov 2012
  by Tom Igoe

  ported for sparkfun esp32
  31.01.2017 by Jan Hendrik Berlin

*/

#include <WiFi.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C

const char* ssid     = "Buffalo-G-6F1E";
const char* password = "hjmgxi5tphvpi";
WiFiServer server(80);

//FreeRTOS
xQueueHandle QUE[4];
TaskHandle_t th[5];

void Task1(void *pvParameters) {
  Serial.println("create task1.");
  vTaskDelay(100); //freeRTOS用のディレイタイム実行
  while (1) {
    Serial.printf("Task1\r\n");
    vTaskDelay(5000); //freeRTOS用のディレイタイム実行
  }
}

void Task2(void *pvParameters) {
  Serial.println("create task2.");
  vTaskDelay(100); //freeRTOS用のディレイタイム実行
  while (1) {
    Serial.printf("Task2\r\n");
    vTaskDelay(5000); //freeRTOS用のディレイタイム実行
  }
}

void TaskBME(void *pvParameters) {
  Serial.println("create task2.");
  vTaskDelay(100); //freeRTOS用のディレイタイム実行
  while (1) {
    Serial.printf("TaskBME\r\n");
    vTaskDelay(5000); //freeRTOS用のディレイタイム実行
  }
}


void setup()
{
  Serial.begin(115200);
  pinMode(5, OUTPUT);      // set the LED pin mode
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
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

  xTaskCreatePinnedToCore(Task1, "Task1", 4096, NULL, 3, &th[0], 0); //Task1実行
  xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL, 5, &th[1], 0); //Task2実行
  xTaskCreatePinnedToCore(Task3, "Task3", 4096, NULL, 5, &th[1], 1); //Task2実行

  xTaskCreatePinnedToCore(TaskBME, "TaskBME", 4096, NULL, 5, &th[1], 1); //Task2実行
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

void loop() {
  while (1) {
  }
}

