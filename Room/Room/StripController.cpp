#include "StripController.h"

// Constructor
StripController::StripController ()
    {
    pinMode (STRIP_DATA_MAIN, OUTPUT);
    pinMode (STRIP_DATA_TABLE, OUTPUT);

    LEDS.addLeds <WS2811, STRIP_DATA_MAIN, GRB> (leds_main, N_LEDS_MAIN);
    LEDS.addLeds <WS2811, STRIP_DATA_TABLE, GRB> (leds_table, N_LEDS_TABLE);
    }

// Visuals
void StripController::update (float dt)
    {
    // updating offsets
    palette_offset += palette_speed*dt;
    rainbow_offset += rainbow_speed*dt;
    
    // Main strip modes switch
    switch (mode)
        {
        case off:
            mainStrip_off_mode ();
            break;
        case fullWhite:
            mainStrip_fullWhite_mode ();
            break;
        case mono:
            mainStrip_mono_mode ();
            break;
        case fade:
            mainStrip_fade_mode ();
            break;
        case fade_switch:
            mainStrip_fade_switch_mode ();
            break;
        case fade_switch_random:
            mainStrip_fade_random_mode ();
            break;
        case rainbow_HSV:
            mainStrip_rainbow_HSV_mode ();
            break;
        case night:
            mainStrip_night_mode ();
            break;
        case RVD:
            mainStrip_RVD_mode ();
            break;
        case rise:
            {
            mainStrip_rise_mode ();
            break;
            }
        case oldschoolRND:
        case oldschool6:
            {
            mainStrip_oldschool_mode ();
            break;
            }
        case RVD_RND:
            {
            mainStrip_RVD_RND_mode ();
            break;
            }
        case fade_smooth:
            {
            mainStrip_fade_smooth_mode ();
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
            float len = float ((N_LEDS_TABLE + 2) / 2) * (float (VU_val) / ANALOG_VU_MAX);

            for (int i = N_LEDS_TABLE / 2; i >= 0; i--)
                {
                float brightness = (len > 1.f) ? 
                                   1.f :
                                   ((len > 0.f) ? 
                                    float (len) - int (len) :
                                    0.f);

                len--;

                leds_table [i] = CHSV (HSV_VU_START + HSV_VU_END * i / (N_LEDS_TABLE / 2), 
                                       MAX_SATURATION,
                                       MAX_BRIGHTNESS * brightness);
                    
                leds_table [N_LEDS_TABLE - i - 1] = leds_table [i];
                }

            break;
            }
        case VU_rain:
            {
            float len = float ((N_LEDS_TABLE + 2) / 2) * (float (VU_val) / ANALOG_VU_MAX);

            for (int i = N_LEDS_TABLE / 2; i >= 0; i--)
                {
                float brightness = (len > 1.f) ?
                    1.f :
                    ((len > 0.f) ?
                     float (len) - int (len) :
                     0.f);

                len--;

                leds_table [i] = CHSV (int (palette_offset + HSV_VU_END * i / (N_LEDS_TABLE / 2)),
                                       255, 
                                       MAX_BRIGHTNESS*brightness);
                
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
            for (int i = 0; i < N_LEDS_TABLE / 2; i++)
                {
                int curr_pos_freq_array = int (i * (SPECTRUM_SIZE-LOWEST) / (N_LEDS_TABLE / 2)) + LOWEST;

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
    
    // Creates fade effect when mode is switched
    if (millis () - mode_activation_time <= MODE_SWITCH_FADE_TIME * 1000 &&
        mode != oldschool6 && mode != oldschoolRND)
        {
        float dt = float (millis ()) - float (mode_activation_time);
        float perc = dt / float (MODE_SWITCH_FADE_TIME * 1000);
        if (perc > 1)
            perc = 1;

        // Generates average color
        for (int i = 0; i < N_LEDS_MAIN; i++)
            {
            leds_main [i] = CRGB ((leds_main [i].r * (perc) + leds_main_copy [i].r * (1 - perc)),
                                  (leds_main [i].g * (perc) + leds_main_copy [i].g * (1 - perc)),
                                  (leds_main [i].b * (perc) + leds_main_copy [i].b * (1 - perc)));

            }
        for (int i = 0; i < N_LEDS_TABLE; i++)
            {
            leds_table [i] = CRGB ((leds_table [i].r * (perc) + leds_table_copy [i].r * (1 - perc)),
                                   (leds_table [i].g * (perc) + leds_table_copy [i].g * (1 - perc)),
                                   (leds_table [i].b * (perc) + leds_table_copy [i].b * (1 - perc)));
            }
    
        }
    
    if (linearization)
        linearize ();
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
void StripController::fillSections (CRGB sections [N_SEC])
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

void StripController::setLinearization (bool lin)
    {
    linearization = lin;
    }

// Setters
void StripController::setMode (byte newMode)
    {
    if (newMode == mode &&
        newMode != oldschool6 && newMode != oldschoolRND &&
        newMode != RVD_RND)
        return;

    for (int i = 0; i < N_LEDS_MAIN; i++)
        leds_main_copy [i] = leds_main [i];

    fadeSwitchColor = CRGB::Red;
    rainbow_offset = rainbow_offset = 0.f;
    switchedColorFlag = false;
    mode = newMode;

    if (newMode == oldschool6)
        {
        for (int i = 0; i < N_SEC; i++)
            currColor3sections [i] = Wheel ((255 / 6) * (rand () % 6));
        }
    if (newMode == oldschoolRND)
        {
        for (int i = 0; i < N_SEC; i++)
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
    for (int i = 0; i < N_LEDS_TABLE; i++)
        leds_table_copy [i] = leds_table [i];

    table_mode = newMode;
    }
void StripController::setColor (CRGB newColor)
    {
    currColor = newColor;
    }
void StripController::setSectionColor (CRGB newColor, byte sec_id)
    {
    if (sec_id < 3)
        currColor3sections [sec_id] = newColor;
    }
void StripController::randomize ()
    {
    setRainbowFrequency (float (rand () % 60001 -
                         30000) / 60000.f);
    setRainbowSpeed (float (rand () % 50001 -
                     25000) / 100000.f);
    setColor (CRGB (CHSV (rand (),
              MAX_SATURATION,
              MAX_BRIGHTNESS)));
    byte mode = 0;
    while (mode == StripController::mainStripMode::off ||
           mode == StripController::mainStripMode::night ||
           mode == StripController::mainStripMode::rise)
        mode = rand () % StripController::n_modes;

    setMode (mode);
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

CRGB StripController::getFirstLEDcolor ()
    {
    return leds_main [0];
    }

// Modes defenitions
void StripController::mainStrip_off_mode ()
    {
    for (int i = 0; i < N_LEDS_MAIN; i++)
        leds_main [i] = CRGB::Black;
    }
void StripController::mainStrip_fullWhite_mode ()
    {
    for (int i = 0; i < N_LEDS_MAIN; i++)
        leds_main [i] = CRGB::White;
    }
void StripController::mainStrip_mono_mode ()
    {
    for (int i = 0; i < N_LEDS_MAIN; i++)
        leds_main [i] = currColor;
    }
void StripController::mainStrip_fade_mode ()
    {
    float brightness = (1 - cosf (double ((2 * PI)*rainbow_offset))) / 2.f;

    for (int i = 0; i < N_LEDS_MAIN; i++)
        {
        leds_main [i].r = currColor.r * brightness;
        leds_main [i].g = currColor.g * brightness;
        leds_main [i].b = currColor.b * brightness;
        }
    }
void StripController::mainStrip_fade_switch_mode ()
    {
    float brightness = (1 - cosf (double ((2 * PI)*rainbow_offset))) / 2.f;

    // Generates new color at low brightness
    bool newColorReady = false;
    if (rainbow_offset > 1.0 || rainbow_offset < -1.0)
        {
        rainbow_offset -= int (rainbow_offset);
        newColorReady = true;
        }
    if (newColorReady)
        {
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
    
    for (int i = 0; i < N_LEDS_MAIN; i++)
        {
        leds_main [i].r = fadeSwitchColor.r * brightness;
        leds_main [i].g = fadeSwitchColor.g * brightness;
        leds_main [i].b = fadeSwitchColor.b * brightness;
        }
    }
void StripController::mainStrip_fade_random_mode ()
    {
    float brightness = (1 - cosf (double ((2 * PI)*rainbow_offset))) / 2.f;
    
    // Generates new color at low brightness
    bool newColorReady = false;
    if (rainbow_offset > 1.0 || rainbow_offset < -1.0)
        {
        rainbow_offset -= int (rainbow_offset);
        newColorReady = true;
        }
    if (newColorReady)
        fadeSwitchColor = CRGB (CHSV (rand (), MAX_SATURATION, MAX_BRIGHTNESS));
        
    
    for (int i = 0; i < N_LEDS_MAIN; i++)
        {
        leds_main [i].r = fadeSwitchColor.r * brightness;
        leds_main [i].g = fadeSwitchColor.g * brightness;
        leds_main [i].b = fadeSwitchColor.b * brightness;
        }
    }

void StripController::mainStrip_rainbow_HSV_mode ()
    {
    for (int i = 0; i < N_LEDS_MAIN; i++)
        leds_main [i] = CHSV (i*(360 / N_LEDS_MAIN)*rainbow_freq + rainbow_offset * 360,
                              MAX_SATURATION,
                              MAX_BRIGHTNESS);
    }
void StripController::mainStrip_night_mode ()
    {
    float dt = float (millis () - mode_activation_time);
    float full_time = float (NIGHT_FADE_TIME) * 60.f * 1000.f;
    float brightness = float (1.f - (dt / full_time));
    if (brightness < 0)
        brightness = 0;

    for (int i = 0; i < N_LEDS_MAIN; i++)
        leds_main [i] = CRGB (NIGHT_COLOR.r*brightness, 
                              NIGHT_COLOR.g*brightness,
                              NIGHT_COLOR.b*brightness);
    }
void StripController::mainStrip_RVD_mode ()
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
    }
void StripController::mainStrip_rise_mode ()
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
    }
void StripController::mainStrip_oldschool_mode ()
    {
    // Fills new color
    if (rainbow_speed != 0)
        if (millis () - mode_activation_time >= (OLDSCHOOL_SWITCH_TIME * 60 * 1000) / (4 * abs (rainbow_speed)))
            setMode (mode);

    // Fades new color from black
    else if (millis () - mode_activation_time < FADE_SWITCH_TIME * 1000)
        {
        float percent = float (millis () - mode_activation_time) /
            float (FADE_SWITCH_TIME * 1000);

        CRGB sections [N_SEC] = {};
        for (int i = 0; i < N_SEC; i++)
            sections [i] = CRGB (currColor3sections [i].r*percent,
                                 currColor3sections [i].g*percent,
                                 currColor3sections [i].b*percent);

        fillSections (sections);
        }

    // Fills current colors
    else if (rainbow_speed == 0 ||
            (rainbow_speed != 0 && 
             millis () - mode_activation_time <
             float (OLDSCHOOL_SWITCH_TIME * 60 * 1000 - FADE_SWITCH_TIME * 1000) / (4 * abs (rainbow_speed))))
        {
        fillSections (currColor3sections);
        }
    else if (rainbow_speed != 0)
        if (millis () - mode_activation_time < 
            (OLDSCHOOL_SWITCH_TIME * 60 * 1000) / (4 * abs (rainbow_speed)))
        {
        float oneColorDuration = (OLDSCHOOL_SWITCH_TIME * 60 * 1000 - FADE_SWITCH_TIME * 1000) / (4 * abs (rainbow_speed));
        float percent = 1.f - float (millis () - mode_activation_time - oneColorDuration) / 
                              (float (FADE_SWITCH_TIME * 1000) / (4 * abs (rainbow_speed)));
        
        CRGB sections [N_SEC] = {};
        for (int i = 0; i < N_SEC; i++)
            sections [i] = CRGB (currColor3sections [i].r*percent,
                                 currColor3sections [i].g*percent,
                                 currColor3sections [i].b*percent);

        fillSections (sections);
        }
    }
void StripController::mainStrip_RVD_RND_mode ()
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
    }

void StripController::mainStrip_fade_smooth_mode ()
    {
    float k = abs (rainbow_offset);

    if (k > 1)
        {
        rainbow_offset -= int (rainbow_offset);
        currColor = nextColor;
        nextColor = Wheel (rand ());
        
        k--;
        }

    for (int i = 0; i < N_LEDS_MAIN; i++)
        {
        CRGB leds_color = CRGB (currColor.r * (1.f - k) + nextColor.r * k,
                                currColor.g * (1.f - k) + nextColor.g * k,
                                currColor.b * (1.f - k) + nextColor.b * k);

        leds_main [i] = leds_color;
        }
    }

void StripController::linearize ()
    {
    for (int i = 0; i < N_LEDS_TABLE; i++)
        {
        byte r = leds_table [i].r;
        byte g = leds_table [i].g;
        byte b = leds_table [i].b;

        r = 255.f * (1 - (log10 (256 - r) / log10 (256)));
        g = 255.f * (1 - (log10 (256 - g) / log10 (256)));
        b = 255.f * (1 - (log10 (256 - b) / log10 (256)));

        leds_table [i] = CRGB (r, g, b);
        }
    for (int i = 0; i < N_LEDS_MAIN; i++)
        {
        byte r = leds_main [i].r;
        byte g = leds_main [i].g;
        byte b = leds_main [i].b;

        r = 255.f * (1 - (log10 (256 - r) / log10 (256)));
        g = 255.f * (1 - (log10 (256 - g) / log10 (256)));
        b = 255.f * (1 - (log10 (256 - b) / log10 (256)));

        leds_main [i] = CRGB (r, g, b);
        }
    }
