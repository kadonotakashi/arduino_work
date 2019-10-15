#include "IFTTTESP8266.h"
#define EVENT  "Your_Maker_Event_Name_here"  // Put here your Maker Event Name
#define KEY  "Your_Key_here"  // Put here your IFTTT key
#define WIFISSID "ssid"
#define PASSWORD "password"

IFTTT client(KEY);

void setup(){
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    String value = String(analogRead(A0));
    client.add(value1); // specifies the args of type "String"
    //Send value can specify up to three
    client.add(value2); // optional
    client.add(value3); // optional
    client.sendAll(EVENT);
}
