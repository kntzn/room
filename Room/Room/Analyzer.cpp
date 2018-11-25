#include "Analyzer.h"
#include <FHT.h>

Analyzer::Analyzer ()
    {
    // Increasing measurements frequency
    sbi (ADCSRA, ADPS2);
    cbi (ADCSRA, ADPS1);
    sbi (ADCSRA, ADPS0);
    }

void Analyzer::update (float dt)
    {
    // Measuring the volume
    VU_out = VUmeter ();

    // Dividing the sound into freqencies
    analyzer (dt);
    }

float Analyzer::measureVol ()
    {
    float volume = 0.f;

    byte source_pin = JACK_INPUT;
    if (source == soundSource::microphone)
        source_pin = MIC_INPUT;

    for (byte i = 0; i < 100; i++)
        {
        int measured = analogRead (source_pin);
        if (volume < measured) 
            volume = measured;
        }

    return volume;
    }

int Analyzer::VUmeter ()
    {
    // Powering and mapping back the volume
    float volume = map (measureVol (), low_pass_filter_VU, 1023, 0, ANALOG_VU_MAX);
    volume = pow (volume, EXP);
    volume = map (volume, 0, VU_OUT_MAX, 0, ANALOG_VU_MAX);
    volume = constrain (volume, 0, ANALOG_VU_MAX);

    // Smoothing filter of volume
    volume_filt = volume * (1 - SMOOTH_VU) + volume_filt * SMOOTH_VU;

    // Average volume smoothing filter
    averVolume = volume_filt * (1 - AVER_VOLUME_SMOOTH) + averVolume * AVER_VOLUME_SMOOTH;

    // Generating output of VU meter
    int output = map (volume_filt, 0, RATIO_MAX_TO_AVG*averVolume, 0, ANALOG_VU_MAX);
    return constrain (output, 0, ANALOG_VU_MAX);
    }

void Analyzer::analyzer (float dt)
    {
    // Transforms the signal
    analyzeAudio ();
    
    // Filters the output of FHT
    for (int i = 0; i < SPECTRUM_SIZE; i++)
        if (fht_log_out [i] < LOW_PASS_FREQ || i < LOWEST)
            fht_log_out [i] = 0;

    // Makes the output smoother
    for (int i = 0; i < SPECTRUM_SIZE; i++)
        {
        if (freq [i] < fht_log_out [i])
            freq [i] += (fht_log_out [i] - freq [i])*0.7;
        else if (freq [i] > FREQ_MODE_FADE_C*dt)
            freq [i] -= FREQ_MODE_FADE_C*dt;
        else
            freq [i] = 0;
        }

    // Gets frequencies types
    getPeaks ();
    } 

float Analyzer::getMaxFreq ()
    {
    float max = 0;
    for (int i = LOWEST; i < SPECTRUM_SIZE; i++)
        if (fht_log_out [i] > uint8_t (max))
            max = float (fht_log_out [i]);
        
    return max;
    }

void Analyzer::getPeaks ()
    {
    // Gets maximum of each frequency type
    for (int i = 0; i < 3; i++)
        freq_peaks [i] = 0;

    // Lows
    for (int i = LOWEST; i < LOWEST + N_LOW; i++)
        if (freq_peaks [0] < freq [i])
            freq_peaks [0] = freq [i];
    // Mids
    for (int i = LOWEST + N_LOW; i < LOWEST + N_LOW + N_MID; i++)
        if (freq_peaks [1] < freq [i])
            freq_peaks [1] = freq [i];
    // Highs
    for (int i = LOWEST + N_LOW + N_MID; i < SPECTRUM_SIZE; i++)
        if (freq_peaks [2] < freq [i])
            freq_peaks [2] = freq [i];
    }

float* Analyzer::getFreqValues ()
    {
    // Maps the frequencies loudness to FREQ_MAX level
    float max = getMaxFreq ();
    if (max > 0)
        for (int i = 0; i < SPECTRUM_SIZE; i++)
            freq_map [i] = freq [i] * float (FREQ_MAX) / max * 
                           pow (float (i + 1) / float (SPECTRUM_SIZE), 1.01);
    else if (max == 0)
        for (int i = 0; i < SPECTRUM_SIZE; i++)
            freq_map [i] *= 0.9;

    return freq_map;
    }

float * Analyzer::getFreq3Values ()
    {
    // Maps the frequencies loudness to FREQ_MAX level
    float max = max (freq_peaks [0], max (freq_peaks [1], freq_peaks [2]));

    if (max > 0)
        for (int i = 0; i < 3; i++)
            freq3_map [i] = freq_peaks [i] * float (FREQ_MAX) / max;
    else if (max == 0)
        for (int i = 0; i < 3; i++)
            freq3_map [i] *= 0.9;

    return freq3_map;
    }

void Analyzer::analyzeAudio () 
    {
    byte source_pin = JACK_INPUT_FREQ;
    if (source == soundSource::microphone)
        source_pin = MIC_INPUT_FREQ;

    for (int i = 0; i < FHT_N; i++) 
        {
        int sample = analogRead (A2);
        fht_input [i] = sample;
        }
    // Windowing the data for better frequency response
    fht_window ();
    // Reordering the data before doing the FHT
    fht_reorder (); 
    // Starting the FHT
    fht_run ();     
    // Getting the output of the FHT
    fht_mag_log (); 

    }