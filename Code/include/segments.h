#include <stdint.h>

// Seven-segment lookup table
//      MSB                LSB
//      _  A  B  C  D  E  F  G

// 0:   0  1  1  1  1  1  1  0
const byte SEGMENTS_0 = 0x7e;

// 1:   0  0  1  1  0  0  0  0
const byte SEGMENTS_1 = 0x30;

// 2:   0  1  1  0  1  1  0  1
const byte SEGMENTS_2 = 0x6d;

// 3:   0  1  1  1  1  0  0  1
const byte SEGMENTS_3 = 0x79;

// 4:   0  0  1  1  0  0  1  1
const byte SEGMENTS_4 = 0x33;

// 5:   0  1  0  1  1  0  1  1
const byte SEGMENTS_5 = 0x5b;

// 6:   0  1  0  1  1  1  1  1
const byte SEGMENTS_6 = 0x5f;

// 7:   0  1  1  1  0  0  0  0
const byte SEGMENTS_7 = 0x70;

// 8:   0  1  1  1  1  1  1  1
const byte SEGMENTS_8 = 0x7f;

// 9:   0  1  1  1  1  0  1  1
const byte SEGMENTS_9 = 0x7b;

// A:   0  1  1  1  0  1  1  1
const byte SEGMENTS_A = 0x77;

// B:   0  0  0  1  1  1  1  1
const byte SEGMENTS_B = 0x1f;

// C:   0  1  0  0  1  1  1  0
const byte SEGMENTS_C = 0x4e;

// D:   0  0  1  1  1  1  0  1
const byte SEGMENTS_D = 0x3d;

// E:   0  1  0  0  1  1  1  1
const byte SEGMENTS_E = 0x4f;

// F:   0  1  0  0  0  1  1  1
const byte SEGMENTS_F = 0x47;

const byte SEGMENTS[] = {
    SEGMENTS_0, SEGMENTS_1, SEGMENTS_2, SEGMENTS_3, SEGMENTS_4, SEGMENTS_5, SEGMENTS_6, SEGMENTS_7,
    SEGMENTS_8, SEGMENTS_9, SEGMENTS_A, SEGMENTS_B, SEGMENTS_C, SEGMENTS_D, SEGMENTS_E, SEGMENTS_F
};
