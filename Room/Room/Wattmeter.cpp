// 
// 
// 

#include "Wattmeter.h"

long PowerSupply::aver_analog (uint8_t pin, size_t times)
    {
    long value = 0;
    for (int i = 0; i < times; i++)
        value += analogRead (pin);
    value /= times;
    return value;
    }

long PowerSupply::readVcc ()
    {
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV (REFS0) | _BV (MUX4) | _BV (MUX3) | _BV (MUX2) | _BV (MUX1);
    #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV (MUX5) | _BV (MUX0);
    #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV (MUX3) | _BV (MUX2);
    #else
    ADMUX = _BV (REFS0) | _BV (MUX3) | _BV (MUX2) | _BV (MUX1);
    #endif
    delay (2); // Wait for Vref to settle
    ADCSRA |= _BV (ADSC); // Start conversion
    while (bit_is_set (ADCSRA, ADSC)); // measuring
    uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both
    long result = (high << 8) | low;

    result = VCC_REF * 1023 * 1000 / result;
    return result;
    }

float PowerSupply::voltage_prec (byte pin)
    {
    // Measuring battery voltage
    return readVcc ()*aver_analog (pin) / 1024 / 1000.0 + 0.05f;
    }

PowerSupply::PowerSupply (byte pinI, byte pinV)
    {    
    pinCurrent = pinI;
    pinVoltage = pinV;

    voltage = current = power = 0.f;
    }

void PowerSupply::update ()
    {
    float current_raw_input = voltage_prec (CURRENT_SENSOR_PIN);

    voltage = voltage_prec (VOLTAGE_SENSOR_PIN);
    current = U_TO_I_K * current_raw_input + U_TO_I_B;
    power = current * voltage;
    }


float PowerSupply::getVoltage ()
    {
    return voltage;
    }
float PowerSupply::getCurrent ()
    {
    return current;
    }
float PowerSupply::getPower ()
    {
    return power;
    }
