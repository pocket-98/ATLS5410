#include <Arduino.h>
#include <IRremote.hpp>


#define LED_BUILTIN 2
#define IR_SEND_PIN 2
//#define IR_RECEIVE_PIN 5

#define POWER_PIN 14
#define POWER_CMD 0x7
#define POWER_ADDR 0x2

#define VOLUP_PIN 26
#define VOLUP_CMD 0x7
#define VOLUP_ADDR 0x7

#define VOLDOWN_PIN 33
#define VOLDOWN_CMD 0x7
#define VOLDOWN_ADDR 0xB

void setup() {
    pinMode(IR_SEND_PIN, OUTPUT);
    pinMode(POWER_PIN, INPUT);

    Serial.begin(115200);   // Status message will be sent to PC at 9600 baud
    while (!Serial);

    //IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    //Serial.print("Ready to receive IR signals of protocols: ");
    //printActiveIRProtocols(&Serial);
    //Serial.print("at pin ");
    //Serial.println(IR_RECEIVE_PIN);

    IrSender.begin(IR_SEND_PIN);
    Serial.print("Send IR signals at pin ");
    Serial.println(IR_SEND_PIN);
}


void loop() {

    bool power = analogRead(POWER_PIN) < 1024;
    if (power) {
        Serial.println("power");
        IrSender.sendSamsung(POWER_CMD, POWER_ADDR, 0);
    }

    bool volup = analogRead(VOLUP_PIN) < 1024;
    if (volup) {
        Serial.println("volume up");
        IrSender.sendSamsung(VOLUP_CMD, VOLUP_ADDR, 0);
    }

    bool voldown = analogRead(VOLDOWN_PIN) < 1024;
    if (voldown) {
        Serial.println("volume down");
        IrSender.sendSamsung(VOLDOWN_CMD, VOLDOWN_ADDR, 0);
    }


    //if (IrReceiver.decode()) {
    //    Serial.println();
    //    Serial.println();
    //    IrReceiver.printIRResultShort(&Serial);

    //    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
    //    } else {
    //        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
    //            Serial.println("Received noise or an unknown (or not yet enabled) protocol");
    //        }
    //        Serial.println();
    //        IrReceiver.printIRSendUsage(&Serial);
    //        Serial.println();
    //        Serial.println("Raw result in internal ticks (50 us) - with leading gap");
    //        IrReceiver.printIRResultRawFormatted(&Serial, false);
    //        Serial.println("Raw result in microseconds - with leading gap");
    //        IrReceiver.printIRResultRawFormatted(&Serial, true);
    //        Serial.println();
    //        Serial.print("Result as internal 8bit ticks (50 us) array - compensated with MARK_EXCESS_MICROS=");
    //        Serial.println(MARK_EXCESS_MICROS);
    //        IrReceiver.compensateAndPrintIRResultAsCArray(&Serial, false);
    //        Serial.print("Result as microseconds array - compensated with MARK_EXCESS_MICROS=");
    //        Serial.println(MARK_EXCESS_MICROS);
    //        IrReceiver.compensateAndPrintIRResultAsCArray(&Serial, true);
    //        IrReceiver.printIRResultAsCVariables(&Serial);
    //        Serial.println();

    //        IrReceiver.compensateAndPrintIRResultAsPronto(&Serial);

    //    }
    //    IrReceiver.resume();
    //}
}

