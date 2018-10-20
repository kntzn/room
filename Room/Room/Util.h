#pragma once

int averAnalog (byte pin, byte times = 100)
    {
    long result = 0;
    for (byte i = 0; i < times; i++)
        result += analogRead (pin);

    result /= times;

    return int (result);
    }
int maxAnalog (byte pin, byte times = 100)
    {
    int result = 0;
    for (byte i = 0; i < times; i++)
        {
        int measured = analogRead (pin);
        if (result < measured)
            result = measured;
        }

    return result;
    }

float filter_f (float raw, float previous, float k = 0)
    {
    return raw*(1-k) + previous*k;
    }
double filter_d (double raw, double previous, double k = 0)
    {
    return raw*(1 - k) + previous*k;
    }

