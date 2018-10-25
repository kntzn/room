// StripController.h

#ifndef _ANALYZER_h
#define _ANALYZER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Predefined.h"

#include <FHT.h>

class Analyzer
    {
    public:
        // ----------------------------------------
        // Enums
        enum soundSource
            {
            microphone,
            headphones
            };
    private:
        // ----------------------------------------
        // Parameters

        // Source of signal
        byte source = microphone;
        int low_pass_filter = 0;

        // ----------------------------------------
        // Internal values
        
        // Last recorded amplitude 
        float volume = 0.f;
        // Filtered volume
        float volume_filt = 0.f;
        // Average volume
        float averVolume = 0.f;
        
        // ----------------------------------------
        // Communication
        // VU meter output
        float VU_out = 0.f;


        // normalized freqencies volume in range [0; 1]
        float freq_filt [SPECTRUM_SIZE] = {};
        // normalized types of freqencies volume in range [0; 1]
        float freq_peaks_filt [3] = {};

    public:
        // ----------------------------------------
        // Constructor
        // Initializes led strips
        Analyzer ();
        
        // ----------------------------------------
        // Update function
        // Listens and filter music's volume
        void update ();

        // -----------------UTIL-------------------
        // ----------------------------------------
        // Measures volume
        float measureVol ();
        int VUmeter ();

        void analyzer ();
        void analyzeAudio ();

        // ----------SETTERS and GETTERS-----------

        // --------------COMMUNICATION-------------
    };

#endif


// freq
/*
// 3-5 режим - цветомузыка
if (this_mode == 2 || this_mode == 3 || this_mode == 4 || this_mode == 7 || this_mode == 8) {
analyzeAudio();
colorMusic[0] = 0;
colorMusic[1] = 0;
colorMusic[2] = 0;
for (int i = 0 ; i < 32 ; i++) {
if (fht_log_out[i] < SPEKTR_LOW_PASS) fht_log_out[i] = 0;
}
// низкие частоты, выборка со 2 по 5 тон (0 и 1 зашумленные!)
for (byte i = 2; i < 6; i++) {
if (fht_log_out[i] > colorMusic[0]) colorMusic[0] = fht_log_out[i];
}
// средние частоты, выборка с 6 по 10 тон
for (byte i = 6; i < 11; i++) {
if (fht_log_out[i] > colorMusic[1]) colorMusic[1] = fht_log_out[i];
}
// высокие частоты, выборка с 11 по 31 тон
for (byte i = 11; i < 32; i++) {
if (fht_log_out[i] > colorMusic[2]) colorMusic[2] = fht_log_out[i];
}
freq_max = 0;
for (byte i = 0; i < 30; i++) {
if (fht_log_out[i + 2] > freq_max) freq_max = fht_log_out[i + 2];
if (freq_max < 5) freq_max = 5;

if (freq_f[i] < fht_log_out[i + 2]) freq_f[i] = fht_log_out[i + 2];
if (freq_f[i] > 0) freq_f[i] -= LIGHT_SMOOTH;
else freq_f[i] = 0;
}
freq_max_f = freq_max * averK + freq_max_f * (1 - averK);
for (byte i = 0; i < 3; i++) {
colorMusic_aver[i] = colorMusic[i] * averK + colorMusic_aver[i] * (1 - averK);  // общая фильтрация
colorMusic_f[i] = colorMusic[i] * SMOOTH_FREQ + colorMusic_f[i] * (1 - SMOOTH_FREQ);      // локальная
if (colorMusic_f[i] > ((float)colorMusic_aver[i] * MAX_COEF_FREQ)) {
thisBright[i] = 255;
colorMusicFlash[i] = true;
running_flag[i] = true;
} else colorMusicFlash[i] = false;
if (thisBright[i] >= 0) thisBright[i] -= SMOOTH_STEP;
if (thisBright[i] < EMPTY_BRIGHT) {
thisBright[i] = EMPTY_BRIGHT;
running_flag[i] = false;
}
}
animation();
}

*/
