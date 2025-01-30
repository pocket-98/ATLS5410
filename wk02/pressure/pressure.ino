// pressure.ino
// analog in demo
// pocket
// 2025-01-30

#define PIN A2
#define DT 20

// pin diagram:
//
//  A2             GRD
//  |               |
//  | 1           2 |
//  |______(PR)_____|
//  |               |
//
// (PR) is a pressure sensor

const int history_len = 5;
int history[history_len];
int loop_num;

void setup() {
    pinMode(PIN, INPUT);
    Serial.begin(9600);
    int j;
    for (j = 0; j < history_len; j++) {
        history[j] = 0;
    }
}


void loop() {
    int x = analogRead(PIN);
    int j;
    double avg = 0.0;
    for (j = 0; j < history_len - 1; j++) {
        history[j] = history[j + 1];
        avg += history[j];
    }
    history[history_len - 1] = x;
    avg += x;
    avg = avg / history_len;
    if (loop_num % 10 == 0) {
        Serial.print(avg);
        Serial.println();
    }
    delay(DT);
}
