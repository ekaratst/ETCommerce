#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ServerExceed.h>
#include <pt.h>

WiFiServer server(80); // nodeMCU server : port 80
char ssid[] = "eXceed";
char password[] = "";
char host[] = "10.32.176.4";
int port = 80;
String group = "etc"; 
char auth[] = "e0f8014854fa4d03a3ee8523a63f7fa3";
struct pt pt_taskBlynk;
struct pt pt_taskEvent;
char blynk_host[] = "10.32.176.4";
int blynk_port = 18442;

ServerExceed mcu(ssid, password, host, port, group, &server);

void setup() {
  Serial.begin(115200);
  mcu.connectServer();
  Blynk.config(auth, blynk_host, blynk_port);
}

String data = "";
int his2 = 0;
int his3 = 0;
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
    his2 = pinValue;
    Serial.println(pinValue);
}
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
    his3 = pinValue;
    Serial.println(pinValue);
}


void loop() {
  if(Serial.available()) {
    data = Serial.readStringUntil('\r');
    data.replace("\r","");
    data.replace("\n","");
    Serial.flush();
    if(data != "") mcu.sendDataFromBoardToServer(data);
    Blynk.run();
  }
  mcu.sendDataFromServerToBoard();
}
