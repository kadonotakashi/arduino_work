#include "kdn_RotaryEncoder.h"

kdn_RotaryEncoder::kdn_RotaryEncoder(uint8_t EncoderA,uint8_t EncoderB,uint8_t Button)
{
    A_pin = EncoderA;
    B_pin = EncoderB;
    Button_pin = Button;

    pinMode(A_pin, INPUT_PULLUP);
    pinMode(B_pin, INPUT_PULLUP);
    pinMode(Button_pin, INPUT_PULLUP);
}

int kdn_RotaryEncoder::CheckRotaryEncoder() {
    static uint8_t  crntRESTS;
    static uint8_t  prevRESTS;
    static uint8_t  validflag;

    prevRESTS = crntRESTS;
    crntRESTS = digitalRead(A_pin) << 1 | digitalRead(B_pin);

    if (prevRESTS == 0x03) {
        validflag = 1;
    }
    if ((crntRESTS == 0x01 ) && (validflag == 1)) {
        validflag = 0;
        return -1;
    }
    else if ((crntRESTS == 0x02 ) && (validflag == 1)) {
        validflag = 0;
        return 1;
    }
    else {
        return 0;
    }
}

int kdn_RotaryEncoder::CheckButton(){

#define TIME_DeGlich    20
    static uint8_t  DglCnt;
    static uint8_t  crntBtnSts; //L:push H:release
    static uint8_t  prevBtnSts;

    prevBtnSts = crntBtnSts;
    crntBtnSts = digitalRead(Button_pin);

    if (crntBtnSts == 1){
        if ((prevBtnSts == 0) && (DglCnt==0)) {
            DglCnt = TIME_DeGlich;
            return 1;
        }
        DglCnt = TIME_DeGlich;
        return 0;
    }
    else{
        if(DglCnt>0){
            DglCnt--;
            return 0;
        }
    }
    return 0;
}

