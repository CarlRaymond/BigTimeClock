// i2c configuration for DS3231

#include <Arduino.h>
const uint8_t DS3231_ADDRESS = 0x68;

const uint8_t DS3231_SECONDS = 0x00;
const uint8_t DS3231_MINUTES = 0x01;
const uint8_t DS3231_HOURS = 0x02;
const uint8_t DS3231_DAY = 0x03;
const uint8_t DS3231_DATE = 0x04;
const uint8_t DS3231_MONTH = 0x05;
const uint8_t DS3231_YEAR = 0x06;
const uint8_t DS3231_AL1_SECONDS = 0x07;
const uint8_t DS3231_AL1_MINUTES = 0x08;
const uint8_t DS3231_AL1_HOURS = 0x09;
const uint8_t DS3231_AL1_DAYDATE = 0x0A;
const uint8_t DS3231_AL2_MINUTES = 0x0B;
const uint8_t DS3231_AL2_HOURS = 0x0C;
const uint8_t DS3231_AL2_DAYDATE = 0x0D;
const uint8_t DS3231_CONTROL = 0x0E;
const uint8_t DS3231_STATUS = 0x0F;
const uint8_t DS3231_AGING = 0x10;
const uint8_t DS3231_TEMP_MSB = 0x11;
const uint8_t DS3231_TEMP_LSB = 0x12;
