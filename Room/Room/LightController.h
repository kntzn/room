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

class LedController
    {
    private:
        // Contents
        StripController LED;
        BulbController Lamp = BulbController (RELAY_LAMP);
        BulbController Torchere = BulbController (RELAY_TORCHERE);

    public:
        LedController ();


        void update (float dt);

    };


#endif

