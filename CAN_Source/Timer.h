#ifndef TIMER_H_INCLUDED 
#define TIMER_H_INCLUDED

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

//#include "error.h" 


#define TIOS_OUTPUT_PIN(pin)      SETBIT(TIOS,pin);
#define TIE_ENABLE_PIN(pin)       SETBIT(TIE,pin);

#define TIOS_INPUT_PIN(pin)        CLEARBIT(TIOS,pin);
#define TIE_DISABLE_PIN(pin)       CLEARBIT(TIE,pin);


///
#define TSCR2_TIMER_PR_1    0x00  // bus clock /PR
#define TSCR2_TIMER_PR_2    0x01
#define TSCR2_TIMER_PR_4    0x02
#define TSCR2_TIMER_PR_8    0x03
#define TSCR2_TIMER_PR_16   0x04
#define TSCR2_TIMER_PR_32   0x05
#define TSCR2_TIMER_PR_64   0x06
#define TSCR2_TIMER_PR_128  0x07



#define TC_10US_DELAY_OFFSET 40



void Timer_Delay_10us(int k); 
void Timer_Delay_100us(int k);
void Timer_Delay_1ms(int k);
void Timer_Delay_10ms(int k); 



#define TC_10US_DELAY_OFFSET 40




void Timer_INIT(void); 
void Timer_Delay_10us(int k); 
void Timer_Delay_100us(int k);
void Timer_Delay_1ms(int k);
void Timer_Delay_10ms(int k); 

#define CLR_TIMER_CHNL_FLAG(chnl) \
          TFLG1 = (1 << (chnl))


#define TIMER_CHNL_INPUT_EDGE(chnl,value) \
          TCTL4 = (value << (chnl*2))



 
#define SET_TIMER_CLOCK_EDGE(chnl,action)  \
  FORCE_WORD(TCTL_3_4, 0x03 << ((chnl)*2), ( (action) << ((chnl)*2) ))
  

#define TIMER_CHNL_CAPTURE_DISA   0x00
#define TIMER_CHNL_CAPTURE_RISE   0x01
#define TIMER_CHNL_CAPTURE_FALL   0x02
#define TIMER_CHNL_CAPTURE_BOTH   0x03


#endif    
