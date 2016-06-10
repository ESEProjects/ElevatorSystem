/*
 * Interrupts.h
 *
 */ 


#ifndef INTERRUPTS_H_INCLUDED_
#define INTERRUPTS_H_INCLUDED_

#include <stdio.h>
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */



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
// Board Specific Desings 
/*******************************************/
#include "board_defines.h" 
#include "callbox.h"



     
extern volatile unsigned char CAN_RRQUEUE_COUNTER; 

//// PING Variables
extern volatile unsigned int ping_counter; 
extern volatile unsigned char ping_process; 
extern volatile unsigned char ping_unplugged_flag; 

///// PROCESS VARs
extern volatile unsigned int  process_counter; 
extern volatile unsigned char process_structure_flag; 
extern volatile unsigned char process_structure_number; 


#define ANALOG_TOLLERANCE  3 /// percent // assume 10 bits   --- at 2percvent approx 20
#define ANALOG_DELAY   3 /// keypad delay in 10ms x value 



/// MASTER DISTANCE SENSOR STUFF
#define DISTANCE_SENSOR_INPUT 0x01 
extern volatile unsigned char process_distance_reading; 
extern volatile unsigned char process_dr_counter;

#define PROCESS_DR_DELAY  5


/// KEYPAD

extern unsigned int keypad_analog_values[9];
#define PROCESS_DELAY     2 
#define KEYPAD_INPUT      0x06   //ANALOG 6






/// resets volitaile variables 
unsigned char Interrupt_Reset_Variables(void);
void Initalize_Interrupts(void);

#endif /* INTERRUPTS_H_ */