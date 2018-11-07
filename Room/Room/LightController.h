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

#include "Predefined.h"
#include "Analyzer.h"

class LightController
    {
    private:
        // Contents
        StripController LED;
        BulbController Lamp = BulbController (RELAY_LAMP);
        BulbController Torchere = BulbController (RELAY_TORCHERE);

    public:
        LightController ();


        void update (float dt);

        void syncWithAnalyzer (Analyzer &analyzer, float dt);
    };


#endif

