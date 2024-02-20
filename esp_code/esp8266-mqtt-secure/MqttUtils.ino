void initMqtt() {
  client.setClient(espClient);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  // client.setBufferSize(2048);
}

void checkMqttConnection() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(esp_id)) {
//    if (client.connect(esp_id, mqtt_user, mqtt_password)) {
      println("connected");
      println(client.state());
      client.subscribe(mqttTopicIncomingCallIn);
      delay(1000);
    } else {
      print("failed, rc=");
      print(client.state());
      println(" try again in 5 seconds");
      espClient.getLastSSLError(err_buf, sizeof(err_buf));
      print("SSL error: ");
      println(err_buf);
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int lng) {
  print("Message arrived [");
  print(topic);
  print("] ");

  char payloadArray[lng+1];
  for (int i = 0; i < lng; i++) 
  {
    payloadArray[i] = (char)payload[i];
  }
  payloadArray[lng] = NULL; // null terminated array

  println(payloadArray);

  if (strcmp(topic, mqttTopicIncomingCallIn) == 0) {

    if (strcmp(payloadArray, "on") == 0) {
      println("call in on");
    }    

  }


}
