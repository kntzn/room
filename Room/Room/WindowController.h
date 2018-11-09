// WindowController.h

#ifndef _WINDOWCONTROLLER_h
#define _WINDOWCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Hbridge.h"

class WindowController
    {
    public:
        enum state
            {
            closed_in,
            opened,
            closed_out
            };
    private:
        byte mode = closed_in, prev_mode = closed_in;
        Hbridge bridge = Hbridge (255, 255);
        unsigned long int mode_switch = 0;

    public:
        WindowController ();

        void setMode (byte newMode);

        void update ();

    };

#endif