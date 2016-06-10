#ifndef _PROCESSES_H_INCLUDED 
#define _PROCESSES_H_INCLUDED


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


/// PING VARIABLES 
extern char ping_slave_send[3]; 
extern char ping_master_respond[3];
extern unsigned char distance_reading_previous;

extern unsigned char Floor_AQ_Previous_Floor; 

#define FLOOR_ACQUISITION_PROCESS   0xFA


void CAN_Process_Ping(unsigned char senderid, unsigned char compid, unsigned char compinst);       
void CAN_Process_FloorAQ(unsigned char senderid, unsigned char compid, unsigned char compinst);
void CAN_Process_CallBox(void);
void CAN_Process_Distance_Sensor(void); 

#endif 