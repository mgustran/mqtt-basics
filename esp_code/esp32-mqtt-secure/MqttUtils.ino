void callback(char* topic, byte* payload, unsigned int lng) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String topic_str = String(topic);

  if (strcmp(topic, mqttTopicStateIn_1) == 0) {

      char payloadArray[lng+1];
      for (int i = 0; i < lng; i++)
      {
        payloadArray[i] = (char)payload[i];
      }
      payloadArray[lng] = NULL; // null terminated array

      Serial.println(payloadArray);

    Serial.println("subscribe activated");
    if (strcmp(payloadArray, "on") == 0) {
      Serial.println("subscribe activated with on value");
    }    

  } else if (strncmp(topic, "ham/domotichdeck/in/", strlen("ham/domotichdeck/in/")) == 0) {

     char payloadArray[lng + 1];
     for (int i = 0; i < lng; i++) {
       payloadArray[i] = (char)payload[i];
     }
     payloadArray[lng] = NULL;  // null terminated array
     Serial.println(payloadArray);

     if (strcmp(payloadArray, "on") == 0) {

       // TEST
       String processId = topic_str.substring(topic_str.lastIndexOf("/"));
       client.publish((String(mqttTopicFinish) + processId).c_str(), "test reply", false);
     }

   }


}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(uniqueID)) {
      client.setBufferSize(mqtt_buffer_size);
      Serial.print("connected as ");
      Serial.println(uniqueID);
      // Subscribe
      client.subscribe(mqttTopicStateIn_1);
      client.subscribe(mqttTopicStateIn_2);
      Serial.print("Subscribed to ");
      Serial.print(mqttTopicStateIn_2);
      Serial.print(", ");
      Serial.println(mqttTopicStateIn_2);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void initMqtt() {
  client.setClient(espClient);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setBufferSize(mqtt_buffer_size);
}
