#pragma once

// LED strip controller predefined parameters

// FastLED lib defines
#define FASTLED_ALLOW_INTERRUPTS 1

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

// Modes defines
#define SWITCH_BRIGHTNESS 25

// VU mode parameters
#define VU_BRIGHTNESS_MIN 127
#define VU_BRIGHTNESS_MAX 255

// FREQ mode parameters

// Maximum brightness of leds in Freq mode
#define FREQ_BRIGHTNESS_MAX 255

// Minimum brightness of leds in Freq mode
#define FREQ_BRIGHTNESS_MIN 50

// High freqences color
#define HSV_HIGH_FREQ_COLOR HUE_YELLOW
// Mid freqences color
#define HSV_MID_FREQ_COLOR HUE_GREEN
// Low freqences color
#define HSV_LOW_FREQ_COLOR HUE_RED

// !LED strip controller predefined parameters


// Analyzer predefined parameters

// VU meter parameters

// Power of signal
#define EXP 1
// Maximum signal from analyzer
#define ANALOG_VU_MAX 1024
// Maxumum value of VU value after power
#define VU_OUT_MAX pow (ANALOG_VU_MAX, EXP)

// Freqences analyzer's parameters

// max output of freqences analyzer
#define FREQ_MAX 1024

// !Analyzer predefined parameters
