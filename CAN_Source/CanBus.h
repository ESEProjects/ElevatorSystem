#ifndef _CANBUS_H_INCLUDED 
#define _CANBUS_H_INCLUDED


#include <stdio.h>
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "board_defines.h"        
   


/*******************************************/
// Processor Modules 
/*******************************************/
#include "CanBus.h" // canbus modules 
#include "SPI.h"     // works great
#include "RS232.h"  // null modem works // needs impr
#include "Analog.h" // 10 - 8 bit  0 = 10  8
#include "Timer.h"  // timer and delay


/*******************************************/
// Macros N stuffs
/*******************************************/    
#include "macros.h"

/*******************************************/
// LCD Libraries
/*******************************************/
#include "ILI9327.h" // 
#include "LCD_Draw.h"    // 
#include "LCD_Config.h" // VERY IMPORTANT CONTROLS ALL LCD RELATED LIBRARIES //
#include "Terminal.h" // printf 
#include "Touch.h"

// hitachi
#include "BASEutils.h"
#include "TIMEutils.h"
#include "LCD.h"

/*******************************************/
// Board Specific Desings 
/*******************************************/
#include "board_defines.h" 
#include "callbox.h"


/*******************************************/
// Interrupt Variables 
/******************************************/
#include "interrupts.h" 

#include "processes.h"

/* Acceptance Code Definitions */
#define ACC_CODE_ID100 0x2000
#define ACC_CODE_ID100_HIGH ((ACC_CODE_ID100@0xFF00)>>8)
#define ACC_CODE_ID100_LOW (ACC_CODE_ID100@0x00FF)
/* Mask Code Definitions */
#define MASK_CODE_ST_ID 0x0007
#define MASK_CODE_ST_ID_HIGH ((MASK_CODE_ST_ID&0xFF00)>>8)
#define MASK_CODE_ST_ID_LOW (MASK_CODE_ST_ID@0xFF)
           
   


#define CAN_MASTER_AD        0x0000
#define CAN_FLOOR_1_AD       0x0001
#define CAN_FLOOR_2_AD       0x0002
#define CAN_FLOOR_3_AD       0x0003
#define CAN_FLOOR_4_AD       0x0004

#define CAN_ELEV_CONT_AD     0x0004
 
  
            
#define MSCAN_INIT    0x01   /// Set --- INITRQ high            

#define ERR_BUFFER_FULL   2
#define CAN_RRQUEUE_SIZE  5
#define CAN_NOT_PROCESSED 1
#define CAN_PROCESSED     0xFF 

#define CAN_PROCESS_STRUCT 1


extern unsigned char elevator_keypad_press;
extern unsigned char elevator_keypad_trigger; 




/// KEYPAD AND ANALOG
volatile extern unsigned char analog_delay_counter;
volatile extern unsigned int analog_previous; 

volatile extern unsigned char keypad_press;
volatile extern unsigned char keypad_press_trigger;

extern volatile unsigned char elevator_button_trigger;

/// approx 30 bytes 

extern struct CAN_Structure{
	unsigned long int Identifier;
	unsigned char Data_Length;
	unsigned char Data[8];
	unsigned int Time_Stamp;
	unsigned char Error; 
	unsigned char Processed; 

}CAN_Struct;


//150 bytes 
extern struct CAN_Structure CAN_RRQUEUE[CAN_RRQUEUE_SIZE];


extern unsigned char CANBUS_ADDRESS_DEVICES[6][4];
extern unsigned int Can_Approved_Addresses[8];

extern volatile unsigned char ping_rmr;

void Can_Initalize(void);
char Can_Send(unsigned long address, char *txdata, int len); 
void Can_Send_Address(unsigned long address);
void Can_Setup_Recieve(char *filter);
void CAN_Print_Structure(struct CAN_Structure Current_Can);
void CAN_Process_Structure(struct CAN_Structure Current_Can);


void CAN_Process_Ping(unsigned char senderid, unsigned char compid, unsigned char compinst); 


void CAN_Process_Ping_Master(unsigned char senderid, unsigned char compid, unsigned char compinst);
void CAN_Process_Ping_Slave(unsigned char senderid, unsigned char compid, unsigned char compinst);


void CAN_Process_Floor_Master(unsigned char senderid, unsigned char compid, unsigned char compinst);
void CAN_Floor_Slave(unsigned char senderid, unsigned char compid, unsigned char compinst);

#endif    




//#define sheeet
/*
  0x0000    Ident reg 0
  0x0001    Ident reg 1
  0x0002    Ident reg 2
  0x0003    Ident reg 3
  
  0x0004    Data Seg Reg 0 
  0x0005    Data Seg Reg 1 
  0x0006    Data Seg Reg 2 
  0x0007    Data Seg Reg 3 
  0x0008    Data Seg Reg 4 
  0x0009    Data Seg Reg 5 
  0x000A    Data Seg Reg 6 
  0x000B    Data Seg Reg 7 

  0x000C    Data Length Register 
  0x000D    Transmit Buffer Reg
  0x000E    Time Stamp High Byte
  0x000F    Time Stamp Low Byte 
*/


        