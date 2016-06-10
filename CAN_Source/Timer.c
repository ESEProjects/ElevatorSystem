#include "Timer.h" 





void Timer_INIT(void){    
    TSCR1 = (TSCR1_TFFCA_MASK | TSCR1_TEN_MASK );      /* enable TCNT and fast timer flag clear */
    TSCR2 = TSCR2_TIMER_PR_4;    //8 us per count
     
    //TIMER_CHNL_INPUT_EDGE(0x00, TIMER_CHNL_CAPTURE_BOTH);  
    
   // TIOS |= (TIOS_IOS2_MASK) | (TIOS_IOS7_MASK) | (TIOS_IOS6_MASK); // set T2 & T7 for output                 
    
  //  TIE |= ( TIE_C6I_MASK);// enable interrupts on 0 and 2 
}
   // place error check for 65,535 // because shit /// 


   // get rid of magic numbers 
void Timer_Delay_10us(int k) {
// setup vars = 5us delay
    unsigned long delay_calculate =0;
     TIOS |=  TIOS_IOS5_MASK;       /* enable OC0 function */
     
     delay_calculate = 125*k; // approx 1.25 ticks per 10us  
     delay_calculate /= 100;  // divide by 100 to make it equal to number of ticks necessary 
   // if(delay_calculate > 0xFFFF)
   //  ERROR_PRINT_SCREEN_2("TC5 - Delay 10us Generic Delay too large, beyond max int, please replace", 11);
    
    
    TC5 = TCNT + (unsigned int)delay_calculate; /* start an OC0 operation */
    
    // HOLD TILL FLAG CLEAR 
  while(!(TFLG1 &TFLG1_C5F_MASK )); 
} 
 
void Timer_Delay_100us(int k) {
// setup vars = 5us delay
    unsigned long delay_calculate =0;
     TIOS |=  TIOS_IOS5_MASK;       /* enable OC0 function */
     
     delay_calculate = 125*k; // approx 1.25 ticks per 10us  
     delay_calculate /= 10;  // divide by 100 to make it equal to number of ticks necessary 
    
  //  if(delay_calculate > 0xFFFF)
   //  ERROR_PRINT_SCREEN_2("TC5- Delay 100us  Generic Delay too large, beyond max int, please replace ", 11);
    
    TC5 = TCNT + (unsigned int)delay_calculate; /* start an OC0 operation */
    
    // HOLD TILL FLAG CLEAR 
  while(!(TFLG1 &TFLG1_C5F_MASK )); 
} 




void Timer_Delay_1ms(int k) {
  unsigned long delay_calculate =0;
     TIOS |=  TIOS_IOS5_MASK;       /* enable OC0 function */
     
     delay_calculate = 125*k*4; // approx 1.25 ticks per 10us  
   //if(delay_calculate > 0xFFFF)
   //  ERROR_PRINT_SCREEN_2("TC5 - Delay 1ms Generic Delay too large, beyond max int, please replace max delay is 520ms", 11);    
    
    TC5 = TCNT + (unsigned int)delay_calculate; /* start an OC0 operation */
  while(!(TFLG1 & TFLG1_C5F_MASK )); /* wait for PT0 to go high */
} 


void Timer_Delay_10ms(int k) {
// setup vars = 5us delay
    unsigned long delay_calculate =0;
    TIOS |=  TIOS_IOS5_MASK;       /* enable OC0 function */

   delay_calculate = 1250; // 620 gives delay of 10ms 
    delay_calculate *= k; // multiply by desired delay 
   // total_delay -= 1; // sub 5us from delay // as setup takes 5us
      
  //  if(delay_calculate > 0xFFFF)
   //  ERROR_PRINT_SCREEN_2("TC5- Delay 10ms  Generic Delay too large, beyond max int, please replace max delay is 520ms", 11);
      
     TC5 = TCNT + (unsigned int)delay_calculate; /* start an OC0 operation */
  while(!(TFLG1 & TFLG1_C5F_MASK )); /* wait for PT0 to go high */
} 


//delay 10us 
// delay 100us
/// delay 1ms 
/// delay 10ms 
/// delay 100ms 
