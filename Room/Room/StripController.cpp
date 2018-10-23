#include "StripController.h"

// Constructor
StripController::StripController ()
    {
    LEDS.addLeds <WS2811, STRIP_DATA_MAIN, GRB> (leds_main, N_LEDS_MAIN);
    LEDS.addLeds <WS2811, STRIP_DATA_TABLE, GRB> (leds_table, N_LEDS_TABLE);
    }

// Visuals
void StripController::update (float dt)
    {
    // updating offsets
    palette_offset += palette_speed*dt;
    rainbow_offset += rainbow_speed*dt;

    switch (mode)
        {
        case off:
            {
            for (int i = 0; i < N_LEDS_MAIN; i++)
                leds_main [i] = CRGB::Black;
            break;
            }
        case mono:
            {
            for (int i = 0; i < N_LEDS_MAIN; i++)
                leds_main [i] = currColor;
            break;
            }
        case fade:
            {
            float brightness = (1 + sinf (double ((2 * PI)*rainbow_offset))) / 2.f;

            for (int i = 0; i < N_LEDS_MAIN; i++)
                {
                leds_main [i].r = currColor.r * brightness;
                leds_main [i].g = currColor.g * brightness;
                leds_main [i].b = currColor.b * brightness;
                }

            break;
            }
        case fade_switch:
            {
            float brightness = (1 + sinf (double ((2 * PI)*rainbow_offset))) / 2.f;

            for (int i = 0; i < N_LEDS_MAIN; i++)
                {
                leds_main [i].r = fadeSwitchColor.r * brightness;
                leds_main [i].g = fadeSwitchColor.g * brightness;
                leds_main [i].b = fadeSwitchColor.b * brightness;
                }

            // Generates new color at low brightness
            if (brightness < float (SWITCH_BRIGHTNESS) / float (MAX_BRIGHTNESS))
                {
                brightness = 0;
                if (!switchedColorFlag)
                    {
                    switchedColorFlag = true;

                    if (fadeSwitchColor == CRGB (CRGB::Red))
                        fadeSwitchColor = CRGB::Yellow;
                    else if (fadeSwitchColor == CRGB (CRGB::Yellow))
                        fadeSwitchColor = CRGB::Green;
                    else if (fadeSwitchColor == CRGB (CRGB::Green))
                        fadeSwitchColor = CRGB::LightBlue;
                    else if (fadeSwitchColor == CRGB (CRGB::LightBlue))
                        fadeSwitchColor = CRGB::Blue;
                    else if (fadeSwitchColor == CRGB (CRGB::Blue))
                        fadeSwitchColor = CRGB::Violet;
                    else
                        fadeSwitchColor = CRGB::Red;
                    }
                }
            else if (switchedColorFlag)
                switchedColorFlag = false;
            break;
            }
        case fade_switch_random:
            {
            float brightness = (1 + sinf (double ((2 * PI)*rainbow_offset))) / 2.f;

            for (int i = 0; i < N_LEDS_MAIN; i++)
                {
                leds_main [i].r = fadeSwitchColor.r * brightness;
                leds_main [i].g = fadeSwitchColor.g * brightness;
                leds_main [i].b = fadeSwitchColor.b * brightness;
                }

            // Generates new color at low brightness
            if (brightness < float (SWITCH_BRIGHTNESS)/float (MAX_BRIGHTNESS))
                {
                brightness = 0;
                if (!switchedColorFlag)
                    {
                    switchedColorFlag = true;

                    fadeSwitchColor = CRGB (CHSV (rand ()%360, MAX_SATURATION, MAX_BRIGHTNESS));
                    }
                }
            else if (switchedColorFlag)
                switchedColorFlag = false;

            

            break;

            }
        case rainbow_Sine:
            {
            CRGB color;
            
            for (int i = 0; i < N_LEDS_MAIN; i++)
                {
                color.r = 128 + 127 * sinf (double (rainbow_freq * i * (2*PI)/N_LEDS_MAIN + (2*PI)*rainbow_offset + 2*PI/3));
                color.g = 128 + 127 * sinf (double (rainbow_freq * i * (2*PI)/N_LEDS_MAIN + (2*PI)*rainbow_offset + 4*PI/3));
                color.b = 128 + 127 * sinf (double (rainbow_freq * i * (2*PI)/N_LEDS_MAIN + (2*PI)*rainbow_offset));
                leds_main [i] = color;
                }

            break;
            }
        case rainbow_HSV:
            {
            for (int i = 0; i < N_LEDS_MAIN; i++)
                leds_main [i] = CHSV (i*(360/N_LEDS_MAIN)*rainbow_freq + rainbow_offset*360, MAX_SATURATION, MAX_BRIGHTNESS);
                
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
        case FREQ_FULL:
            {
            for (int i = 0; i < N_LEDS_TABLE / 2; i++)
                {
                float brightness = freqency_full [int (i * SPECTRUM_SIZE / (N_LEDS_TABLE/2))];
                leds_table [i] = CHSV (freq_full_rainbow_offset + freq_full_rainbow_freq * i / (N_LEDS_TABLE / 2), 255, brightness);
                
                leds_table [N_LEDS_TABLE - i - 1] = leds_table [i];
                }


            break;
            }

        default:
            break;
        }
    }
void StripController::display () 
    { 
    LEDS.show (); 
    }

// Util
void StripController::sync_strips ()
    {
    for (int i = 0; i < N_LEDS_TABLE; i++)
        leds_table [N_LEDS_TABLE - 1 - i] = leds_main [i];
    }

// Setters
void StripController::setMode (byte newMode)
    {
    fadeSwitchColor = CRGB::Red;
    switchedColorFlag = false;
    mode = newMode;
    }
void StripController::setTableMode (byte newMode)
    {
    table_mode = newMode;
    }
void StripController::setColor (CRGB newColor)
    {
    currColor = newColor;
    }
void StripController::setRainbowFrequency (float newFreq)
    {
    rainbow_freq = newFreq;
    }
void StripController::setRainbowSpeed (float newSpeed)
    {
    rainbow_speed = newSpeed;
    }
void StripController::setPaletteSpeed (float newSpeed)
    {
    palette_speed = newSpeed;
    }
void StripController::setFreqModeRainFreq (float newFreq)
    {
    freq_full_rainbow_freq = newFreq;
    }
void StripController::setFreqModeRainOffset (float newOffset)
    {
    freq_full_rainbow_offset = newOffset;
    }

// Communication
void StripController::setVU_val (int newVU_val)
    {
    VU_val = map (newVU_val, 0, VU_OUT_MAX, 0, ANALOG_VU_MAX);
    }
void StripController::setFreq3values (float newFreqVal [3])
    {
    for (int i = 0; i < 3; i++)
        freqency_3 [i] = newFreqVal [i];
    }
void StripController::setFreqValues (float newFreqVal [SPECTRUM_SIZE])
    {
    for (int i = 0; i < SPECTRUM_SIZE; i++)
        freqency_full [i] = newFreqVal [i];
    }
