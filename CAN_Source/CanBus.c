#include "CanBus.h" 

#define FLOOR_ACQUISITION_PROCESS   0xFA


unsigned int Can_Approved_Addresses[8] = {
  CAN_MASTER_AD,  //MASTER
  CAN_FLOOR_1_AD,  //Floor1
  CAN_FLOOR_2_AD,  //Floor2
  CAN_FLOOR_3_AD,  //Floor3
  CAN_ELEV_CONT_AD,  //Elevator Controller
  0x0005,  //PID CONTROLLER
  0x0006,  /// NT USED 
  0x0007
};




/*-----------------------------------------------------------------------*/
/*  Floor LCD                                               */
/*-----------------------------------------------------------------------*/

void Floor_Elevator_LCD(unsigned char current_floor){

    LCDprintf("Elevator @ %d \n",current_floor);
    LCDprintf("Cur Floor %d    \n",CAN_CURRENT_ADDRESS);

}






///CAN PROCESS PING USED BY MASTER 
void CAN_Process_Ping_Master(unsigned char senderid, unsigned char compid, unsigned char compinst){
  unsigned char sender_address;
  //// 
   
    Can_Send(senderid,ping_master_respond,3);
    
    #if  PRINTF_DEBUG_PING
    printf("\n Send Ping to Slave %d", senderid); 
    #endif 
    
  
       EnableInterrupts;
        ping_rmr = 1;
       DisableInterrupts;

  return; 
}


void CAN_Process_Ping_Slave(unsigned char senderid, unsigned char compid, unsigned char compinst){   
      EnableInterrupts;
        ping_rmr = 1;
       DisableInterrupts;
       #if  PRINTF_DEBUG_PING
       printf("\n Ping %d Recieve from master", (unsigned char)compinst);
       #endif
  return; 
}





/*-----------------------------------------------------------------------*/
/*  CAN_Print_Structure prints  structure                                */
/*-----------------------------------------------------------------------*/
void CAN_Print_Structure(struct CAN_Structure Current_Can){
unsigned char i, data_length, data;
   
  data_length = Current_Can.Data_Length; 
  if(data_length > 7)
     data_length = 7; 
  
    printf("\n CAN: ADD:0x%lx  DATA: (", Current_Can.Identifier); 
     
     for (i = 0; i < data_length; i++){     
        data = Current_Can.Data[i];
        printf(".0x%x ", data); 
    }
     printf(")   length: %d", data_length);  
      

  return; 
}



/*-----------------------------------------------------------------------*/
/*   Can_Initalize 
/*         - 500kbps                           
/*-----------------------------------------------------------------------*/
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


/*-----------------------------------------------------------------------*/
/*   Can_Send 
/*        -give array of data, give long address and len of data    
/*        
/*-----------------------------------------------------------------------*/
char Can_Send(unsigned long address, char *txdata, int len){

  unsigned char counter=0; 
  unsigned char priority = 1; 
  unsigned char txbuffer =0; 

  
      /*Check the transmit buffer, is it full? */
      if(!CANTFLG) 
         return ERR_BUFFER_FULL; 
      
      CANTBSEL = CANTFLG; /*Select Empty Buffer*/
     
     do{
        txbuffer = CANTBSEL; // Backup the buffer
     }while (txbuffer ==0); 

  
     Can_Send_Address(address); 
               

  /* Load the DATA into the data register*/
     for(counter=0; counter < len; counter++){
      *(&CANTXDSR0 + counter) = txdata[counter]; 
     
     }

    CANTXDLR = len;  //send the length 
    CANTXTBPR = priority;  // send the priorit
    CANTFLG = txbuffer; 
     
   
    /// hangs while we wait 
   while((CANTFLG & txbuffer) ){} // wait for completetion 

}

/*-----------------------------------------------------------------------*/
/*   Can_Send_Address
/*      -Takes in an address and writes to the respective registers 
/*        for the hcs12 
/*-----------------------------------------------------------------------*/
void Can_Send_Address(unsigned long address){
  unsigned long address_long=0;  
  unsigned char stdaddress_flag =1;
  unsigned char address_temp, address_temp2 = 0;
    
     if(address <= 0x7FF)
       stdaddress_flag = 1; 
     else 
       stdaddress_flag = 0; 

  /* Load the ID into the id register */     
  //10 bits
   if(stdaddress_flag){
   // Grab bits 10-3 
   
      address_long =  ((address & 0x7F8))  ;
      address_long >>=3; 
      address_temp = address_long &0xFF;
      
    /// send bits 10-3
     *(&CANTXIDR0) = (unsigned char)(address_temp &0xFF);    
     
      address_temp =  (((address) &0x07) << 5);
     *(&CANTXIDR1) = address_temp;// address_temp&0x70; //((unsigned char)(address_temp)&0x07);
   } else{
   //28 bits extended 
      address_long = address; 
      address_long >>= 21; 
      address_temp = (address_long & 0xFF); 
      *(&CANTXIDR0) = (unsigned char)address_temp; 
      
      address_long = address; 
      address_long >>= 13; 
      // extended ID 
      address_temp = (address_long & 0xE0) + ((address_long & 0x1C) >>2) + (CANTXIDR1_IDE_MASK);
      *(&CANTXIDR1) = (unsigned char)address_temp; 
      address_temp2 =   ((address_long & 0x03)) << 6; 
       
      address_long = (address); 
      address_long >>= 7;
      address_temp = (address_long & 0x3f) + address_temp2;
      *(&CANTXIDR2) = (unsigned char)address_temp; 
      
      
      address_long = address; 
      address_long <<= 1;
      address_temp = (address_long & 0xfe);
      *(&CANTXIDR3) = (unsigned char)address_temp; 
      
   }
  
   
 }




// * useless// we have an interrupt 
void Can_Setup_Recieve(char *filter){
 unsigned char datain[7]; 
 unsigned char len =0; 
 unsigned int counter=0; 
   /* Load the ID into the id register */
  //  for(counter=0; counter < 2; counter++){
 //       *(&CANTXIDR0 + counter) = id[counter];
   // }

/* Load the DATA into the data register*/
  // for(counter=0; counter < len; counter++){
 //   *(&CANTXDSR0 + counter) = txdata[counter]; 
  //  printf("\n %d: %d 0x%x", counter,txdata[counter],txdata[counter]);  
   
 //  }

//  CANTXDLR = len;  //send the length 
//  CANTXTBPR = priority;  // send the priority

//  CANTFLG = txbuffer; 
  


}

/*
Out of Reset: 
-Enable CAN module 
by setting CANE bit of CANnCTL1 to 1.
-Request to enter Initialization Mode by setting INITRQ bit of CANnCTL0 to 1.
-Wait until Initialization Mode is entered by waiting until the INITAK bit of CANnCTL1 is 1.
-Clear INITRQ bit of CANnCTL0 to leave Initialization Mode and enter Normal Mode.

Normal Mode:
-Make  sure  MSCAN transmission  queue  is  empty  and  bring  the  module  into  sleep  mode  by  asserting  the  SLPRQ  bit
(CANnCTL0 register) and waiting for the SLPAK bit (CANnCTL1 register)
to be ‘1’.
-Enter the Initialization Mode
-Configure CAN parameters: Write to the Configuration Registers in Initialization Mode.
-Clear INITRQ bit of CANnCTL0 to leave the Initialization Mode and enter Normal Mode.

*/




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
