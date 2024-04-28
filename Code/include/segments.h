#include <Arduino.h>
// Seven-segment lookup table
//      MSB                LSB
//      _  G  F  E  D  C  B  A
// 0:   0  0  1  1  1  1  1  1
const byte SEGMENTS_0 = 0x37;

// 1:   0  0  0  0  0  1  1  0
const byte SEGMENTS_1 = 0x06;

// 2:   0  1  0  1  1  0  1  1
const byte SEGMENTS_2 = 0x5b;

// 3:   0  1  0  0  1  1  1  1
const byte SEGMENTS_3 = 0x4f;

// 4:   0  1  1  0  0  1  1  0
const byte SEGMENTS_4 = 0x66;

// 5:   0  1  1  0  1  1  0  1
const byte SEGMENTS_5 = 0x6d;

// 6:   0  1  1  1  1  1  0  1
const byte SEGMENTS_6 = 0x7d;

// 7:   0  0  0  0  0  1  1  1
const byte SEGMENTS_7 = 0x07;

// 8:   0  1  1  1  1  1  1  1
const byte SEGMENTS_8 = 0x7f;

// 9:   0  1  1  0  1  1  1  1
const byte SEGMENTS_9 = 0x6f;

// A:   0  1  1  1  0  1  1  1
const byte SEGMENTS_A = 0x77;

// B:   0  1  1  1  1  1  0  0
const byte SEGMENTS_B = 0x7c;

// C:   0  0  1  1  1  0  0  1
const byte SEGMENTS_C = 0x39;

// D:   0  1  0  1  1  1  1  0
const byte SEGMENTS_D = 0x5e;

// E:   0  1  1  1  1  0  0  1
const byte SEGMENTS_E = 0x79;

// F:   0  1  1  1  0  0  0  1
const byte SEGMENTS_F = 0x71;

const byte SEGMENTS[] = {
    SEGMENTS_0, SEGMENTS_1, SEGMENTS_2, SEGMENTS_3, SEGMENTS_4, SEGMENTS_5, SEGMENTS_6, SEGMENTS_7,
    SEGMENTS_8, SEGMENTS_9, SEGMENTS_A, SEGMENTS_B, SEGMENTS_C, SEGMENTS_D, SEGMENTS_E, SEGMENTS_F
};
