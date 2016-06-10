#ifndef MACROS_H_INCLUDED 
#define MACROS_H_INCLUDED


#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
            
            
#define LOW(BIT)                            ((BIT)&0xFF)            
            
#define SETBIT_SHIFT(ADDRESS,BIT)           (ADDRESS |= (1<<BIT))
#define CLEARBIT_SHIFT(ADDRESS,BIT)         (ADDRESS &= ~(1<<BIT) )
#define FLIPBIT_SHIFT(ADDRESS,BIT)          (ADDRESS ^= (1<<BIT))
#define CHECKBIT_SHIFT(ADDRESS,BIT)         (ADDRESS & (1<<BIT))

#define EQUATE(PORT, VALUE)                   ((PORT) = (VALUE))
#define SETBIT(ADDRESS,BIT)           (ADDRESS |= (BIT))
#define CLEARBIT(ADDRESS,BIT)         (ADDRESS &= LOW(~(BIT)))
#define FLIPBIT(ADDRESS,BIT)          (ADDRESS ^= (BIT))
#define CHECKBIT(ADDRESS,BIT)         (ADDRESS & (BIT))
//#define Delay_1us()  asm("nop"); 
//#define Delay_125ns(x)  for(;x >0;x--) asm("nop"); 

//#define Delay_125ns(x)                 \
//  for (;;) {                \
 //    asm("nop");                      \
//  }



#define DISABLE_PWM_CHNL(chan)     CLEARBIT_SHIFT(PWME, chan)
#define ENABLE_PWM_CHNL(chan)      SETBIT_SHIFT(PWME, chan)


/// TIMER
#define TIMER_MK_CHN_OC(chan)      SETBIT_SHIFT(TIOS, chan)   // output compare
#define TIMER_MK_CHN_IC(chan)      CLEARBIT_SHIFT(TIOS, chan) // iNPUT cOMPARE

#define TIMER_CHN_FLAG_CLR(chan)        SETBIT_SHIFT(TFLG1,chan); 



       
#endif    `