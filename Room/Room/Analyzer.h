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
        enum freq_peak_type
            {
            low_f,
            mid_f,
            high_f
            };
    private:
        // ----------------------------------------
        // Parameters

        // level of noise if VU mode
        int low_pass_filter_VU = 0;
        
        // ----------------------------------------
        // Internal values
        
        // Last recorded amplitude 
        float volume_low_pass = 0.f;
        // Last recorded amplitude 
        float volume = 0.f;
        // Filtered volume
        float volume_filt = 0.f;
        // Average volume
        float averVolume = 0.f;

        // Raw frequencies array
        float freq [SPECTRUM_SIZE] = {};
        // Mapped and filtered array of frequencies
        float freq_map [SPECTRUM_SIZE] = {};

        // Raw frequencies types array
        float freq_peaks [SPECTRUM_SIZE] = {};
        // Mapped and filtered array of frequencies types
        float freq3_map [SPECTRUM_SIZE] = {};
        
        // The loudest frequency
        float freq_max_filt = 0.f;


        // Last recorded signal time
        long lastSignal = millis ();

        // ----------------------------------------
        // Communication
        // VU meter output
        int VU_out = 0;

        

        // -----------------UTIL-------------------
        // ----------------------------------------
        // Measures volume
        float measureVol ();
        // Powers and filters the volume
        int VUmeter ();

        // Transforms the signal
        void analyzeAudio ();
        // Filters transformed signal
        void analyzer (float dt);
        // Gets the loudest frequency value
        float getMaxFreq ();
        // Fills the array of frequency values
        void getPeaks ();

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
        bool signalAvailable ();
        int getVUout ();
        float* getFreqValues ();
        float* getFreq3Values ();


        // --------------COMMUNICATION-------------
    };

#endif
