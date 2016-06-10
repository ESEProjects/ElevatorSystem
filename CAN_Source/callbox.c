#include "callbox.h"



/* Utility Functions */



void Send_CallRequest(struct CAN_Structure Current_Can) {
      
      //PING -> message ???? ?
      //???????? ???
   // if(CAN_CURRENT_ADDRESS != CAN_MASTER_AD)   
     //Can_Send(CAN_CURRENT_ADDRESS,ping_slave_send,3);
}
/*
/// approx 30 bytes 

extern struct CAN_Structure{
	unsigned long int Identifier;
	unsigned char Data_Length;
	unsigned char Data[8];
	unsigned int Time_Stamp;
	unsigned char Error; 
	unsigned char Processed; 

}CAN_Struct;

void CAN_Process_Ping_Master(unsigned char senderid, unsigned char compid, unsigned char compinst){

*/

void Interpret_Message(struct CAN_Structure Current_Can){
unsigned char i, data_length, data;
unsigned char process,senderid,recieveid,compinst;

  data_length = Current_Can.Data_Length; 
  if(data_length > 7)
     data_length = 7; 
  
  recieveid = (unsigned char)Current_Can.Identifier;
  senderid = Current_Can.Data[0];
  process = Current_Can.Data[1];
  compinst = Current_Can.Data[2];
/*

      //Make sure the message came from the controller node 
     if((senderid)== CAN_MASTER_AD) {
      
          //Determine which sub-component the message is for
          switch(response_message->component_id) {
               
               //The up indication LED
               case IND_LED1:
                    switch(response_message->component_instruction){
                         
                         case LED_ON:
                              Set_IndicationLEDs(1,LED_ON);
                         break;
                         
                         case LED_OFF:
                              Set_IndicationLEDs(1, LED_OFF);
                         break;
                         
                         default:
                         break;
                    }
               break;
               
               //The down indication LED
               case IND_LED2:
                    switch(response_message->component_instruction){
                         case LED_ON:
                              Set_IndicationLEDs(2, LED_ON);
                         break;
                         
                         case LED_OFF:
                              Set_IndicationLEDs(2, LED_OFF);
                         break;
                         
                         default:
                         break;
                    }
               break;
               
               //The elevator car information to be displayed on the LCD
               case CAR_INFO:
                    car_height=response_message->component_instruction;
               break;
               
               //Don't know what to do if nothing fits
               default:
               break;
               
          }//Component ID check
     }//Controller ID check
     */ 
}





/////////////////////////////////////
////////////////////////////////////





////////////////////////////////////////////////// Interface Functions

//Initialize the ports for controlling 
//the indication LEDs and reading the button presses.
void Init_Components(void) {
     
     //Configure LEDs on port S
     LED_DDR = LED_DIR_OUT;
     
     //Configure buttons on port J
     BTN_DDR=BTN_DIR_IN;
     
     PTJ6_INTERRUPT_ENABLE;
     PTJ7_INTERRUPT_ENABLE;
     
     PTJ_FULL_STRENGTH;
}


//Report which button has beed pressed
unsigned char Button_Read(void){
     
     //Temperary variable to pass button press
     unsigned char btn_id;
     
     btn_id=btn_id_flag;
     
     //Set flag to zero so duplicate presses events
     //don't get passed.
     btn_id_flag=0x00;
     
     return(btn_id);
}


//Set the output pattern of the indication LED 
//to the passed in state.
void Set_IndicationLEDs(char led_id, char led_state){
     if(led_state){
          SET_BITS(LED_PORT,(PTS_PTS1_MASK<<led_id));
     } else{
          CLR_BITS(LED_PORT,(PTS_PTS1_MASK<<led_id));
     }
}




