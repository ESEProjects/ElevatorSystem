#ifndef ANALOG_H_INCLUDED
#define ANALOG_H_INCLUDED
                                            
#include <stdio.h>
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


#define ANALOG_8BIT       0x08
#define ANALOG_10BIT      0x0A

void Analog_Init(unsigned char resolution_bits); // either 8 or 10 // 
unsigned int Analog_Read(unsigned char channel); 

 
                                             //CLEAR BIT /// 
#define ANALOG_ENABLE_PIN(ANPIN)  {    SETBIT_SHIFT(ATDDIEN, ANPIN); CLEARBIT_SHIFT(DDRAD, ANPIN); }// set input   
 
 
 
#define ANALOG_ENABLE_INPUT(ANPIN)  {    SETBIT_SHIFT(ATDDIEN, ANPIN); CLEARBIT_SHIFT(DDRAD, ANPIN); }// set input   
#define ANALOG_ENABLE_OUPUT(ANPIN)  {    SETBIT_SHIFT(ATDDIEN, ANPIN); SETBIT_SHIFT(DDRAD, ANPIN); }// set input   
 
   
  
  


#endif /* SWITCHES_ */