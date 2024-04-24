/*
 * Simple queue for button presses received from the remote.
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include "PinDefinitionsAndMore_IRMP.h"
#include "remoteinfo.h"
#include "ds3231.h"

#define IRMP_SUPPORT_FAN_PROTOCOL       1
#define IRMP_USE_COMPLETE_CALLBACK      1
#include <irmp.hpp>

const uint8_t LED1_PIN = 6;
const uint8_t LED2_PIN = 7;

// Data about received IR code
IRMP_DATA irmp_data;

#define USE_NO_LCD

// IR handler
void handleReceivedIRData();

// Simple queue for button presses
const uint8_t queue_size = 16;
volatile uint8_t queue_in_pos = 0;
volatile uint8_t queue_out_pos = 0;
volatile uint8_t queue_free = queue_size;
char queue[queue_size];
bool enqueue(char c);
bool dequeue(char &c);
void readtime();

void setup() {

    DDRD = 0b11000000;

    Wire.begin();
    Wire.setClock(400000);
    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    //Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRMP));

    irmp_init();
    //irmp_irsnd_LEDFeedback(true); // Enable receive signal feedback at LED_BUILTIN
    irmp_register_complete_callback_function(&handleReceivedIRData);


}

void blink(uint8_t n) {
    while (n > 0) {
        PORTD |= 0b10000000;
        delay(25);
        PORTD &= 0b01111111;
        delay(100);
        n--;
    }
}

void loop() {
    char c;
    int light;
    if (dequeue(c)) {
        Serial.print(c);
        switch(c) {
            case '1':
                light = analogRead(A0);
                Serial.print(F(": "));
                Serial.print(light);
                break;

            case '2':
                Serial.print(F(": "));
                readtime();
                break;
        }
        Serial.println();
        blink(c - '0');
    }
}

void readtime() {
    byte buffer[19];

    // Read all registers from DS3231. Set register pointer to 0.
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(0);
    Wire.endTransmission(false);

    // Read 19 bytes
    Wire.requestFrom(DS3231_ADDRESS, (byte)19);
    Wire.readBytes(buffer, 19);

    for (int8_t i=0;  i<19;  i++) {
        byte v = buffer[i];
        //Serial.print(F(" 0x"));
        Serial.print(' ');
        if (v < 16) {
            Serial.print("0");
        }
        Serial.print(v, HEX);
    }
}

bool enqueue(char c) {
    bool retval = false;
    if (queue_free > 0) {
        cli();
        if (queue_free > 0) {
            queue[queue_in_pos] = c;
            queue_in_pos = (queue_in_pos+1) % queue_size;
            queue_free--;
            retval = true;
        }
        sei();
    }

    return retval;
}

bool dequeue(char &out) {
    bool retval = false;
    if (queue_free < queue_size) {
        cli();
        if (queue_free < queue_size) {
            out = queue[queue_out_pos];
            queue_out_pos = (queue_out_pos+1) % queue_size;
            queue_free++;
            retval = true;
        }
        sei();
    }
    return retval;
}

void handleReceivedIRData() {
    PORTD |= 0b0100000;
    irmp_get_data(&irmp_data);
    if (irmp_data.flags & IRMP_FLAG_REPETITION) {
        return;        
    }
    else {
        switch (irmp_data.command) {
            case BUTTON_1:
                enqueue('1');
                break;
            case BUTTON_2:
                enqueue('2');
                break;
            case BUTTON_3:
                enqueue('3');
                break;
            case BUTTON_4:
                enqueue('4');
                break;
            case BUTTON_5:
                enqueue('5');
                break;
            case BUTTON_6:
                enqueue('6');
                break;
        }
    }
    PORTD &= 0b10111111;
}