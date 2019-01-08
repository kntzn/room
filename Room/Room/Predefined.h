#pragma once
#ifndef PREDEFINED_OFF

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |=  _BV(bit))

// ----------------------------------------
// LED strip controller defines

// FastLED lib defines


// Number of leds
#define N_LEDS_MAIN 164
#define N_LEDS_TABLE 42

#define N_SEC 3
#define N_LEDS_SEC_0 53
#define N_LEDS_SEC_1 39
#define N_LEDS_SEC_2 72

#define N_LEDS_SUBSEC 9
// ----------------------------------------

// ----------------------------------------


// HSV parameters
#define MAX_BRIGHTNESS 255
#define MAX_SATURATION 255
// ----------------------------------------

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
#define NIGHT_COLOR CRGB (45, 10, 34)
// Night fade time (min)
#define NIGHT_FADE_TIME 10
// Max brightness of night mode strip
#define NIGHT_BRIGHTNESS_MAX 100

// RVD full rise time (min)
#define RVD_RISE_TIME 0.3
// Rand amplitude value (does not change anything)
#define RND_AMPL 50

// Rise mode color
#define RISE_COLOR CRGB (255, 170, 0)
// Rise mode stage rise time (min)
#define RISE_MODE_RISE_TIME 1.5f
// Max brightness of night mode strip
#define RISE_BRIGHTNESS_MAX 255

// Time between color switches (min)
#define OLDSCHOOL_SWITCH_TIME 0.25f

// Time of fade and rise while switch (secs)
#define FADE_SWITCH_TIME 2.f

// Number of RVD_RND colors
#define N_COLORS_RVD 6

// Switch fade time (sec)
#define MODE_SWITCH_FADE_TIME 1

// !LED strip controller defines
// ----------------------------------------

// ----------------------------------------
// Analyzer defines

#define LOG_OUT 1

// ----------------------------------------
// VU meter parameters

// Ratio of max volume and average volume
#define RATIO_MAX_TO_AVG 1.8f
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
#define LOW_PASS_FREQ 40
// Smoothness of the output
#define SMOOTH_FREQ 0.5f

// Spectrum size
#define SPECTRUM_SIZE 32
#define FHT_N SPECTRUM_SIZE*2
// Max output of frequences analyzer
#define FREQ_MAX 1000


// Lowest frequency
#define LOWEST 2
// Number of low freqs
#define N_LOW 1
// Number of mid freqs
#define N_MID 6
// Number of high freqs
#define N_HIGH 23

// Check for incorrect ranges
#if N_LOW + N_MID + N_HIGH != SPECTRUM_SIZE - LOWEST
#error Incorrect frequencies ranges
#endif // N_LOW + N_MID + N_HIGH != SPECTRUM_SIZE

// Average frequency volume smoothness
#define AVER_FREQ_VOLUME_SMOOTH 0.8f

#define FREQ_MODE_FADE_C 50.f

// Signal avialable timeout (secs)
#define ANALYZER_TIMEOUT 15

// ----------------------------------------

// !Analyzer predefined parameters
// ----------------------------------------

// ----------------------------------------
// Window

// Servo speeds
#define SERVO_NEGATIVE_SPEED 800
#define SERVO_POSITIVE_SPEED 2300

// Time, required to rotate the blinds by 180 deg
#define FULL_OPEN_TIME 1.8f

// Brightness threshold
#define BRIGHTNESS_TH 50

// Brightness value update period (sec)
#define BRIGHTNESS_UPDATE_PERIOD 15
// Number of measurements
#define N_MES 100

// !Window
// ----------------------------------------

// ----------------------------------------
// Power Supply

// Reference voltage
#define VCC_REF 1.0

// Coefficients
#define U_TO_I_K 2.82702703
#define U_TO_I_B 0.35

#define VOLTAGE_AMPL_FACTOR 1.5f

// !Power Supply
// ----------------------------------------

// ----------------------------------------
// Hardware Monitor
#define SERIAL_HW_MONITOR Serial1

// Updates of HWM screen per second
#define UPS_HWM 1

// Timeout in seconds
#define HWM_TIMEOUT 3

// !Hardware Monitor
// ----------------------------------------

// ----------------------------------------
// Other
#define BAUD_RATE_SERIAL 9600

// Door cap sensor threshold
#define DOOR_CAP_TH 45

// !Other
// ----------------------------------------

// Pinout
// Not connected pin used to generate random seed
#define NC A9

#define CAP_SENSOR_DOOR A8

#define BUTT_LEFT 29
#define BUTT_MIDL 27
#define BUTT_RGHT 25

#define STRIP_DATA_MAIN 35
#define STRIP_DATA_TABLE 37

#define RELAY_LAMP 33
#define RELAY_TORCHERE 31

#define JACK_INPUT A0
#define JACK_INPUT_FREQ A1

#define WINDOW_OUTPUT 47
#define LIGHT_SENSOR_OUTSIDE A2
#define LIGHT_SENSOR_INSIDE A3

#define CURRENT_SENSOR_PIN A6
#define VOLTAGE_SENSOR_PIN A7
#define TEMPERTURE_SENSOR_PIN 41

#define HWM_AUTO_BRIGHTNESS_PIN 2

#endif // !PREDEFINED_OFF


