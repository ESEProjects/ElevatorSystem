#ifndef RS232_H_INCLUDED 
#define RS232_H_INCLUDED  
                             
#include <stdio.h>
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define RS232_MESSAGE_STORAGE_LENGTH 40
#define E_CLOCK    8000000UL
#define RS232_IOREGISTER    SCIDRL 
#define RS232_BAUDLB_REG    SCIBDL
#define RS232_BAUDHB_REG    SCIBDH


#define RS232_CONTROL_REG   SCICR1
#define RS232_CONTROL_REG2  SCICR2

#define RS232_STATUS_REG    SCISR1

#define RS232_STATUS_N81    0x00

/// SCICR2 
/// no declare
#define RS232_TIE   // Transmit Interrupt enable bit
#define RS232_TCIE  // Transmit complete interrupt enable bit
#define RS232_RIE  //  Reciever full interrupt enable bit
#define RS232_ILIE  // Idle line interrupt enable bit 
#define RS232_TE    //  Transmitter enable bit 
#define RS232_RE    //  reciever enable 
#define RS232_RWU   //
#define RS232_SBK   //



//
extern volatile int RS232_COUNTER_OUTSIDE; 
//typedef struct RS232_Message			RS232_Message_Node;

//RS232_Message_Node  *Current_Message_Node;

unsigned char RS232_Init(unsigned long baudRate);
char RS232_Write(unsigned char data);
char RS232_Write_String(unsigned char *data);
unsigned char RS232_Read( void );

unsigned char *Process_RS232(unsigned char *RS232_INPUT, unsigned char Length); 

extern unsigned long RS232_BAUD_RATE; 

extern volatile int data_in , data_2; 



extern volatile unsigned char Error_Flag_Number; 

// INTERUPT VARIABLE 
extern unsigned char RS232_Data_Input[10];
extern unsigned char RS232_Counter;  /// 0 
extern unsigned char RS232_Recieve_Flag;
 
extern char   RS232_Com_PORT;
extern char   Board_Function; 
extern char	 MessageSequenceNumber;  //  Place in packet   in a message package of 430 bytes( 2 - 140- 279)
extern char	 lDataLength;     // size of message sent after the heade

// both need to be volaitle // they are used outside of interrupt routine 
extern volatile unsigned char 	MessageStorageArray[RS232_MESSAGE_STORAGE_LENGTH]; // leave room for terminating chars
extern volatile unsigned char   Message_Storage_Counter; 

extern volatile char   RS232_Processed_Data_Flag; 
extern unsigned char   RS232_Transmission_Flag;


extern volatile unsigned char FUCKER; 
extern unsigned char RS232_Counter_; 


#endif    `
