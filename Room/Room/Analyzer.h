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

        // -----------------UTIL-------------------
        // ----------------------------------------
        // Measures volume
        float measureVol ();
        // Powers and filters the volume
        int VUmeter ();

        // Divides signal into array of frequencies
        void analyzeAudio ();
        // Divides array into 3 types of frequencies and filters it
        void analyzer (float dt);

        float getMaxFreq ();

    public:
        // ----------------------------------------
        // Constructor
        // Initializes led strips
        Analyzer ();
        
        // ----------------------------------------
        // Update function
        // Listens and filter music's volume
        void update (float dt);

        // ----------SETTERS and GETTERS-----------
        int getVUout ()
            {
            return VU_out;
            }
        float* getFreqValues ();

        // --------------COMMUNICATION-------------
    };

#endif
