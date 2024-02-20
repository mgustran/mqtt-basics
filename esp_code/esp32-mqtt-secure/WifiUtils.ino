
void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  loadCerts();
  
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());
}

void initTime() {
  Serial.print("[TIME] Synchronizing time using NTP");
  configTime(1 * 3600, 0, "es.pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 1000) {
    delay(500);
    Serial.print("*");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("[TIME] Current time: ");
  Serial.println(asctime(&timeinfo));
}



//========= Load MQTT Certs =========
void loadCerts() {

  espClient.setCACert(CA_CERT);
  espClient.setCertificate(CLIENT_CERT); // for client verification
  espClient.setPrivateKey(CLIENT_KEY);	// for client verification

  // espClient.allowSelfSignedCerts();
}
