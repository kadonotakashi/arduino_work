#ifndef kdnr_re_h
#define kdnr_re_h

#include "Arduino.h"

class kdn_RotaryEncoder
{
    private:
        uint8_t A_pin;
        uint8_t B_pin;
        uint8_t Button_pin;

    public:
        kdn_RotaryEncoder(uint8_t EncoderA,uint8_t EncoderB,uint8_t Button);
        int CheckRotaryEncoder();
        int CheckButton();
};



#endif