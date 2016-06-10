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

/*
ELEVATOR CONTROLLER 
R: Emergency Stop
R: door state
R floor select
R firefighter over ride 
R up/down request
s ping 

*/

/*
Call stations 
R: floor posotion 
R: car stop 
S: up/down request
S: ping 

*/

/*
Elevator car

R: Emergency Stop
R: floor position
R: car stop
R: ping 

S : emergency stop
S: door state
S: floor select 
S: firefighter over ride 


0x00  Emerge Stop
0x01  Floor Position 
0x02  Car stop
0x03  Door State 
0x04  Floor Select
0x05  Firefight over ride 
0x06  Up/Down Request 
0x07  PING
0x08  Node offline.

*/


/*-----------------------------------------------------------------------*/
/*  Initalize                                                             */
/*-----------------------------------------------------------------------*/
void Initalize_Timer(void);
void Initalize_Interrupts(void);

/*-----------------------------------------------------------------------*/
/*  Connnect printf with TerminalChar                                    */
/*-----------------------------------------------------------------------*/
TERMIO_PutChar(char ch){

    TerminalChar(ch);	
  return;
}

 unsigned char com_nbr[4] = {
      16,17,20,15
 };


unsigned char LCD_INITALIZE(void);




/*-----------------------------------------------------------------------*/
/*  Routine code                                                          */
/*-----------------------------------------------------------------------*/
void main(void) {
 	
  int counter =0; 
  unsigned char process_ping_local =0; 
  unsigned char ping_rmr_local=0; 
  unsigned char process_structure_flag_local =0; 
  unsigned char process_structure_number_local=0; 
  
  unsigned char keypad_press_local =0;
  unsigned char keypad_press_trigger_local =0;  
  
  
  unsigned char process_call_box_local =0; 
  unsigned char call_box_press_local =0; 
  
  unsigned char process_distance_reading_local=0; 
  
  unsigned long address = 0x1EE;
  
  unsigned char analog_value =0x00; 
  char data[8]= { 0x00, 0xAA, 0xA0, 0x31, 0xFF, 0x11,0x00, 0x21  }; 
  
  unsigned char distance_reading_previous =0;
  unsigned char distance_reading_compare=0;  
  
  unsigned char Elevator_State =0; 
  
    elevator_button_trigger = FALSE;


    /*
        INIT  LCD 
    */

    Initalize_Timer();  
          

    RS232_Init(RS232_BAUD);   // start rs232 
    
    // UNIVERSAL 
    Can_Initalize(); 
     
     /// SET PINS 
     Initalize_Interrupts();
     EnableInterrupts;    /* enable interrupt globally */
    
    /*INIT LCDS -- either ili or hatachi */ 
     LCD_INITALIZE(); 
   
   /// start terminal emulator 
    terminalemulator();    //init the terminal emulator 
   
     Analog_Init(ANALOG_10BIT);
        
        
     /*
        Elevator take in analog from keypress 
    */
     if(CAN_CURRENT_ADDRESS == CAN_ELEV_CONT_AD)
        ANALOG_ENABLE_PIN(KEYPAD_INPUT);
     
     /*
        MASTER - Simulate distance Sensor  
    */
     if(CAN_CURRENT_ADDRESS== CAN_MASTER_AD)
       ANALOG_ENABLE_PIN(DISTANCE_SENSOR_INPUT);

 
  for(;;) {
    _FEED_COP(); /* feeds the dog */
   
  

  //  Can_Send(address,data,7);
  
    
    DisableInterrupts;
      process_ping_local = ping_process; 
      process_structure_flag_local = process_structure_flag ; 
      process_structure_number_local=  process_structure_number; 
      /// keypress 
      elevator_keypad_trigger =  keypad_press_trigger;
      elevator_keypad_press = keypad_press; 
      keypad_press_local = keypad_press; 
      keypad_press_trigger_local = keypad_press_trigger;  
      
      /// call box 
      process_call_box_local = process_call_box;  
      call_box_press_local = call_box_press; 
      
      /// MASTER CALL BOX
      process_distance_reading_local = process_distance_reading; 
    EnableInterrupts; 
      
        
      /*
         ALL CAN SYSTEMS PROCESS 
      */
      if(process_structure_flag_local == CAN_PROCESS_STRUCT){
           DisableInterrupts;
             process_structure_flag = FALSE;  
          EnableInterrupts; 
           CAN_Process_Structure(CAN_RRQUEUE[process_structure_number_local]);
        //  
      }

      /*
         NOT ELEVATOR PROCESS
      */
      #if (CAN_CURRENT_ADDRESS != CAN_ELEV_CONT_AD)
          if(keypad_press_trigger_local){
              DisableInterrupts;
               keypad_press_trigger = FALSE;    
              EnableInterrupts;  

              printf("\n BUTTON %d PRESSED", keypad_press_local);   
          }
      #endif  
        
     
      /*
        CALL BOX & ELEVATOR  PROCESSES 
      */   
      #if (CAN_CURRENT_ADDRESS != CAN_MASTER_AD)         
          /// RUN THROUGH THE PING ROUTINE // SLAVES ONLY  
          if((process_ping_local) && ( CAN_CURRENT_ADDRESS != CAN_MASTER_AD)) {
          
            if( CAN_CURRENT_ADDRESS != CAN_MASTER_AD){
              DisableInterrupts;
                 ping_process = FALSE;    
              EnableInterrupts;    
              Can_Send(CAN_MASTER_AD,ping_slave_send,3);
           
            }
          }
      #endif

     
    

      /*
      MASTER ONLY PROCESS
      */   
      #if (CAN_CURRENT_ADDRESS == CAN_MASTER_AD)
        
      if(process_distance_reading_local == TRUE){
          CAN_Process_Distance_Sensor();  
          DisableInterrupts;
             process_distance_reading = FALSE;    
          EnableInterrupts; 
       }
      #endif      


  
      /*
      Elevator ONLY PROCESS
      */   
      #if (CAN_CURRENT_ADDRESS == CAN_ELEV_CONT_AD)
        
      if(keypad_press_trigger_local == TRUE && keypad_press_local >= 4){
         if(keypad_press_local == 4 &&Elevator_State ==0){
          printf("\nelevator open");
          Elevator_State = 1; 
         
         }else if(keypad_press_local == 5 && Elevator_State == 1){
          printf("\n Elevator closing"); 
         Elevator_State =0; 
         }
         
          DisableInterrupts;
            keypad_press_trigger = FALSE;    
          EnableInterrupts; 
       }
      #endif      




      
     
  } /* loop forever */
  /* please make sure that you never leave main */
}


