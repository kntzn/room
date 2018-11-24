#include "StripController.h"

// Constructor
StripController::StripController ()
    {
    LEDS.addLeds <WS2811, STRIP_DATA_MAIN, GRB> (leds_main, N_LEDS_MAIN);
    LEDS.addLeds <WS2811, STRIP_DATA_TABLE, GRB> (leds_table, N_LEDS_TABLE);

    LEDS.setBrightness (50);
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
        case fullWhite:
            {
            for (int i = 0; i < N_LEDS_MAIN; i++)
                leds_main [i] = CRGB::White;
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
        case night:
            {
            float dt = float (millis () - mode_activation_time);
            float full_time = float (NIGHT_FADE_TIME) * 60.f * 1000.f;
            float brightness = float (NIGHT_BRIGHTNESS_MAX)*
                               float (1.f -  (dt/full_time));
            
            if (brightness < 0)
                brightness = 0;

            for (int i = 0; i < N_LEDS_MAIN; i++)
                leds_main [i] = CHSV (NIGHT_COLOR, MAX_SATURATION, int (brightness));

            break;
            }
        case RVD:
            {
            float dt = float (millis () - mode_activation_time);
            float full_time = float (RVD_RISE_TIME) * 60.f * 1000.f;
            float brightness = float (MAX_BRIGHTNESS)*
                               float ((dt / full_time));
            
            if (brightness > 255)
                {
                brightness = 255;

                // Just for stabillity
                for (int i = 0; i < N_LEDS_SEC_0; i++)
                    leds_main [i] = CRGB::Blue;
                for (int i = N_LEDS_SEC_0; i < N_LEDS_SEC_0 + N_LEDS_SEC_1; i++)
                    leds_main [i] = CRGB::Red;
                for (int i = N_LEDS_SEC_0 + N_LEDS_SEC_1; i < N_LEDS_MAIN; i++)
                    leds_main [i] = CRGB (40, 0, 255);
                }
            else
                {
                // Section 0
                for (int i = 0; i < N_LEDS_SEC_0; i += N_LEDS_SUBSEC)
                    {
                    CRGB subsec_color = CRGB (0, 0, brightness);

                    int rnd_val = rand () % RND_AMPL;
                    if (rnd_val >= RND_AMPL - RND_AMPL * pow (2, -brightness / 20))
                        subsec_color = CRGB (0, 0, 0);

                    for (int j = 0; j < N_LEDS_SUBSEC; j++)
                        {
                        if (i + j >= N_LEDS_MAIN)
                            break;
                        leds_main [i + j] = subsec_color;
                        }
                    }
                // Section 1
                for (int i = N_LEDS_SEC_0; i < N_LEDS_SEC_0 + N_LEDS_SEC_1; i += N_LEDS_SUBSEC)
                    {
                    CRGB subsec_color = CRGB (brightness, 0, 0);

                    int rnd_val = rand () % RND_AMPL;
                    if (rnd_val >= RND_AMPL - RND_AMPL * pow (2, -brightness / 20))
                        subsec_color = CRGB (0, 0, 0);

                    for (int j = 0; j < N_LEDS_SUBSEC; j++)
                        {
                        if (i + j >= N_LEDS_SEC_0 + N_LEDS_SEC_1)
                            break;
                        leds_main [i + j] = subsec_color;
                        }
                    }
                // Section 2
                for (int i = N_LEDS_SEC_0 + N_LEDS_SEC_1; i < N_LEDS_MAIN; i += N_LEDS_SUBSEC)
                    {
                    CRGB subsec_color = CRGB (brightness / 10, 0, brightness);

                    int rnd_val = rand () % RND_AMPL;
                    if (rnd_val >= RND_AMPL - RND_AMPL * pow (2, -brightness / 20))
                        subsec_color = CRGB (0, 0, 0);

                    for (int j = 0; j < N_LEDS_SUBSEC; j++)
                        {
                        if (i + j >= N_LEDS_MAIN)
                            break;
                        leds_main [i + j] = subsec_color;
                        }
                    }
                }
            
            break;
            }
        case rise:
            {
            float dt = float (millis () - mode_activation_time);
            float full_time = float (RISE_MODE_RISE_TIME) * 60.f * 1000.f;
            float brightness = float (RISE_BRIGHTNESS_MAX) *
                float (dt / full_time);

            // RGB color
            CRGB riseColor = RISE_COLOR;
            // Black to RISE_COLOR stage
            if (brightness <= RISE_BRIGHTNESS_MAX)
                {
                riseColor = CRGB (riseColor.r * float (brightness / float (RISE_BRIGHTNESS_MAX)),
                                  riseColor.g * float (brightness / float (RISE_BRIGHTNESS_MAX)),
                                  riseColor.b * float (brightness / float (RISE_BRIGHTNESS_MAX)));
                }
            // RISE_COLOR to white stage
            else if (brightness <= 2 * RISE_BRIGHTNESS_MAX)
                {
                CRGB deltaColor = CRGB (CRGB::White) - riseColor;

                deltaColor = CRGB (deltaColor.r * float (brightness - RISE_BRIGHTNESS_MAX) /
                                   float (RISE_BRIGHTNESS_MAX),
                                   deltaColor.g * float (brightness - RISE_BRIGHTNESS_MAX) /
                                   float (RISE_BRIGHTNESS_MAX),
                                   deltaColor.b * float (brightness - RISE_BRIGHTNESS_MAX) /
                                   float (RISE_BRIGHTNESS_MAX));
                riseColor += deltaColor;
                }
            else
                riseColor = CRGB::White;

            // Fills leds' colors array
            for (int i = 0; i < N_LEDS_MAIN; i++)
                leds_main [i] = riseColor;
            
            break;
            }
        case oldschoolRND:
        case oldschool6:
            {
            // Fills new color
            if (millis () - mode_activation_time >= (OLDSCHOOL_SWITCH_TIME * 60 * 1000) / (4 * abs (rainbow_speed)))
                setMode (mode);
            // Fades new color from black
            else if (millis () - mode_activation_time < FADE_SWITCH_TIME * 1000)
                {
                float percent = float (millis () - mode_activation_time) / 
                                float (FADE_SWITCH_TIME * 1000);

                CRGB sections [3] = {};
                for (int i = 0; i < 3; i++)
                    sections [i] = CRGB (currColor3sections [i].r*percent,
                                         currColor3sections [i].g*percent, 
                                         currColor3sections [i].b*percent);
                
                fillSections (sections);
                }
            // Fills current colors
            else if (millis () - mode_activation_time < 
                     float (OLDSCHOOL_SWITCH_TIME * 60 * 1000 - FADE_SWITCH_TIME * 1000) / (4 * abs (rainbow_speed)))
                {
                fillSections (currColor3sections);
                
                }
            else if (millis () - mode_activation_time < (OLDSCHOOL_SWITCH_TIME * 60 * 1000) / (4 * abs (rainbow_speed)))
                {
                float percent = 1.f - float (millis () - mode_activation_time - 
                                          (OLDSCHOOL_SWITCH_TIME * 60 * 1000 - 
                                          FADE_SWITCH_TIME * 1000)) /
                                          float (FADE_SWITCH_TIME * 1000);

                CRGB sections [3] = {};
                for (int i = 0; i < 3; i++)
                    sections [i] = CRGB (currColor3sections [i].r*percent,
                                         currColor3sections [i].g*percent,
                                         currColor3sections [i].b*percent);

                fillSections (sections);
                }

            break;
            }
        case RVD_RND:
            {
            float dt = float (millis () - mode_activation_time);
            float full_time = float (RVD_RISE_TIME) * 60.f * 1000.f;
            float brightness = float (MAX_BRIGHTNESS)*
                float ((dt / full_time));



            if (brightness > MAX_BRIGHTNESS)
                {
                brightness = MAX_BRIGHTNESS;

                // Just for stabillity
                for (int i = 0; i < N_LEDS_SEC_0; i++)
                    leds_main [i] = currColor3sectionsRVD [0];
                for (int i = N_LEDS_SEC_0; i < N_LEDS_SEC_0 + N_LEDS_SEC_1; i++)
                    leds_main [i] = currColor3sectionsRVD [1];
                for (int i = N_LEDS_SEC_0 + N_LEDS_SEC_1; i < N_LEDS_MAIN; i++)
                    leds_main [i] = currColor3sectionsRVD [2];
                }
            else
                {
                // Section 0
                for (int i = 0; i < N_LEDS_SEC_0; i += N_LEDS_SUBSEC)
                    {
                    CRGB thisColor = currColor3sectionsRVD [0];
                    CRGB subsec_color = CRGB (float (thisColor.r)*brightness / 255.f,
                                              float (thisColor.g)*brightness / 255.f,
                                              float (thisColor.b)*brightness / 255.f);

                    int rnd_val = rand () % RND_AMPL;
                    if (rnd_val >= RND_AMPL - RND_AMPL * pow (2, -brightness / 20))
                        subsec_color = CRGB (0, 0, 0);

                    for (int j = 0; j < N_LEDS_SUBSEC; j++)
                        {
                        if (i + j >= N_LEDS_MAIN)
                            break;
                        leds_main [i + j] = subsec_color;
                        }
                    }
                // Section 1
                for (int i = N_LEDS_SEC_0; i < N_LEDS_SEC_0 + N_LEDS_SEC_1; i += N_LEDS_SUBSEC)
                    {
                    CRGB thisColor = currColor3sectionsRVD [1];
                    CRGB subsec_color = CRGB (float (thisColor.r)*brightness / 255.f,
                                              float (thisColor.g)*brightness / 255.f,
                                              float (thisColor.b)*brightness / 255.f);

                    int rnd_val = rand () % RND_AMPL;
                    if (rnd_val >= RND_AMPL - RND_AMPL * pow (2, -brightness / 20))
                        subsec_color = CRGB (0, 0, 0);

                    for (int j = 0; j < N_LEDS_SUBSEC; j++)
                        {
                        if (i + j >= N_LEDS_SEC_0 + N_LEDS_SEC_1)
                            break;
                        leds_main [i + j] = subsec_color;
                        }
                    }
                // Section 2
                for (int i = N_LEDS_SEC_0 + N_LEDS_SEC_1; i < N_LEDS_MAIN; i += N_LEDS_SUBSEC)
                    {
                    CRGB thisColor = currColor3sectionsRVD [2];
                    CRGB subsec_color = CRGB (float (thisColor.r)*brightness / 255.f,
                                              float (thisColor.g)*brightness / 255.f,
                                              float (thisColor.b)*brightness / 255.f);

                    int rnd_val = rand () % RND_AMPL;
                    if (rnd_val >= RND_AMPL - RND_AMPL * pow (2, -brightness / 20))
                        subsec_color = CRGB (0, 0, 0);

                    for (int j = 0; j < N_LEDS_SUBSEC; j++)
                        {
                        if (i + j >= N_LEDS_MAIN)
                            break;
                        leds_main [i + j] = subsec_color;
                        }
                    }
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
                                           map (frequency_3 [highs], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else if (i < int (2*segment_size))
                    leds_table [i] = CHSV (HSV_MID_FREQ_COLOR, MAX_SATURATION,
                                           map (frequency_3 [mids], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else if (i < int (3 * segment_size))
                    leds_table [i] = CHSV (HSV_LOW_FREQ_COLOR, MAX_SATURATION,
                                           map (frequency_3 [lows], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else if (i < int (4*segment_size))
                    leds_table [i] = CHSV (HSV_MID_FREQ_COLOR, MAX_SATURATION,
                                           map (frequency_3 [mids], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else
                    leds_table [i] = CHSV (HSV_HIGH_FREQ_COLOR, MAX_SATURATION,
                                           map (frequency_3 [highs], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
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
                                           map (frequency_3 [highs], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else if (i < int (2 * segment_size))
                    leds_table [i] = CHSV (HSV_MID_FREQ_COLOR, MAX_SATURATION,
                                           map (frequency_3 [mids], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                else
                    leds_table [i] = CHSV (HSV_LOW_FREQ_COLOR, MAX_SATURATION,
                                           map (frequency_3 [lows], 0, FREQ_MAX, FREQ_BRIGHTNESS_MIN, FREQ_BRIGHTNESS_MAX));
                }

            break;
            }
        case FREQ_FULL:
            {
            Serial.println (frequency_full [6]);

            for (int i = 0; i < N_LEDS_TABLE / 2; i++)
                {
                int curr_pos_freq_array = int (i * SPECTRUM_SIZE / (N_LEDS_TABLE / 2));

                float brightness = 0;

                
                brightness = frequency_full [curr_pos_freq_array];
                
                int HUE = freq_full_rainbow_offset + freq_full_rainbow_freq * i / (N_LEDS_TABLE / 2);

                leds_table [N_LEDS_TABLE / 2 - i - 1] = CHSV (HUE, 255, brightness);
                
                leds_table [N_LEDS_TABLE - i - 1] = leds_table [i];
                }

            break;
            }

        default:
            break;
        }

    display ();
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
void StripController::fillSections (CRGB sections [3])
    {
    // Paints the sections
    for (int i = 0; i < N_LEDS_SEC_0; i++)
        leds_main [i] = sections [0];
    for (int i = N_LEDS_SEC_0; i < N_LEDS_SEC_0 + N_LEDS_SEC_1; i++)
        leds_main [i] = sections [1];
    for (int i = N_LEDS_SEC_0 + N_LEDS_SEC_1; i < N_LEDS_MAIN; i++)
        leds_main [i] = sections [2];
    }
CRGB StripController::Wheel (byte WheelPos)
    {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
        {
        return CRGB (255 - WheelPos * 3, 0, WheelPos * 3);
        }
    else if (WheelPos < 170)
        {
        WheelPos -= 85;
        return CRGB (0, WheelPos * 3, 255 - WheelPos * 3);
        }
    else
        {
        WheelPos -= 170;
        return CRGB (WheelPos * 3, 255 - WheelPos * 3, 0);
        }
    }

// Setters
void StripController::setMode (byte newMode)
    {
    fadeSwitchColor = CRGB::Red;
    switchedColorFlag = false;
    mode = newMode;

    if (newMode == oldschool6)
        {
        for (int i = 0; i < 3; i++)
            currColor3sections [i] = Wheel ((255 / 6) * (rand () % 6));
        }
    if (newMode == oldschoolRND)
        {
        for (int i = 0; i < 3; i++)
            currColor3sections [i] = Wheel (rand () % 256);
        }
    if (newMode == RVD_RND)
        {
        // Generates neon different colors

        // section 0
        // RED / BLUE
        bool id_rnd_rvd_color_0 = rand () % 2;
        currColor3sectionsRVD [0] = Color3sectionsRVD [id_rnd_rvd_color_0];
        
        // section 1
        // BLUE / RED
        currColor3sectionsRVD [1] = Color3sectionsRVD [!id_rnd_rvd_color_0];
        
        // 3rd section
        // ANOTHER_COLOR
        int id_rnd_rvd_color_2 = 2 + rand () % (N_COLORS_RVD-2);
        currColor3sectionsRVD [2] = Color3sectionsRVD [id_rnd_rvd_color_2];
        }
        

    mode_activation_time = millis ();
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
        frequency_3 [i] = newFreqVal [i];
    }
void StripController::setFreqValues (float newFreqVal [SPECTRUM_SIZE])
    {
    for (int i = 0; i < SPECTRUM_SIZE; i++)
        frequency_full [i] = newFreqVal [i];
    }
