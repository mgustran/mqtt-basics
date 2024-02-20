
void initSerial() {
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  delay(1500);
  println("Booting");
  delay(500);
#endif
}

void print(char msg[]) {
#ifdef SERIAL_DEBUG
  Serial.print(msg);
#endif
}

void println(char msg[]) {
#ifdef SERIAL_DEBUG
  Serial.println(msg);
#endif
}

void print(long msg) {
#ifdef SERIAL_DEBUG
  Serial.print(msg);
#endif
}

void println(long msg) {
#ifdef SERIAL_DEBUG
  Serial.println(msg);
#endif
}

void print(int msg) {
#ifdef SERIAL_DEBUG
  Serial.print(msg);
#endif
}

void println(int msg) {
#ifdef SERIAL_DEBUG
  Serial.println(msg);
#endif
}

void print(boolean msg) {
#ifdef SERIAL_DEBUG
  Serial.print(msg);
#endif
}

void println(boolean msg) {
#ifdef SERIAL_DEBUG
  Serial.println(msg);
#endif
}

void println() {
#ifdef SERIAL_DEBUG
  Serial.println();
#endif
}
