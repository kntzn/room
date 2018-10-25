#include "Analyzer.h"

#include <FHT.h>

Analyzer::Analyzer ()
    {
    // Increasing measurements frequency
    sbi (ADCSRA, ADPS2);
    cbi (ADCSRA, ADPS1);
    sbi (ADCSRA, ADPS0);

    pinMode (MIC_INPUT, INPUT);
    pinMode (MIC_INPUT_FREQ, INPUT);
    pinMode (JACK_INPUT, INPUT);
    pinMode (JACK_INPUT_FREQ, INPUT);
    }

void Analyzer::update ()
    {
    // Measuring the volume
    VU_out = VUmeter ();

    // Dividing the sound into frequencies
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
    for (int i = 0; i < SPECTRUM_SIZE; i++)
        {
        // Filtering
        if (fht_log_out [i] < low_pass_filter_FREQ)
            fht_log_out [i] = 0;

        // Fading or burning
        if (freq [i] < fht_log_out [i])
            freq [i] = fht_log_out [i];
        else freq [i] *= FREQ_MODE_FADE_C;
        
        }

    for (int i = 0; i < 3; i++)
        freq_peaks [i] = 0;

    // Determinating the loudest frequency volume
    float freq_max = 0;
    for (byte i = 2; i < 32; i++)
        {
        if (fht_log_out [i] > freq_max) freq_max = fht_log_out [i];
        }
    if (freq_max < low_pass_filter_FREQ)
        freq_max = low_pass_filter_FREQ;

    // Filtering max freq volume
    freq_max_filt = freq_max*(1.f-AVER_FREQ_VOLUME_SMOOTH) + freq_max_filt*AVER_FREQ_VOLUME_SMOOTH;

    // Filling peaks array
    for (int i = 0; i < N_LOW; i++)
        {
        if (freq [i] > freq_peaks [low_f])
            freq_peaks [low_f] = freq [i];
        }
    for (int i = N_LOW; i < N_LOW + N_MID; i++)
        {
        if (freq [i] > freq_peaks [mid_f])
            freq_peaks [mid_f] = freq [i];
        }
    for (int i = N_LOW + N_MID; i < SPECTRUM_SIZE; i++)
        {
        if (freq [i] > freq_peaks [high_f])
            freq_peaks [high_f] = freq [i];
        }

    // Filtering frequency peaks list array
    for (int i = 0; i < 3; i++)
        {
        freq_peaks_filt [i] = map (freq_peaks [i], 0, freq_max_filt, 0, FREQ_MAX);
        freq_peaks_filt [i] = constrain (freq_peaks_filt [i], 0, FREQ_MAX);
        }
    // Filtering full frequency list array
    for (int i = 0; i < SPECTRUM_SIZE; i++)
        {
        freq_filt [i] = map (freq [i], 0, freq_max_filt, 0, FREQ_MAX);
        freq_filt [i] = constrain (freq_filt [i], 0, FREQ_MAX);
        }

    }

void Analyzer::analyzeAudio ()
    {
    byte source_pin = JACK_INPUT_FREQ;
    if (source == soundSource::microphone)
        source_pin = MIC_INPUT_FREQ;

    for (int i = 0; i < FHT_N; i++)
        {
        int sample = analogRead (source_pin);
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


void Analyzer::calibrateLowPass ()
    {
    byte source_pin = JACK_INPUT;
    if (source == soundSource::microphone)
        source_pin = MIC_INPUT;

    // ADC init
    delay (10);

    // VU mode
    int noise = 0;
    for (byte i = 0; i < 200; i++)
        {
        int measured = analogRead (source_pin);
        if (measured > noise)
            noise = measured;
        delay (4);
        }
    low_pass_filter_VU = noise + 10;

    // FREQ mode
    noise = 0;
    for (byte i = 0; i < 100; i++)
        {
        analyzeAudio ();
        for (byte j = 2; j < 32; j++)
            {
            int measured = fht_log_out [j];
            if (measured > noise)
                noise = measured;
            }
        delay (4);
        }

    low_pass_filter_FREQ = noise + 3;
    }

float * Analyzer::getFreqPeaksArray ()
    {
    return freq_peaks_filt;
    }

float * Analyzer::getFreqArray ()
    {
    return freq_filt;
    }

