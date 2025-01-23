// blink.ino

#define PIN 13
#define DT 1000

void setup() {
  pinMode(PIN, OUTPUT);
}

void loop() {
  digitalWrite(PIN, HIGH);
  delay(DT);
  digitalWrite(PIN, LOW);
  delay(DT);
}
