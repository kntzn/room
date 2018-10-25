#include "Analyzer.h"

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
    
    }

void Analyzer::measureVol ()
    {
    volume = 0.f;

    byte source_pin = JACK_INPUT;
    if (source == soundSource::microphone)
        source_pin = MIC_INPUT;

    for (byte i = 0; i < 100; i++)
        {
        int measured = analogRead (MIC_INPUT);
        if (volume < measured) volume = measured;
        }
    }