// 
// 
// 

#include "Button.h"



void Button::reset ()
    {
    flag = false;
    state = buttonState::Prsd;
    }

Button::Button (byte b_pin)
    {
    pin = b_pin;
    pinMode (b_pin, INPUT_PULLUP);
    }

void Button::update ()

    {
    if (!digitalRead (pin))
        {
        if (flag == false)
            {
            flag = true;
            last_press = millis ();
            state = buttonState::Prs;
            }
        else
            {
            if (millis () - last_press > HOLD_TIME)
                {
                if (hold_flag == false)
                    {
                    hold_flag = true;
                    state = buttonState::Hold;
                    }
                else
                    state = buttonState::Unpr;
                }

            else
                state = buttonState::Prsd;
            }
        }
    else
        {
        if (flag == true && hold_flag == false)
            {
            flag = false;
            state = buttonState::Rlsd;
            }
        else
            {
            state = buttonState::Unpr;
            hold_flag = false;
            flag = false;
            }

        }

    if (state != buttonState::Unpr)
        last_button_activity = millis ();
    }

byte Button::getState ()
    {
    return state;
    }

