#include "Analog.h"


void Analog_Init(unsigned char resolution_bits){
  
  // ATDCTL0 - 1 are reserved registers // not used 
/// not using interrupts // 
  ATDCTL2 = (ATDCTL2_ADPU_MASK); 
  // bit 7 ADPU=1 Power down 
  // bit 6 AFFC=0 ATD Fast Flag Clear All
  // bit 5 AWAI=0 ATD Power Down in Wait Mode
  // bit 4 ETRIGLE=0 External Trigger Level/Edge Control
  // bit 3 ETRIGP=0 External Trigger Polarity
  // bit 2 ETRIGE=0 External Trigger Mode Enable
  // bit 1 ASCIE=0 ATD Sequence Complete Interrupt Enable
  // bit 0 ASCIF=0 ATD Sequence Complete Interrupt Flag
  
  ATDCTL3 = (ATDCTL3_S8C_MASK);  // conversion once

  // 10 bit res if res = 10 or 0 // else 8 bits 
  if(resolution_bits == 0 || resolution_bits == 10)
     ATDCTL4 = (ATDCTL4_PRS2_MASK | ATDCTL4_PRS2_MASK);
  
  else 
     ATDCTL4 = (ATDCTL4_SRES8_MASK | ATDCTL4_PRS2_MASK | ATDCTL4_PRS2_MASK);
        
  ATDCTL5 = (ATDCTL5_Cx_BITNUM | ATDCTL5_DSGN_MASK | ATDCTL5_CB_MASK); 
  
  return; 
}


unsigned int Analog_Read(unsigned char channel){
  unsigned int analog_input =0;  
  
  ATDCTL5 = 0x80 | (unsigned char)channel;  // start sequence
  
   while(  !(ATDSTAT1 & ATDSTAT0_SCF) && !(analog_input == 0xFF) ){    // flag set to 1 when complete // ff for time out 
    analog_input++;
   } //

   analog_input= (ATDDR0);

  return analog_input; 
}





/*                                                                                  not necessary // 
     //   analog_pointer = (unsigned int  *)(0x0090+(0x02*(unsigned char)channel) );

void Analog_Init(void){

  // ATDCTL0 - 1 are reserved registers // not used 

  ATDCTL2 =0xE0; // enable ADC , fast flag, power down in wait
  // bit 7 ADPU=1 Power down 
  // bit 6 AFFC=0 ATD Fast Flag Clear All
  // bit 5 AWAI=0 ATD Power Down in Wait Mode
  // bit 4 ETRIGLE=0 External Trigger Level/Edge Control
  // bit 3 ETRIGP=0 External Trigger Polarity
  // bit 2 ETRIGE=0 External Trigger Mode Enable
  // bit 1 ASCIE=0 ATD Sequence Complete Interrupt Enable
  // bit 0 ASCIF=0 ATD Sequence Complete Interrupt Flag
  
  ATDCTL3 = 0x08;  // conversion once

  
  ATDCTL4 = 0x81; // enable ADC     // divide by 4
 
  
  return; 
}


unsigned int Analog_Read(unsigned char channel){
  unsigned int analog_input =0;
  
  ATDCTL5 = 0x80 | (unsigned char)channel;  // start sequence
  
 while(  !(ATDSTAT1 & ATDSTAT0_SCF) && !(analog_input == 0xFF) ){    // flag set to 1 when complete // ff for time out 
 analog_input++;
 } //
   analog_input= 0; 
  analog_input= (ATDDR0);

  return analog_input; 
}



*/