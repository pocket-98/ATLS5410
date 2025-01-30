// altitude.ino
// bmp180 altitude sensor
// pocket
// 2025-01-30

// https://www.adafruit.com/product/1603
// https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

// i2c:
// https://docs.arduino.cc/language-reference/en/functions/communication/wire/
#include <Wire.h>

// bmp 180:
// https://lastminuteengineers.com/bmp180-arduino-tutorial/
// https://github.com/adafruit/Adafruit-BMP085-Library
#include <Adafruit_BMP085.h>
// arduino-cli lib install "Adafruit BMP085 Library"

#define CLOCK 100000
#define DT 200

// pin diagram:
//
// 3.3             GRD
//  |               |
//  | 1           2 |
//  |_____(BMP)_____|
//  |               |
//
//
// SCL             GRD
//  |               |
//  | 3             |
//  |___(R1)__(BMP) |
//  |               |
//
//
// SDA             GRD
//  |               |
//  | 4             |
//  |___(R2)__(BMP) |
//  |               |
//
// (BMP) is an altitude pressure sensor
// (BMP) 1: 3.3v
// (BMP) 2: ground
// (BMP) 3: SCL
// (BMP) 4: SDA
// (R1) 330ohm resistor
// (R2) 330ohm resistor

Adafruit_BMP085 bmp;

const int history_len = 5;
int history[history_len];
int loop_num;

void setup() {
    Serial.begin(9600);
    Serial.println("setup");

    Serial.println("init bmp");
    bmp.begin();

    int j;
    for (j = 0; j < history_len; j++) {
        history[j] = 0;
    }

    loop_num = 1;
}


void loop() {
    int a = bmp.readAltitude(); // meters
    int p = bmp.readPressure(); // Pascals
    int t = bmp.readTemperature(); // deg C

    int j;
    double avg = 0.0;
    for (j = 0; j < history_len - 1; j++) {
        history[j] = history[j + 1];
        avg += history[j];
    }
    history[history_len - 1] = a;
    avg += a;
    avg = avg / history_len;

    if (loop_num % 10 == 0) {
        Serial.println("{");
        Serial.print("  \"altitude\": ");
        Serial.print(a);
        Serial.println(",");
        Serial.print("  \"pressure\": ");
        Serial.print(p);
        Serial.println(",");
        Serial.print("  \"temperature\": ");
        Serial.print(t);
        Serial.println();
        Serial.println("}");

        loop_num = 0;
    }
    loop_num++;
    delay(DT);
}
