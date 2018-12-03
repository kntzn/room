// LightController.h

#ifndef _LIGHTCONTROLLER_h
#define _LIGHTCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LightBulbController.h"
#include "StripController.h"
#include "DoorSensor.h"

#include "Predefined.h"
#include "Analyzer.h"

class LightController
    {
    public:
        enum profiles
            {
            off,
            full,
            noLed,
            film,
            night,
            rise,
            def
            };

    private:
        // Contents
        StripController LED;
        BulbController Lamp = BulbController (RELAY_LAMP);
        BulbController Torchere = BulbController (RELAY_TORCHERE);

        // ---------------------------
        // Util 
        byte lastAnimationMode = StripController::off;
        byte profile = 0;
        void restartAnimation ();

    public:
        LightController ();

        // ---------------------------
        // Main functions

        // Upates all light sources
        void update (float dt, DoorSensor* sens);

        // Gets information from analyzer and sets parameters to led strips
        void syncWithAnalyzer (Analyzer &analyzer, float dt);
        
        // Sets premade profile
        void setProfile (byte mode, DoorSensor* sens);

        // ---------------------------
        // Led strip parameters setters

        // Sets the mode of main led strip
        void setLedMode (byte newMode);
        // Sets the mode for table led strip
        void setLedTableMode (byte newMode);
        // Sets main color of LED
        void setLedColor (CRGB newColor);
        // Sets frequency of the animation
        // e.g. rainbow frequency
        void setLedAnimationFrequency (float newFreq);
        // Sets speed of the animation
        void setLedAnimationSpeed (float newSpeed);
        // Sets speed of the rainbow in VU_rain LED mode
        void setLedAnimationSpeedVU (float newSpeed);
        // Sets speed of the rainbow's color change 
        // in Freq_full LED mode
        // i.e. how fast does color changes while moving from LED's center
        void setLedAnalyzerAnimationFrequency (float newFreq);
        // Sets beginning of the rainbow's color 
        // in Freq_full LED mode
        // i.e. color at the center of LED strip
        void setLedAnalyzerAnimationOffset (float newOffset);
        
        // ---------------------------
        // Lamps parameters setters

        // Sets the mode of the torchere
        void setTorchereState (byte newMode);
        // Sets the mode of the lamp
        void setLampState (byte newMode);

        // ---------------------------
        // Getters
        byte getProfile ();
    };


#endif

