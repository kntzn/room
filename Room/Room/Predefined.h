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
#define MAX_SATURATION 255
#define HSV_VU_START HUE_RED
#define HSV_VU_END HUE_GREEN

// FREQ mode parameters
#define FREQ_BRIGHTNESS_MAX 255
#define FREQ_BRIGHTNESS_MIN 25

#define HSV_HIGH_FREQ_COLOR HUE_YELLOW
#define HSV_MID_FREQ_COLOR HUE_GREEN
#define HSV_LOW_FREQ_COLOR HUE_RED

// !LED strip controller predefined parameters



// Analyzer predefined parameters

// VU meter parameters

// Power of signal
#define EXP 1
// Maximum signal from analyzer
#define ANALOG_VU_MAX 1000
// Maxumum value of VU value after power
#define VU_OUT_MAX pow (ANALOG_VU_MAX, EXP)

// Freqences analyzer's parameters

// max output of freqences analyzer
#define FREQ_MAX 1000

// !Analyzer predefined parameters
