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
void configureADC(uint8_t adcChan);
void configureTimer1(uint16_t ticks);
void configureClock();
void configureIllumination();
void processIllumination();
void readtime();

const uint8_t illuminaton_size = 16;
uint16_t illumination[illuminaton_size];
uint16_t illumination_sum = 0;
uint8_t illumination_pos = 0;

volatile bool adcFlag;
volatile uint16_t ADCresult;


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

    configureIllumination();

    configureADC(0);
    configureClock();

    // Set a 1 second interval for ADC conversions
    configureTimer1(15625);
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

    if (adcFlag) {
        adcFlag = false;
        Serial.print( F("Ill = " ));
        Serial.println( illumination_sum );
        Serial.println();
    }
}

void configureIllumination() {
    illumination_pos = 0;
    illumination_sum = 0;
    for (uint8_t i = 0; i < illuminaton_size;  i++) {
        illumination[i] = 1023;
        illumination_sum += 1023;
    }
}

void processIllumination(uint16_t newVal) {
    uint16_t oldVal = illumination[illumination_pos];
    illumination[illumination_pos] = newVal;
    illumination_sum = illumination_sum - oldVal + newVal;
    illumination_pos = (illumination_pos + 1) % illuminaton_size;
}

void configureADC(uint8_t adcChan) {
  
  cli(); 
  // enable adc, auto trigger, interrupt enable, prescale=128
  ADCSRA = (( 1<<ADEN ) | ( 1<<ADATE ) | ( 1<<ADIE ) | ( 1<<ADPS2 ) | ( 1<<ADPS1 ) | ( 1<<ADPS0 )); 
  // Timer/Counter 1 Compare Match B 
  ADCSRB = (( 1<<ADTS2 ) | ( 1<<ADTS0 ));
  // ref=AVcc + adc chan   
  ADMUX  = (( 1<<REFS0 ) + adcChan );
  sei();
  
} // initialize_ADC

void configureTimer1 (uint16_t ticks) {
  
  TCCR1A  = 0;
  TCCR1B  = 0;
  TCNT1   = 0;
  TIMSK1  = 0;
  
  TCCR1B  = ( 1 << WGM12 ) ;  // Configure for CTC mode 4 OCR1A=TOP
  
  OCR1B   = ticks;            // compare value
  OCR1A   = ticks;            // Set CTC TOP value, must be >= OCR1B
  
  // start timer, give it a clock
  TCCR1B |= (( 1 << CS10 ) | ( 1 << CS12 )) ;  // Fcpu/1024, 64us tick @ 16 MHz
  
} // TimerOneInit


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

void configureClock() {
    // Set control register to 0
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(DS3231_CONTROL);
    Wire.write(0);
    Wire.endTransmission();
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

ISR( ADC_vect ) // ADC conversion complete 
{
  
  ADCresult = ADC;          // Read the ADC
  processIllumination(ADCresult);
  adcFlag   = true;         // set flag  
  TIFR1     = ( 1<<OCF1B ); // clear Compare Match B Flag
  
} // ISR
