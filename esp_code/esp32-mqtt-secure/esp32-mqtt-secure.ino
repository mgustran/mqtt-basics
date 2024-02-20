//#include "esp_camera.h"
//#include <WiFi.h>
#include <WiFiClientSecure.h>  //included WiFiClientSecure does not work!
//#include "src/dependencies/WiFiClientSecure/WiFiClientSecure.h" //using older WiFiClientSecure
#include <time.h>
#include <PubSubClient.h>

//#include "base64.h"
#include "secrets.h"





// Setup Debugging
bool debug = true;


WiFiClientSecure espClient;
PubSubClient client(mqtt_server, mqtt_port, espClient);

unsigned long timeInterval = 500;
unsigned long timeStamp = 0;

#define SERIAL_DEBUG true

time_t now;
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  initWifi();
  initTime();
  initMqtt();
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(millis() - timeStamp > timeInterval ){

    //Serial.println("timed print");

    timeStamp = millis();
  }
}
