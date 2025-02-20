// motor.ino

//L293D motor controller
// https://www.ti.com/lit/ds/symlink/l293d.pdf

// ANALOG SERVO
//#define PIN 9
//#define DT 200
//
//void setup() {
//    pinMode(PIN, OUTPUT);
//}
//
//void loop() {
//    int i;
//    for (i=0; i<256; i+=10) {
//        analogWrite(PIN, i);
//        delay(DT);
//    }
//    for (i=255; i>=0; i-=10) {
//        analogWrite(PIN, i);
//        delay(DT);
//    }
//}

// Stepper Motor
#include <Stepper.h>

const int stepsPerRevolution = 2038;

Stepper s = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
    //
}

void loop() {
    s.setSpeed(5);
    s.step(stepsPerRevolution);
    delay(1000);
    // Rotate CCW quickly at 10 RPM
    s.setSpeed(10);
    s.step(-stepsPerRevolution);
    delay(1000);
}
