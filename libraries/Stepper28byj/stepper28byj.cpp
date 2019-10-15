#include "stepper28byj.h"
int UniHalf[8][4] = {
//    A0,B0,A1,B1
    {0, 0, 0, 1},  
    {0, 0, 1, 1},  
    {0, 0, 1, 0},  
    {0, 1, 1, 0},  
    {0, 1, 0, 0},  
    {1, 1, 0, 0},  
    {1, 0, 0, 0},  
    {1, 0, 0, 1},  
};

int BiPolar[4][2] = {
//    A,B
    {0, 0},  
    {1, 0}, 
    {1, 1},
    {0, 1}
};

void stepper28byj::writeStep(uint8_t phase){
//    Serial.printf(" :  %d", phase);
 
    for(int i=0;i<4;i++){
        digitalWrite(smpin[i], UniHalf[phase][i]);
    }
}

void stepper28byj::stop(void){
    StopFlag=1;
}

int stepper28byj::getCurrentPos(){
    return Position;
}

stepper28byj::stepper28byj(uint8_t A0out,uint8_t A1out,uint8_t B0out,uint8_t B1out){

    smpin[0] = A0out;
    smpin[1] = A1out;
    smpin[2] = B0out;
    smpin[3] = B1out;

    for(int i=0;i<4;i++){
        pinMode(smpin[i], OUTPUT);
    }

    lastTime = micros();
    for(int i=7;i>=0;i--){
        current_time = micros();
        while((current_time - lastTime)<STEP_TIME){
            current_time = micros();
        }
        writeStep(i);
        lastTime = micros();
    }
    Position = 0;
    status = STS_IDLE;
    StopFlag = 0;
}


void stepper28byj::run(int direction){
    uint8_t phase = (uint8_t )(Position % 8);
    lastTime = micros();

    if(direction == ROUND_CW){
        while(1){
            if(StopFlag!=0){
                StopFlag=0;
                break;
            }

            current_time = micros();
            while((current_time - lastTime)<STEP_TIME){
                current_time = micros();
            }
            writeStep(phase);
            Position++;
            if(phase==7){
                phase=0;
            }else{
                phase++;
            }
            lastTime = micros();
        }
    }else{
        while(1){
            if(StopFlag!=0){
                StopFlag=0;
                break;
            }
            current_time = micros();
            while((current_time - lastTime)<STEP_TIME){
                current_time = micros();
            }
            writeStep(phase);
            Position--;

            if(phase==0){
                phase=7;
            }else{
                phase--;
            }
            lastTime = micros();
        }
    }
} 

void stepper28byj::moveStep(int step){
    uint8_t phase;
    int pos;
    lastTime = micros();

    pos = Position+step;

    if(step == 0){
        return;
    }
    else if(pos > Position){
        for( ; Position <  pos;){
            if(StopFlag!=0){
                StopFlag=0;
                break;
            }
            current_time = micros();
            while((current_time - lastTime)<STEP_TIME){
                current_time = micros();
            }
            Position++;
            phase = (uint8_t )(Position & 0x7);
            writeStep(phase);
            lastTime = micros();
        }
    }else{  //if(pos < Position)
        for( ; Position > pos;){
            if(StopFlag!=0){
                StopFlag=0;
                break;
            }
            current_time = micros();
            while((current_time - lastTime)<STEP_TIME){
                current_time = micros();
            }
            Position--;
            phase = (uint8_t )(Position & 0x7);
            writeStep(phase);
            lastTime = micros();
        }
    } 
} 

void stepper28byj::goPos(int pos){
    uint8_t phase;

    lastTime = micros();

    if(pos == Position){
        return;
    }
    else if(pos > Position){
        for( ; Position <  pos;){
            if(StopFlag!=0){
                StopFlag=0;
                break;
            }
            current_time = micros();
            while((current_time - lastTime)<STEP_TIME){
                current_time = micros();
            }
            Position++;
            phase = (uint8_t )(Position & 0x7);
            writeStep(phase);
            lastTime = micros();
        }
    }else{  //if(step < 0){
        for( ; Position > pos;){
            if(StopFlag!=0){
                StopFlag=0;
                break;
            }
            current_time = micros();
            while((current_time - lastTime)<STEP_TIME){
                current_time = micros();
            }
            Position--;
            phase = (uint8_t )(Position & 0x7);
            writeStep(phase);
            lastTime = micros();
        }
    } 

}