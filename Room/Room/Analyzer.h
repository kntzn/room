// StripController.h

#ifndef _ANALYZER_h
#define _ANALYZER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Predefined.h"

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
        enum freq_peak_type
            {
            low_f,
            mid_f,
            high_f
            };
    private:
        // ----------------------------------------
        // Parameters

        // Source of signal
        byte source = headphones;
        // level of noise if VU mode
        int low_pass_filter_VU = 0;
        // level of noise in freq mode
        int low_pass_filter_FREQ = 0;

        // ----------------------------------------
        // Internal values
        
        // Last recorded amplitude 
        float volume = 0.f;
        // Filtered volume
        float volume_filt = 0.f;
        // Average volume
        float averVolume = 0.f;

        // Raw frequencies array
        float freq [SPECTRUM_SIZE] = {};
        // Raw frequencies types array
        float freq_peaks [SPECTRUM_SIZE] = {};

        // The loudest frequency
        float freq_max_filt = 0.f;

        // ----------------------------------------
        // Communication
        // VU meter output
        int VU_out = 0;


        // normalized frequencies volume in range [0; 1]
        float freq_filt [SPECTRUM_SIZE] = {};
        // normalized types of frequencies volume in range [0; 1]
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
        // Powers and filters the volume
        int VUmeter ();

        // Divides signal into array of frequencies
        void analyzeAudio ();
        // Divides array into 3 types of frequencies
        void analyzer ();
        
        // ----------SETTERS and GETTERS-----------
        void calibrateLowPass ();

        int getVUout ()
            {
            return VU_out;
            }

        float* getFreqPeaksArray ();
        float* getFreqArray ();

        // --------------COMMUNICATION-------------
    };

#endif


// freq
/*
// 3-5 режим - цветомузыка
if (this_mode == 2 || this_mode == 3 || this_mode == 4 || this_mode == 7 || this_mode == 8) {

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
