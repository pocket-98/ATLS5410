#include <Stepper.h>

// Define stepper motor steps per revolution
#define STEPS 60

// Create stepper motor instance
Stepper stepper(STEPS, 8, 10, 9, 11);

// Define button pins
const int leftButtonPin = 2;
const int rightButtonPin = 3;
const int fanPin = 5;

// // Variables to track button state
// bool leftState = HIGH;
// bool lastLeftState = HIGH;
// bool rightState = HIGH;
// bool lastRightState = HIGH;

// Track motor direction
int direction = 0; // 1 = clockwise, -1 = counterclockwise

void setup() {
    Serial.begin(115200);
    stepper.setSpeed(600); // Set speed in RPM
    
    pinMode(leftButtonPin, INPUT_PULLUP);
    pinMode(rightButtonPin, INPUT_PULLUP);

    Serial.println("Stepper motor button control initialized.");
}

void loop() {
    // Read button states
    int leftState = digitalRead(leftButtonPin);
    int rightState = digitalRead(rightButtonPin);

    // Check for left button press (state change from HIGH to LOW)
    if (leftState == HIGH && rightState == LOW) {
        direction = -1; // Set direction to counterclockwise
        Serial.println("moving LEFT");
    } else if (rightState == HIGH && leftState == LOW) {
        direction = 1; // Set direction to clockwise
        Serial.println("moving RIGHT");
    } else {
        direction = 0;
    }

    // Move motor in the selected direction
    if (direction == 1) {
        stepper.step(STEPS * 1); // Move clockwise
    } else if (direction == -1) {
        stepper.step(-STEPS * 1); // Move counterclockwise
    } else {
        // dont move
    }

    digitalWrite(fanPin, HIGH);

}
