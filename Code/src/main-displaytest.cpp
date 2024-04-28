#include <Arduino.h>
#include <SPI.h>
#include "segments.h" 

const uint8_t PIN_LED2 = 6;
const uint8_t PIN_LED1 = 7;
const uint8_t PIN_LATCH = 8;
const uint8_t PIN_BLANK = 9;
const uint8_t PIN_SS = 10;
const uint8_t PIN_MOSI = 11;
const uint8_t PIN_SCLK = 13;
const uint8_t PIN_LIGHT = A0; // 14

SPISettings settings = SPISettings(1000000, MSBFIRST, SPI_MODE2);
void setup();
void loop();
void configureADC(uint8_t adcChan);
void configureTimer1(uint16_t ticks);
void configureIllumination();
void processIllumination();

const uint8_t illuminaton_size = 16;
volatile uint16_t illumination[illuminaton_size];
volatile uint16_t illumination_sum = 0;
volatile uint8_t illumination_pos = 0;

volatile bool adcFlag;
volatile uint16_t ADCresult;

uint8_t counter = 0;

void setup() {

    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LATCH, OUTPUT);
    digitalWrite(PIN_LATCH, LOW);
    pinMode(PIN_BLANK, OUTPUT);
    digitalWrite(PIN_BLANK, LOW);
    pinMode(PIN_SS, OUTPUT);
    SPI.begin();

    Serial.begin(112500);

    configureIllumination();

    // Use ADC channel 0, and set a 1 second interval for ADC conversions
    configureADC(0);
    configureTimer1(15625);
}

void loop() {

    // Set brightness
    cli();
    uint16_t temp = illumination_sum;
    sei();
    uint8_t ill = (temp >> 6);
    Serial.println(temp);
    analogWrite(PIN_BLANK, ill);

    uint8_t ones = SEGMENTS[(counter & 0x0f)];
    uint8_t tens = SEGMENTS[(counter >> 4) & 0x0f];

    digitalWrite(PIN_LATCH, LOW);
    SPI.beginTransaction(settings);
    SPI.transfer(ones);
    SPI.transfer(tens);
    SPI.endTransaction();
    digitalWrite(PIN_LATCH, HIGH);

    counter++;
    delay(250);
    digitalWrite(PIN_LED1, counter & 0x01);
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
}

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
}

// Initialize illumination buffer with 1023 (darkness) in each slot,
// and set illumination_sum to match.
void configureIllumination() {
    illumination_pos = 0;
    illumination_sum = 0;
    for (uint8_t i = 0; i < illuminaton_size;  i++) {
        illumination[i] = 1023;
        illumination_sum += 1023;
    }
}

// Replace oldest illumination value with the new value,
// and adjust the sum accordingly.
void processIllumination(uint16_t newVal) {
    uint16_t oldVal = illumination[illumination_pos];
    illumination[illumination_pos] = newVal;
    illumination_sum = illumination_sum - oldVal + newVal;
    illumination_pos = (illumination_pos + 1) % illuminaton_size;
}

// Invoked when ADC conversion complete
ISR( ADC_vect ) { 
 
  ADCresult = ADC;          // Read the ADC
  processIllumination(ADCresult);
  adcFlag   = true;         // set flag
  TIFR1     = ( 1<<OCF1B ); // clear Compare Match B Flag
}
