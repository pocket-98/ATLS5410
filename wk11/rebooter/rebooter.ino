#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <IRremote.hpp>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif


//////////////////////////////
// wifi and target settings //
//////////////////////////////
#define WIFI_SSID "asdfasfasfREDACTED"
#define WIFI_PASSWORD "ligmaREDACTED"

static const char* target_ip = "10.0.0.22";
static const unsigned int target_port = 22;

#define WIFI_TIME_CHECK_DELTA 45000
#define TARGET_TIME_CHECK_DELTA 60000

#define TARGET_RELAY_PIN 5


////////////////////////
// ir remote settings //
////////////////////////
#define IR_SEND_PIN 2
//#define IR_RECEIVE_PIN 5

// samsung nec2 command codes
// https://github.com/zmoteio/irdb.tk/blob/master/codes/Samsung/TV/7%2C7.csv
#define POWER_PIN 14
#define POWER_CMD 0x7
#define POWER_ADDR 0x2

#define VOLUP_PIN 26
#define VOLUP_CMD 0x7
#define VOLUP_ADDR 0x7

#define VOLDOWN_PIN 33
#define VOLDOWN_CMD 0x7
#define VOLDOWN_ADDR 0xB


//////////
// vars //
//////////
static long unsigned int time_last_wifi_check;
static const unsigned int wifi_history_len = 5;
static int wifi_history[wifi_history_len];

static long unsigned int time_last_target_check;
static const unsigned int target_history_len = 5;
static int target_history[target_history_len];

static HTTPClient http_client;
static WiFiClient esp_client;

///////////
// setup //
///////////
void setup() {
    // serial
    Serial.begin(115200);
    //while (!Serial);

    // pins
    pinMode(TARGET_RELAY_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(IR_SEND_PIN, OUTPUT);
    pinMode(POWER_PIN, INPUT);
    pinMode(VOLUP_PIN, INPUT);
    pinMode(VOLDOWN_PIN, INPUT);
    digitalWrite(TARGET_RELAY_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);

    IrSender.begin(IR_SEND_PIN);
    Serial.println();
    Serial.print("Send IR signals at pin ");
    Serial.println(IR_SEND_PIN);

    // wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int max_tries = 600;
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
        if ((max_tries--) < 1) {
            Serial.println();
            Serial.println("rebooting");
            ESP.restart();
        }
        if (max_tries % 2 == 0) {
            digitalWrite(LED_BUILTIN, HIGH);
        } else {
            digitalWrite(LED_BUILTIN, LOW);
        }
    }
    digitalWrite(LED_BUILTIN, LOW);

    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    time_last_wifi_check = 0;

    // wifi status history
    int j;
    for (j=0; j<wifi_history_len; ++j) {
        wifi_history[j] = 0;
    }

    // target status history
    for (j=0; j<target_history_len; ++j) {
        target_history[j] = 0;
    }
}


void loop() {

    // read button inputs
    bool power = digitalRead(POWER_PIN) == LOW;
    if (power) {
        Serial.println("power");
        IrSender.sendSamsung(POWER_CMD, POWER_ADDR, 0);
    }

    bool volup = digitalRead(VOLUP_PIN) == LOW;
    if (volup) {
        Serial.println("volume up");
        IrSender.sendSamsung(VOLUP_CMD, VOLUP_ADDR, 0);
    }

    bool voldown = digitalRead(VOLDOWN_PIN) == LOW;
    if (voldown) {
        Serial.println("volume down");
        IrSender.sendSamsung(VOLDOWN_CMD, VOLDOWN_ADDR, 0);
    }

    // check wifi status
    if (millis() >= WIFI_TIME_CHECK_DELTA + time_last_wifi_check) {
        time_last_wifi_check = millis();
        Serial.println("checking wifi");
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);

        shift_queue(wifi_history, wifi_history_len);
        wifi_history[0] = get_wifi_status();

        Serial.print("wifi_history = ");
        print_arr(wifi_history, wifi_history_len);
        Serial.println();

        if (none_are_zeroes(wifi_history, wifi_history_len)) {
            Serial.print("wifi has been off for ");
            Serial.print(wifi_history_len);
            Serial.println(" iterations, rebooting");
            ESP.restart();
        }
    }

    // check target ip/port status
    if (millis() >= TARGET_TIME_CHECK_DELTA + time_last_target_check) {
        time_last_target_check = millis();
        Serial.print("checking target ");
        Serial.print(target_ip);
        Serial.print(":");
        Serial.print(target_port);
        Serial.println();
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);

        shift_queue(target_history, target_history_len);
        target_history[0] = get_target_status(target_ip, target_port);

        Serial.print("target_history = ");
        print_arr(target_history, target_history_len);
        Serial.println();

        if (none_are_zeroes(target_history, target_history_len)) {
            Serial.print("target has been off for ");
            Serial.print(target_history_len);
            Serial.println(" iterations:");
            Serial.println("sending command to trigger relay");
            digitalWrite(TARGET_RELAY_PIN, HIGH);
            delay(500);
            digitalWrite(TARGET_RELAY_PIN, LOW);
            int j;
            for (j=0; j<target_history_len; ++j) {
                target_history[j] = 0;
            }
        }
    }
}

void print_arr(int* arr, unsigned int len) {
    int j;
    Serial.print("[ ");
    for (j=0; j<len; ++j) {
        Serial.print(arr[j]);
        Serial.print(" ");
    }
    Serial.print("]");
}

void shift_queue(int* arr, unsigned int len) {
    int j;
    for (j=len-1; j>0; --j) {
        arr[j] = arr[j-1];
    }
}

bool none_are_zeroes(int* arr, unsigned int len) {
    int j;
    for (j=0; j<len; ++j) {
        if (arr[j] == 0) {
            return false;
        }
    }
    return true;
}

int get_wifi_status() {
    // connected and online = 0
    // disconnected = 1
    // connected no internet = 2
    bool connected;
    if (WiFi.status() == WL_CONNECTED) {
        connected = true;
    } else {
        connected = false;
    }

    if (connected) {
        const char* google = "https://www.google.com/";
        http_client.begin(google);
        int http_code = http_client.GET();
        //Serial.print("http response code: ");
        //Serial.println(http_code);
        if (http_code > 0) {
            //Serial.println(http_client.getString());
            if (http_client.getString().indexOf("<html") > 0) {
                http_client.end();
                return 0;
            } else {
                http_client.end();
                return 3;
            }
        } else {
            http_client.end();
            return 2;
        }
    } else {
        return 1;
    }
}

int get_target_status(const char* ip, const unsigned int port) {
    // target online = 0
    // target offline = 1
    int status;
    String get;
    if (!esp_client.connected()) {
        if (esp_client.connect(ip, port)) {
            char c;
            while (esp_client.available() > 0) {
                c = esp_client.read();
                get += c;
                Serial.print(c);
            }
            status = 0;
            esp_client.stop();
        } else {
            status = 1;
        }
    }
    return status;
}
