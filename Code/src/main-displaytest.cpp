#include <Arduino.h>
#include <SPI.h>
#include "segments.h" 

const uint8_t PIN_SCLK = 13;
const uint8_t PIN_MOSI = 11;
const uint8_t PIN_LATCH = 8;
const uint8_t PIN_BLANK = 9;
const uint8_t PIN_LED1 = 7;
const uint8_t PIN_LED2 = 6;
const uint8_t PIN_SS = 10;

SPISettings settings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
void setup();
void loop();

uint8_t value = 0;

void setup() {

    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LATCH, OUTPUT);
    digitalWrite(PIN_LATCH, LOW);
    pinMode(PIN_BLANK, OUTPUT);
    digitalWrite(PIN_BLANK, LOW);
    pinMode(PIN_SS, OUTPUT);
    SPI.begin();
}

void loop2() {

    uint8_t ones = SEGMENTS[(value & 0x0f)];
    uint8_t tens = SEGMENTS[(value >> 4) & 0x0f];

    digitalWrite(PIN_LATCH, LOW);
    SPI.beginTransaction(settings);
    SPI.transfer(ones);
    SPI.transfer(tens);
    SPI.endTransaction();
    digitalWrite(PIN_LATCH, HIGH);

    value++;
    delay(250);
    digitalWrite(PIN_LED1, value & 0x01);
}

void loop() {
    for (uint8_t i=0;  i<16;  i++) {
        uint16_t data = 1 << i;
        digitalWrite(PIN_LATCH, LOW);
        SPI.beginTransaction(settings);
        SPI.transfer16(data);
        SPI.endTransaction();
        digitalWrite(PIN_LATCH, HIGH);
        digitalWrite(PIN_LED1, data & 0x01);
        delay(250);
    }
    delay(1000);
}