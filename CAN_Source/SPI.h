#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED
                                            
#include <stdio.h>
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
//#include "Sysclock.h"
#include "Macros.h" 


  // master mode 
#define   SPI_CONTROL_REG_1    SPICR1   //RW
#define   SPI_CONTROL_REG_2    SPICR2   //RW
#define   SPI_BAUDRATE_REG     SPIBR    //RW
#define   SPI_STATUS_REG       SPISR   //R

// RW reg
#define  SPI_DATA             SPIDR

#define SPI_MISO        PTM_PTM2_MASK
#define SPI_SS          PTM_PTM3_MASK
#define SPI_MOSI        PTM_PTM4_MASK
#define SPI_SCK         PTM_PTM5_MASK

#define SPI_DDR         DDRM
#define SPI_PORT        PTM 


#define SPI_DDR_INIT()     {          \
  SETBIT(SPI_DDR, (SPI_MOSI | SPI_SCK )); \
  CLEARBIT(SPI_DDR, (SPI_MISO));   }\

unsigned char SPI_Toggle_MasterSS(unsigned char status); // 1 = enabled // 0 = disabled 
unsigned char SPI_INIT(unsigned long desired_clock);
unsigned char SPI_Send_Char(unsigned char);


unsigned char SPI_Recieve_Char(unsigned char dummy);
unsigned char SPI_Master_Transmit_Recieve(char cData);
void SPI_Master_Transmit_Bytes(char *data, int length);

#endif /* SWITCHES_ */


