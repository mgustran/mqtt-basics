void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  initTime();
  loadCerts();
  
  println("Ready");
  print("IP address: ");
  println(WiFi.localIP().toString().c_str());
}

void loadCerts() {
  // BearSSL::X509List *serverCert = new BearSSL::X509List(CA_CERT);
  BearSSL::X509List *clientCert = new BearSSL::X509List(CLIENT_CERT);
  BearSSL::PrivateKey *clientKey = new BearSSL::PrivateKey(CLIENT_KEY);

  espClient.allowSelfSignedCerts();
  espClient.setFingerprint(FINGERPRINT);

  //espClient.setTrustAnchors(serverCert);
  espClient.setClientRSACert(clientCert, clientKey);

}

void initTime() {
  print("[TIME] Synchronizing time using NTP");
  configTime(1 * 3600, 0, "es.pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 1000) {
    delay(500);
    print("*");
    now = time(nullptr);
  }
  println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  print("Current time: ");
  print(asctime(&timeinfo));
}
