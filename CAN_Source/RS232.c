/*
Serial libraries -
Serial 
        PS0 - RX
        PS1 - TX 
        
              
*/ 


#include "RS232.h"

 unsigned char RS232_COMPARE; 


unsigned char RS232_Init(unsigned long baudRate){
   unsigned char rs232_clock=0;
   
    RS232_BAUD_RATE = (unsigned long)baudRate;
   // Set Control register  
    RS232_CONTROL_REG =   RS232_STATUS_N81;  // N81 data
 
    /* bit value meaning
    7   0    LOOPS, no looping, normal
    6   0    WOMS, normal high/low outputs
    5   0    RSRC, not appliable with LOOPS=0
    4   0    M, 1 start, 8 data, 1 stop
    3   0    WAKE, wake by idle (not applicable)
    2   0    ILT, short idle time (not applicable)
    1   0    PE, no parity
    0   0    PT, parity type (not applicable with PE=0) */ 
 
    RS232_CONTROL_REG2 = 0x2C; // (SCICR2_RIE_MASK)   | (SCICR2_RE_MASK | SCICR2_TE_MASK);   // arm only the RDRF
  
    /* bit value meaning
    7   0    TIE, no transmit interrupts on TDRE
    6   0    TCIE, no transmit interrupts on TC
    5   0    RIE, no receive interrupts on RDRF
    4   0    ILIE, no interrupts on idle
    3   1    TE, enable transmitter
    2   1    RE, enable receiver
    1   0    RWU, no receiver wakeup
    0   0    SBK, no send break */ 
  
   //SET BAUDRATE 
  // allow for higher ECLOCKS 
  rs232_clock = (E_CLOCK) / 8000000;  // default is 8MZ
  
  // SCI Baudrate = SCI Clock / (16 * SCIBDL) -- Eclock 8MHZ 
  /// so far so good pretty damn accurate 
  switch(baudRate){
    case 2400:  
      RS232_BAUDLB_REG=(208*rs232_clock); 
      break;
    case 4800:  
      RS232_BAUDLB_REG=(104*rs232_clock); 
      break;
    case 9600:  
      RS232_BAUDLB_REG=(52*rs232_clock);  
      break;
    case 19200:
      RS232_BAUDLB_REG=(26*rs232_clock);  
      break;
    case 38400:
      RS232_BAUDLB_REG=(13*rs232_clock);  
      break;
    case 83200:
      RS232_BAUDLB_REG=(6*rs232_clock);  
      break;  
    case 166400:
      RS232_BAUDLB_REG=(3*rs232_clock);  
      break;
    case 499200:
      RS232_BAUDLB_REG=(rs232_clock);  
      break;  
     
    default:    
      RS232_BAUDLB_REG = (52*rs232_clock) ;  // default to 9600 
  }
   RS232_BAUDHB_REG = 0;  // upper bits are 0 
    
    
    
   if (SCISR1 & 0x80){ /*Poll TDRE flag*/
     // printf("TDRE OK"); /*TDRE set, return OK*/
    }
    
   else{
    /*TDRE clear, return ERROR*/
    //  printf("TDRECLEAR ERROR");
      return 0x05;    //EIO      5  /* I/O error */
    }
    
 // printf("\n RS232 Setup");   
        
  return 0;
}


char RS232_Write(unsigned char data){
  while(!(RS232_STATUS_REG & SCISR1_TDRE_MASK));    // READ TDRE FLAG
    SCIDRL = data;
  return 0;
}



char RS232_Write_String(unsigned char *data){
  while(*data)
    RS232_Write(*data++);  
 return 0;
}



unsigned char RS232_Read( void )
{
	while(!(SCISR1 & SCISR1_RDRF_MASK));
	return(SCIDRL);
}


