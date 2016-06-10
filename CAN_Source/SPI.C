#include "SPI.h"

#define E_CLOCK 16000000                
/*
PG 413
 
 BEST SOURCE  PG 388 - VALVANO 
*/
char *SPI_INIT_ERROR[2] = {
  "Desired Clock Larger than E_Clock",
  "Divisor Error, Desired clock too low",
} ;

// read previous reg values// 
// set ssoe and modfen to 0  refrence page 418
unsigned char SPI_Toggle_MasterSS(unsigned char status){
  unsigned char temp_reg =0;
  
  
  ///TOGGLED ON 
  if(status >= 1){ 
   SETBIT(SPI_CONTROL_REG_1,SPICR1_SSOE_MASK); 
    SETBIT(SPI_CONTROL_REG_2, SPICR2_MODFEN_MASK);
  }
  
  //GPIO
  else{
    CLEARBIT(SPI_CONTROL_REG_1,SPICR1_SSOE_MASK); 
    CLEARBIT(SPI_CONTROL_REG_2, SPICR2_MODFEN_MASK);
  }

}



unsigned char SPI_INIT(unsigned long desired_clock){
   // limited to lowest clock being eclock /256 
    unsigned char SPI_SPPR =0, SPI_SPR=0, SPI_DIVISOR=0;
    unsigned char x= 0; 
    unsigned long clock =0; 
    
   // Set DDR // NOT CONTROLLING CS 
  // SPI_DDR_INIT() 
    
   // SPI CONTROL REGS
   SPI_CONTROL_REG_1 = (SPICR1_SPE_MASK | SPICR1_MSTR_MASK); //0x50;   
    /*     PG417
    SPIE    = INTERRUPT ENABLE = 0 /disable
    SPE     = SPI ENABLE =  1 /enable
    SPTIE   = TRANSMIT INTERRUPT = 0 /disable
    MSTR    = SPI = MASTER  = 1 / SET MASTER
    CPOL    = ACTIVE LOW = 0
    CPHA    = SAMP AT ODD = 0
    SSOE    = SLAVE SELECT OUTPUT ENABLE = 1 // enable
    LSBFE   =  LSB First enable = 0 // MSB FIRST    
    */
   SPI_CONTROL_REG_2 = SPICR2_MODFEN_MASK ;    // read this reg for spi ss
    /*   PG417
    MODFEN  = // Mode fault = 1 // enable spi SS 
    BIDIROE = // Disable buffer = 0 
    SPISWAI = // SPI normal in wait =0 // no power save
    SPC0    = // not necessary // no bidirectional pin config necessary
    */
    
    // most significant bit first, clock low on idle, no interrupt and spi master
    // SPICR1_SSOE_MASK  = slave select output//
    //SPI_CONTROL_REG_2 = (SPICR2_MODFEN | SPICR2_BIDIROE); 
    //SPICR2_MODFEN -- 1 = SS op 
    //SPICR2_BIDIROE  -- buffer enabled 
    
    
    ////baudrate  = BUSCLOCK / BaudrateDivisor  
    /// Baudratedivisor = (SPPR + 1) *2 ^(SPR +1) 
    //SPI_BAUDRATE_REG = 0x00; // clock at 4Mhz 
    SPI_DIVISOR = (E_CLOCK / desired_clock)/2;
   
    if(SPI_DIVISOR <= 0){
      SPI_BAUDRATE_REG  = 0x00; 
      return 1; ///  error     
    }
    
    else{
       while(SPI_DIVISOR > 0){
          //  printf(" \n %d    x: %d", SPI_DIVISOR, x);
            SPI_DIVISOR = (SPI_DIVISOR/2);
            x++;
           
            if(x > 7){
              SPI_BAUDRATE_REG  = 0x07; 
              return 2; 
            }
              
       }
          SPI_BAUDRATE_REG = (x-1);
          printf("\n  Set Clock to approx %lu Hz", desired_clock); 
    }
    
    return 0; 
}


/* works PERFECK */
unsigned char SPI_Send_Char(unsigned char data_out){
   unsigned char temp= 0;

   while(!(SPISR & SPISR_SPTEF_MASK)); 	/* wait until write is permissible */
   SPI_DATA = data_out; 
   // wait for spi clear flag//       
   while(!(SPISR & SPISR_SPIF_MASK  ));
   temp =  SPI_DATA; // clear spi//
   
   return 0; 
}

// Works just fine // use the test routine 
unsigned char SPI_Recieve_Char(unsigned char dummy){
  unsigned char data_in=0;  
  
   
   while(!(SPISR & SPISR_SPTEF_MASK)); 	/* wait until write is permissible */
   SPI_DATA = dummy; 
   // wait for spi clear flag//       
   while(!(SPISR & SPISR_SPIF_MASK  ));
   data_in =  SPI_DATA; // clear spi//
   
     	
                
  return data_in;   
}


void SPI_Master_Transmit_Bytes(char *data, int length){
  int x=0;
  for(x=0; x < length; x++)
    SPI_Send_Char(data[x]);
}
void SPI_Master_Recieve_Bytes(char *data, int length){
  int x=0; 
  for(x=0; x< length; x++)
    data[x]= SPI_Recieve_Char(0xAA);
}


unsigned char SPI_Master_Transmit_Recieve(char cData)
{
     unsigned char data_in=0;  
  
 

   while(!(SPISR & SPISR_SPTEF_MASK)); 	
    SPIDR = cData; 	/* write junk data  */
    /* wait until read data is permissable */ 
      

   
   while(!(SPISR & SPISR_SPIF_MASK)); // wait untill a char has been shifted in ! 
   // critical - timing errors will be had if we dont wait for the SPIF FLAG  	
   	data_in  = SPI_DATA; // 

  return data_in; 
}




