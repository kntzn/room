#include "Analyzer.h"
#include <FHT.h>

Analyzer::Analyzer ()
    {
    // Increasing measurements frequency
    
    pinMode (MIC_INPUT, INPUT);
    pinMode (MIC_INPUT_FREQ, INPUT);
    pinMode (JACK_INPUT, INPUT);
    pinMode (JACK_INPUT_FREQ, INPUT);
    }

void Analyzer::update ()
    {
    // Measuring the volume
    VU_out = VUmeter ();

    // Dividing the sound into freqencies
    analyzer ();
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
        if (volume < measured) volume = measured;
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
    volume_filt = volume*(1 - SMOOTH_VU) + volume_filt * SMOOTH_VU;

    // Average volume smoothing filter
    averVolume = volume_filt * (1 - AVER_VOLUME_SMOOTH) + averVolume * AVER_VOLUME_SMOOTH;

    // Generating output of VU meter
    int output = map (volume_filt, 0, RATIO_MAX_TO_AVG*averVolume, 0, ANALOG_VU_MAX);
    return constrain (output, 0, ANALOG_VU_MAX);
    }

void Analyzer::analyzer ()
    {
    analyzeAudio ();
    }

void Analyzer::calibrateLowPass ()
    {
    analyzeAudio ();

    uint8_t max = 0;
    for (int i = 3; i < SPECTRUM_SIZE; i++)
        if (fht_log_out [i] > max)
            max = fht_log_out [i];

    low_pass_filter_FREQ = max;

    //Serial.println (low_pass_filter_FREQ);
    }

uint8_t * Analyzer::getFreqValues ()
    {
    return fht_log_out;
    }

void Analyzer::analyzeAudio () 
    {
    byte source_pin = JACK_INPUT_FREQ;
    //if (source == soundSource::microphone)
      //  source_pin = MIC_INPUT_FREQ;

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

    for (int i = 0; i < SPECTRUM_SIZE; i++)
        if (fht_log_out [i] < 24)
            fht_log_out [i] = 0;
    }