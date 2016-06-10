/******************************************************************************
Author:        Joshua Findlay

Date:          May 24, 2016

Description:   This file contains the function prototypes, global variables,
               definitions and Macros for the CALLBOX CAN node for the
               semester 6 project.
******************************************************************************/

#ifndef CALLBOX_H_INCLUDED_
#define CALLBOX_H_INCLUDED_


#include <hidef.h>            /* common defines and macros */
#include "derivative.h"       /* derivative-specific definitions */
#include "BASEutils.h"

#include "CanBus.h"
#include "board_defines.h"



/* Macros */

/****************************** Components ******************************/
/////LED Component
//Init
#define LED_PORT              PTS
#define LED_DDR               DDRS


#define LED1  1
#define LED2  2

#define LED_DIR_OUT           0xFF
#define LED_DIR_IN            0x00

//Interface
#define LED_ON                (1)
#define LED_OFF               (0)

/////Button Component
//Init
#define BTN_PORT              PTJ
#define BTN_DDR               DDRJ

#define BTN_DIR_IN            0x00
#define BTN_DIR_OUT           0xFF

#define PTJ6_INTERRUPT_ENABLE           SET_BITS(PIEJ,PIEJ_PIEJ6_MASK)
#define PTJ6_INTERRUPT_DISABLE          CLR_BITS(PIEJ,PIEJ_PIEJ6_MASK)
#define CLR_J6_INT_FLAG                 CLR_BITS(PIFJ,PIFJ_PIFJ6_MASK)

#define PTJ7_INTERRUPT_ENABLE           SET_BITS(PIEJ,PIEJ_PIEJ7_MASK)
#define PTJ7_INTERRUPT_DISABLE          CLR_BITS(PIEJ,PIEJ_PIEJ7_MASK)
#define CLR_J7_INT_FLAG                 CLR_BITS(PIFJ,PIFJ_PIFJ7_MASK)

#define PTJ_FULL_STRENGTH               SET_BITS(RDRJ,(RDRJ_RDRJ6_MASK&RDRJ_RDRJ7_MASK));
#define PTJ_THIRD_STRENGTH              CLR_BITS(RDRJ,(RDRJ_RDRJ6_MASK&RDRJ_RDRJ7_MASK));

//Interface
#define READ_BTN_PRESS(x)          (x=BTN_PORT)
#define BTN1                       PTJ_PTJ6_MASK
#define BTN2                       PTJ_PTJ7_MASK


/* Globals */
#define BUTTON_UP             0x02
#define BUTTON_DOWN           0x01



//// CALL BOX VAR
extern volatile unsigned char car_height;
extern volatile int btn_id_flag;
extern volatile char bpFlag; 
extern volatile unsigned char process_call_box; 
extern volatile unsigned char call_box_press;



/* Function Prototypes */

//Componenet Interface Functions
void Init_Components(void);
unsigned char Button_Read(void); 
void Set_IndicationLEDs(char led1_state, char led2_state);

//CAN Communication Functions
void Send_CallRequest(struct CAN_Structure Current_Can);
void Interpret_Message(struct CAN_Structure Current_Can);

#endif

