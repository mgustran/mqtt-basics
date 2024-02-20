#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <time.h>


#include "secrets.h"


#define SERIAL_DEBUG  // comment to disable


// *********************************   WIFI CONFIG   *********************************

BearSSL::WiFiClientSecure espClient;
PubSubClient client;

const char* mqttTopicIncomingCallIn = "ham/domotichdeck/in";
const char* mqttTopicIncomingCallOut = "ham/domotichdeck/out";


// *********************************   INTERVALS   *********************************
const unsigned long timeInterval = 5000;
unsigned long timeStamp = 0;

// *********************************   VARIABLES   *********************************
char err_buf[256];


// *********************************   SETUP   *********************************

void setup() {

  initSerial();

  initWifi();
  initMqtt();

  delay(1000);
}


// *********************************   LOOP   *********************************

void loop() {
  checkMqttConnection();


  if(millis() - timeStamp > timeInterval ) {

    println("timed print");
    client.publish(mqttTopicIncomingCallOut, "timed publish", false);

    timeStamp = millis();
  }
}
