#include "Analyzer.h"

Analyzer::Analyzer ()
    {
    sbi (ADCSRA, ADPS2);
    cbi (ADCSRA, ADPS1);
    sbi (ADCSRA, ADPS0);

    }

Analyzer::~Analyzer ()
    {
    }
