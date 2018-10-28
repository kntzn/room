#pragma once
#ifndef PREDEFINED_OFF

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |=  _BV(bit))

// ----------------------------------------
// LED strip controller defines

// FastLED lib defines
#define FASTLED_ALLOW_INTERRUPTS 1

// Number of leds
#define N_LEDS_MAIN 164
#define N_LEDS_TABLE 42

#define N_LEDS_SEC_0 53
#define N_LEDS_SEC_1 39
#define N_LEDS_SEC_2 72

#define N_LEDS_SUBSEC 9
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
#define VU_BRIGHTNESS_MIN 64
#define VU_BRIGHTNESS_MAX 255
#define HSV_VU_START HUE_RED
#define HSV_VU_END HUE_GREEN
// ----------------------------------------


// FREQ mode parameters

// Maximum brightness of leds in Freq mode
#define FREQ_BRIGHTNESS_MAX 255
// Minimum brightness of leds in Freq mode
#define FREQ_BRIGHTNESS_MIN 50
// High frequences color
#define HSV_HIGH_FREQ_COLOR HUE_YELLOW
// Mid frequences color
#define HSV_MID_FREQ_COLOR HUE_GREEN
// Low frequences color
#define HSV_LOW_FREQ_COLOR HUE_RED
// ----------------------------------------


// Night mode color
#define NIGHT_COLOR HUE_PURPLE
// Night fade time (min)
#define NIGHT_FADE_TIME 10
// Max brightness of night mode strip
#define NIGHT_BRIGHTNESS_MAX 100

// RVD full rise time (min)
#define RVD_RISE_TIME 0.3
// Rand amplitude value (does not change anything)
#define RND_AMPL 50


// !LED strip controller defines
// ----------------------------------------

// ----------------------------------------
// Analyzer defines

#define LOG_OUT 1

// Pinout
#define MIC_INPUT A1
#define MIC_INPUT_FREQ A0
#define JACK_INPUT A3
#define JACK_INPUT_FREQ A2
// ----------------------------------------
// VU meter parameters

// Ratio of max volume and average volume
#define RATIO_MAX_TO_AVG 1.5f
// Average volume smoothness
#define AVER_VOLUME_SMOOTH 0.995f
// Smoothness of volume bar
#define SMOOTH_VU 0.8f
// Power of signal
#define EXP 1
// Maximum signal from analyzer
#define ANALOG_VU_MAX 1000
// Maxumum value of volume after being powered
#define VU_OUT_MAX pow (ANALOG_VU_MAX, EXP)
// ----------------------------------------
// frequences analyzer's parameters

// Filter's parameters

// Minimum loudness of frequency
#define LOW_PASS_FREQ 28
// Smoothness of the output
#define SMOOTH_FREQ 0.7f

// Spectrum size
#define SPECTRUM_SIZE 32
#define FHT_N SPECTRUM_SIZE*2
// Max output of frequences analyzer
#define FREQ_MAX 1000

// Number of low freqs
#define N_LOW 6
// Number of mid freqs
#define N_MID 5
// Number of high freqs
#define N_HIGH 21

// Check for incorrect ranges
#if N_LOW + N_MID + N_HIGH != SPECTRUM_SIZE
#error Incorrect frequencies ranges
#endif // N_LOW + N_MID + N_HIGH != SPECTRUM_SIZE

// Average frequency volume smoothness
#define AVER_FREQ_VOLUME_SMOOTH 0.8f

#define FREQ_MODE_FADE_C 0.9f


// ----------------------------------------

// !Analyzer predefined parameters
// ----------------------------------------

// ----------------------------------------
// Other
#define BAUD_RATE_SERIAL 9600
// !Other
// ----------------------------------------

#endif // !PREDEFINED_OFF


