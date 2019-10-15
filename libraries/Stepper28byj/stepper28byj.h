#ifndef _stepper28byj
#define _stepper28byj

#include "Arduino.h"


#define STS_IDLE 0
#define STS_CCW 1
#define STS_CW 2

#define ROUND_CCW   1
#define ROUND_CW   0

#define STEP_TIME   2500 //2500us

class stepper28byj{

    private:
        int status;
        int Position;
        int StopFlag;
        unsigned long lastTime,current_time;

        uint8_t smpin[4];
        uint8_t pin_phase_a;
        uint8_t pin_phase_b;
        uint8_t pin_enable;

    public:
        stepper28byj(uint8_t A0out,uint8_t A1out,uint8_t B0out,uint8_t B1out);
        void stop(void);
        void run(int direction); //move infinitely
        void moveStep(int step);     //move step
        void goPos(int pos);
        int getCurrentPos();
        void writeStep(uint8_t phase);
};

#endif