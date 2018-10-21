#pragma once

// LED strip controller predefined parameters

// Number of leds
#define N_LEDS_MAIN 164
#define N_LEDS_TABLE 42

// Pinout
#define STRIP_DATA_MAIN 13
#define STRIP_DATA_TABLE 12

// HSV parameters
#define MAX_BRIGHTNESS 255
#define HSV_VU_START 0
#define HSV_VU_END 100

// !LED strip controller predefined parameters

// Analyzer predefined parameters

// VU meter parameters

// Power of signal
#define EXP 1
// Maximum signal from analyzer
#define ANALOG_VU_MAX 1000
// Maxumum value of VU value after power
#define VU_OUT_MAX pow (ANALOG_VU_MAX, EXP)
