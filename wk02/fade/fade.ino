// fade.ino

#define PIN 9
#define DT 20

void setup() {
    pinMode(PIN, OUTPUT);
}

void loop() {
    int i;
    for (i=0; i<256; ++i) {
        analogWrite(PIN, i);
        delay(DT);
    }
    for (i=255; i>=0; --i) {
        analogWrite(PIN, i);
        delay(DT);
    }
}
