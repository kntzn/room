#include "StripController.h"

StripController::StripController ()
    {
    LEDS.addLeds <WS2811, STRIP_DATA_MAIN, GRB> (leds_main, N_LEDS_MAIN);
    LEDS.addLeds <WS2811, STRIP_DATA_TABLE, GRB> (leds_table, N_LEDS_TABLE);
    }

void StripController::update (float dt)
    {
    // updating offsets
    palette_offset += palette_speed*dt;
    rainbow_offset += rainbow_speed*dt;

    switch (mode)
        {
        case off:
            for (int i = 0; i < N_LEDS_MAIN; i++)
                leds_main [i] = CRGB::Black;
            break;
        case mono:
            for (int i = 0; i < N_LEDS_MAIN; i++)
                leds_main [i] = CRGB::Red;
            break;
        case rainbow_SineWheel:
            {
            CRGB color;
            
            for (int i = 0; i < N_LEDS_MAIN; i++)
                {
                color.r = 128 + 127 * sinf (double (rainbow_freq*i*(2 * PI) / N_LEDS_MAIN + 2 * PI*rainbow_offset + 2*PI/3));
                color.g = 128 + 127 * sinf (double (rainbow_freq*i*(2 * PI) / N_LEDS_MAIN + 2 * PI*rainbow_offset + 4*PI/3));
                color.b = 128 + 127 * sinf (double (rainbow_freq*i*(2 * PI) / N_LEDS_MAIN + 2 * PI*rainbow_offset));
                leds_main [i] = color;
                }

            break;
            }

        default:
            break;
        }

    // VU_bright mode sets it's own brightness
    // Otherwise, brightness is maximal
    if (mode != VU_bright)
        LEDS.setBrightness (MAX_BRIGHTNESS);
    
    switch (table_mode)
        {
        case sync:
            {
            sync_strips ();
            break;
            }
        case VU_bright:
            {
            sync_strips ();
            LEDS.setBrightness (map (VU_val, 0, ANALOG_VU_MAX, 0, VU_BRIGHTNESS_MAX - VU_BRIGHTNESS_MIN) + VU_BRIGHTNESS_MIN);
            break;
            }
        case VU:
            {
            int len = map (VU_val, 0, ANALOG_VU_MAX, 0, N_LEDS_TABLE / 2);

            for (int i = 0; i <= N_LEDS_TABLE/2; i++)
                {
                if (i >= N_LEDS_TABLE / 2 - len)
                    leds_table [i] = CHSV (HSV_VU_START + HSV_VU_END * i / (N_LEDS_TABLE / 2), 255, MAX_BRIGHTNESS);
                else
                    leds_table [i] = CRGB::Black;

                leds_table [N_LEDS_TABLE - i - 1] = leds_table [i];
                }

            break;
            }
        case VU_rain:
            {
            int len = map (VU_val, 0, ANALOG_VU_MAX, 0, N_LEDS_TABLE / 2);

            for (int i = 0; i <= N_LEDS_TABLE / 2; i++)
                {
                if (i >= N_LEDS_TABLE / 2 - len)
                    leds_table [i] = CHSV (int (palette_offset + HSV_VU_END * i / (N_LEDS_TABLE / 2)), 255, MAX_BRIGHTNESS);
                else
                    leds_table [i] = CRGB::Black;

                leds_table [N_LEDS_TABLE - i - 1] = leds_table [i];
                }

            break;
            }
        case FREQ_5:
            {
            float segment_size = N_LEDS_TABLE / 5.f;

            for (int i = 0; i < N_LEDS_TABLE; i++)
                {
                if (i < int (segment_size))
                    leds_table [i] = CHSV (HSV_HIGH_FREQ_COLOR, MAX_SATURATION, 
                                           map (freqency_3 [highs], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else if (i < int (2*segment_size))
                    leds_table [i] = CHSV (HSV_MID_FREQ_COLOR, MAX_SATURATION,
                                           map (freqency_3 [mids], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else if (i < int (3 * segment_size))
                    leds_table [i] = CHSV (HSV_LOW_FREQ_COLOR, MAX_SATURATION,
                                           map (freqency_3 [lows], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else if (i < int (4*segment_size))
                    leds_table [i] = CHSV (HSV_MID_FREQ_COLOR, MAX_SATURATION,
                                           map (freqency_3 [mids], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else
                    leds_table [i] = CHSV (HSV_HIGH_FREQ_COLOR, MAX_SATURATION,
                                           map (freqency_3 [highs], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                }

            break;
            }
        case FREQ_3:
            {
            float segment_size = N_LEDS_TABLE / 3.f;

            for (int i = 0; i < N_LEDS_TABLE; i++)
                {
                if (i < int (segment_size))
                    leds_table [i] = CHSV (HSV_HIGH_FREQ_COLOR, MAX_SATURATION,
                                           map (freqency_3 [highs], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else if (i < int (2 * segment_size))
                    leds_table [i] = CHSV (HSV_MID_FREQ_COLOR, MAX_SATURATION,
                                           map (freqency_3 [mids], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else
                    leds_table [i] = CHSV (HSV_LOW_FREQ_COLOR, MAX_SATURATION,
                                           map (freqency_3 [lows], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                }

            break;
            }

        default:
            break;
        }
    }

void StripController::setVU_val (int newVU_val)
    {
    VU_val = map (newVU_val, 0, VU_OUT_MAX, 0, ANALOG_VU_MAX);
    }

void StripController::setFreq3values (float newFreqVal [3])
    {
    for (int i = 0; i < 3; i++)
        freqency_3 [i] = newFreqVal [i];
    }

void StripController::display () 
    { 
    LEDS.show (); 
    }

void StripController::sync_strips ()
    {
    for (int i = 0; i < N_LEDS_TABLE; i++)
        leds_table [N_LEDS_TABLE - 1 - i] = leds_main [i];
    }

void StripController::setMode (byte newMode)
    {
    mode = newMode;
    }

void StripController::setTableMode (byte newMode)
    {
    table_mode = newMode;
    }
