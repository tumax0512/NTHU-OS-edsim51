#include <8051.h>
#include "preemptive.h"


__data __at (0x30) char savedSP[4];
__data __at (0x34) char bitmap;
__data __at (0x35) char cur_threadID;
__data __at (0x36) char count;
__data __at (0x37) char temp;
__data __at (0x38) char new_threadID;
   

#define SAVESTATE \
    __asm \
        push ACC\
        push B\
        push DPL\
        push DPH\
        push PSW\
    __endasm; \
    savedSP[cur_threadID] = SP;
        

#define RESTORESTATE \
    SP = savedSP[cur_threadID];\
    __asm \
        pop PSW\
        pop DPH\
        pop DPL\
        pop B\
        pop ACC\
    __endasm; 


extern void main(void);

void Bootstrap(void) {
      
      TMOD = 0;  // 設定 Timer 0 為 Mode 0
      IE = 0x82;  // enable timer 0 interrupt; keep consumer polling
                // EA  -  ET2  ES  ET1  EX1  ET0  EX0
      TR0 = 1; // set bit TR0 to start running timer 0
      bitmap = 0;
      cur_threadID = ThreadCreate( main );
      RESTORESTATE;



}


ThreadID ThreadCreate(FunctionPtr fp) {
   EA = 0; //關掉
        if( bitmap == 0b1111) return -1;
         //a
        if( !( bitmap & 0x01 ) ){ //thread 0可用 xxx0
            bitmap = bitmap | 1;
            new_threadID = 0;
        }
        else if( !( bitmap & 0x02 ) ){ //thread 1可用 xx0x
            bitmap = bitmap | 2;
            new_threadID = 1;
        }
        else if( !( bitmap & 0x04 ) ){ //thread 2可用 x0xx
            bitmap = bitmap | 4;
            new_threadID = 2;
        }
        else if( !( bitmap & 0x08 ) ){ //thread 3可用 0xxx
            bitmap = bitmap | 8;
            new_threadID = 3;
        }
         //b,c
         temp = SP;
         SP = (0x3F) + (0x10) * new_threadID;
         //d,e
         __asm
            push DPL
            push DPH
            MOV A, #0
            push ACC
            push ACC
            push ACC
            push ACC
         __endasm;
         //f
         PSW = new_threadID << 3;
         __asm 
            push PSW
         __endasm;
         //g
         savedSP[new_threadID] = SP;
         //h
         SP = temp;
         //i
    EA = 1; //打開
         return new_threadID;        
}



void ThreadYield(void) {
   EA = 0; //關掉
       SAVESTATE;
       do {
                
               cur_threadID = (cur_threadID < 3 ) ?  cur_threadID+1 : 0; //往下一個switch 超過3就->0
               if(bitmap & (1<<cur_threadID)) break;
                
        } while (1);
        RESTORESTATE;
   EA = 1; //打開
}

void myTimer0Handler(void) {
   EA = 0; //關掉
       SAVESTATE;
       __asm 
         MOV A, R0
         PUSH ACC
         MOV A, R1
         PUSH ACC
         MOV A, R2
         PUSH ACC
         MOV A, R3
         PUSH ACC
         MOV A, R4
         PUSH ACC
         MOV A, R5
         PUSH ACC
         MOV A, R6
         PUSH ACC
         MOV A, R7
         PUSH ACC
      __endasm;
       do {    
            cur_threadID = (cur_threadID < 3 ) ?  cur_threadID+1 : 0;
            temp=(1<<cur_threadID);
         if( bitmap & temp ){//皆被使用
            break;
         }     
                
        } while (1);
        __asm
         POP ACC
         MOV R7, A
         POP ACC
         MOV R6, A
         POP ACC
         MOV R5, A
         POP ACC
         MOV R4, A
         POP ACC
         MOV R3, A
         POP ACC
         MOV R2, A
         POP ACC
         MOV R1, A
         POP ACC
         MOV R0, A
      __endasm;  
        RESTORESTATE;
   EA = 1; //打開
   __asm 
      RETI
   __endasm;
}
void ThreadExit(void) {
        
        RESTORESTATE;
}