#pragma once

#ifndef PREDEFINED_OFF

// ----------------------------------------
// LED strip controller defines

// FastLED lib defines
#define FASTLED_ALLOW_INTERRUPTS 1

// Number of leds
#define N_LEDS_MAIN 164
#define N_LEDS_TABLE 42
// ----------------------------------------

// Pinout
#define STRIP_DATA_MAIN 35
#define STRIP_DATA_TABLE 37
// ----------------------------------------



// HSV parameters
#define MAX_BRIGHTNESS 255
#define MAX_SATURATION 255
// ----------------------------------------

// Modes' defines
#define SWITCH_BRIGHTNESS 2
#define N_PREDEFINED_COLORS 147
// ----------------------------------------

// VU mode parameters
#define VU_BRIGHTNESS_MIN 127
#define VU_BRIGHTNESS_MAX 255
#define HSV_VU_START HUE_RED
#define HSV_VU_END HUE_GREEN
// ----------------------------------------


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
// ----------------------------------------


// Night mode color
#define NIGHT_COLOR HUE_PURPLE
// Night fade time (min)
#define NIGHT_FADE_TIME 10
// Max brightness of night mode strip
#define NIGHT_BRIGHTNESS_MAX 100


// !LED strip controller defines
// ----------------------------------------

// ----------------------------------------
// Analyzer defines

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |=  _BV(bit))

#define LOG_OUT 1

// Pinout
#define MIC_INPUT A1
#define MIC_INPUT_FREQ A0
#define JACK_INPUT A2
#define JACK_INPUT_FREQ A3
// ----------------------------------------

// VU meter parameters

// Ratio of max volume and average volume
#define RATIO_MAX_TO_AVG 1.5f
// Average volume smoothness
#define AVER_VOLUME_SMOOTH 0.95f
// Smoothness of volume bar
#define SMOOTH_VU 0.8f
// Power of signal
#define EXP 1.1
// Maximum signal from analyzer
#define ANALOG_VU_MAX 1000
// Maxumum value of volume after being powered
#define VU_OUT_MAX pow (ANALOG_VU_MAX, EXP)
// ----------------------------------------

// Freqences analyzer's parameters

// Spectrum size
#define SPECTRUM_SIZE 32
#define FHT_N SPECTRUM_SIZE*2
// Max output of freqences analyzer
#define FREQ_MAX 1024
// ----------------------------------------

// !Analyzer predefined parameters
// ----------------------------------------

// ----------------------------------------
// Other
#define BAUD_RATE_SERIAL 9600
// !Other
// ----------------------------------------

#endif // !PREDEFINED_OFF


