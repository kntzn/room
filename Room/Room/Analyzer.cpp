#include "Analyzer.h"

Analyzer::Analyzer ()
    {
    // Increasing measurements frequency
    sbi (ADCSRA, ADPS2);
    cbi (ADCSRA, ADPS1);
    sbi (ADCSRA, ADPS0);
    }

void Analyzer::update ()
    {
    }

void Analyzer::measureVol ()
    {
    }