unsigned char LCD_INITALIZE(void){
/*INIT LCDS */ 

  #if LCD_ILI9327
    ILI9327_INITALIZE(); // init lcd     
    LCD_Fill_Rectangle(0,0,TerminalProperties.xResolution,TerminalProperties.yResolution, 0x000000);
  #endif  

  #if LCD_HATACHI
    LCDinit(); 
    printf("HATICH"); 
     if(CAN_CURRENT_ADDRESS == CAN_MASTER_AD)
      LCDprintf("Master Cont\n");
    else
      LCDprintf("Floor %d\n",CAN_CURRENT_ADDRESS);
    
     LCDprintf("COM %d @%d\n",com_nbr[CAN_CURRENT_ADDRESS], 499200);
  #endif


}



void Initalize_Timer(void){
  //Setup Timer timerbase
   // Timer_INIT();       // enable interrupts 
   // EnableInterrupts;    /* enable interrupt globally */
    
    SET_TIMER_PRESCALE(PRESCALE_8);
    DISABLE_TO_INTERRUPT;
    ENABLE_CNTR_FREERUN;
    ENABLE_TSFRZ;
    ENABLE_FAST_CLEAR;
    ENABLE_TIMER;
     
    EnableInterrupts;    /* enable interrupt globally */
    Heartbeat_Init();

}

