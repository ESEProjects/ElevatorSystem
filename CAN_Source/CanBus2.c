#include "CanBus.h"



/*    CAN 2.0 A/B protocol as defined in the 
BOSCH specification
1
. It supports standard and extended data frames as well as remote frames with a 
programmable bit rate up to 1 Mbps
2

*/ 





/*
  CANTFLG  =  When TXE0, TXE1, TXE2 are  set to 1  buffer can be loaded with a new frame 
  
  while(CANTFLG&CANTFLG_TXE0_MASK) !=0) {}      // Wait for buffer to empty .//// not wise 
*/



/*
   MSCAN Recieve buffer 



*/


   

//// CAn 2.0 B -- extended /// 
                 
void Can_Initalize(void){
                      // prop_seg to 1 Tq */ 
 // Enter Init Mode
  CANCTL1 = (   CANCTL1_CANE_MASK ); 

  CANCTL0 =  CANCTL0_INITRQ_MASK;  
   //  wait for the Initalizastion mode ACK to clear /// 0 = running 1 = complete   
  while(!(CANCTL1&CANCTL1_INITAK_MASK)){ }
  // Exit Init Mode
  
   // CANCTL1 = (   CANCTL1_CANE_MASK |   CANCTL1_CLKSRC_MASK ); 

  CANCTL1 = 0x84; 


// Set SJW  // BAUD RATE // 
  CANBTR0=0x03;    /// set baud prescaler to 4 
  
  ///// TSEG 21 &20   3clock cycles    /// TSEG11    3 tq clock cycles 
  CANBTR1=0x32; // for a CAN Baud of 500kbps at 16Mhz crystal
// MSCAN is enabled    /// clock source is bus clock 


// FOR EXTENDED ALL FFOUR ACCEPTACNCE REGS ARE APPLIED CANIDAR 0-3 
          
/// MSCAN ACCEPTANCE REG // ACCEPT ALL           
CANIDAR0 = 0x00; 
CANIDAR1 = 0x00;
CANIDAR2 = 0x00;
CANIDAR3 = 0x00;   
CANIDAR4 = 0x00;
CANIDAR5 = 0x00;
CANIDAR6 = 0x00;
CANIDAR7 = 0x00;

     
          

  /// MSCAN Identifier mask registers 
CANIDMR0 = 0xFF;
CANIDMR1 = 0xFF;
CANIDMR2 = 0xFF; 
CANIDMR3 = 0xFF; 
CANIDMR4 = 0xFF; 
CANIDMR5 = 0xFF;
CANIDMR6 = 0xFF; 
CANIDMR7 = 0x0F; 
 
 //CANRXIDR0 -extended 

 
 // PAGE 307 --- set to 2 32bit acceptance filters 
 /// 312 - set CANIDMR1 and 5 to dont care 
 CANIDAC = 0x00; 
//CANCTL0 = 0x05;  /// CSWAI WUPE INITRQ   
 CANCTL0 &=  ~CANCTL0_INITRQ_MASK;
 while(((CANCTL1& 0x01) !=0)) {} 
 
 printf("Finished Can Bus Init"); 
 CANCTL1 = (CANCTL1_CANE_MASK); 
 CANRIER = (CANRIER_RXFIE_MASK); /// enable interrupts 
  return; 
}



/*
    CANCTL0////
    RXFRM- 1 a valid message was recieved, 0 no valid message was recieved. 
    RXACT- 0 MSCAN is transmitting or idle , 1 MSCAN is recieving a message 
    CSWAI- Used for low power not important set 0
    SYNCH- 0 MSCAN is synchronised to the can bus, 1 otherwise 
    TIME- Timer enable 0 disable 
    WUPE- 0 wake up disabled, wake up enabled 1 
    SLPRQ-  Sleep request - not important
    INITRQ- Initalize mode - 1 set MSCAN to init mode - resets the can regesters. 




///TIMER REGISTERS
 /*
     CANBTR0
     CANBTR1 
 */
 // CANBTR0= (CANBTR0_SJW0_MASK );      //0x20       /// sjw should be  2 
// Time Segment2 = 2 Clock Cycles      Time Segment1 = 10  clock cycles   /// 
//  CANBTR1 = (CANBTR1_TSEG20_MASK | CANBTR1_TSEG13_MASK |CANBTR1_TSEG11_MASK );      // 0x3A

/// RX Control 
 /*
  CANRFLG
  CANRIER 
   
 */
  
/// TX CTRLBITS  
 /*
 CANTARQ     /// Abort request 
 CANTBSEL
 CANTAAK    /// Abort ack
 CANTIER 
 CANTFLG 
   
 */ 
