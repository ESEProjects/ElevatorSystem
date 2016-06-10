#include "elevator.h"

#if ELEVATOR==1

////////////////////////////////////////////////// Globals
unsigned char car_height;

////////////////////////////////////////////////// Interface Functions
/////Component Interface Functions




/////CAN Communications Interface Functions

//
void Send_DoorRequest(message *request_message) {
      //PING -> message door address, data, length
    if(CAN_CURRENT_ADDRESS != CAN_MASTER_AD){
     Can_Send(CAN_CURRENT_ADDRESS,ping_slave_send,3);
    }
}

//
void Send_FloorSelectRequest(message *request_message){
      //PING -> message floor
    if(CAN_CURRENT_ADDRESS != CAN_MASTER_AD){
      Can_Send(CAN_CURRENT_ADDRESS,ping_slave_send,3);
    }
}

//
void Interpret_Message(message *response_message){
 //Make sure the message came from the controller node 
     if((response_message->sender_node_id)==CONTROLLER_ID) {
      
          //Determine which sub-component the message is for
          switch(response_message->component_id) {
               
               //The up indication LED
               case FLOOR_IND:
                    switch(response_message->component_instruction){
                         
                         case FLOOR_ONE:
                              //Print to LCD
                         break;
                         
                         case FLOOR_TWO:
                              //Print to LCD
                         break;
                         
                         case FLOOR_THRE:
                              //print to LCD
                         break;
                         
                         default:
                         break;
                    }
               break;
               
               //The down indication LED
               case DOOR_IND:
                    switch(response_message->component_instruction){
                         case DOOR_OPEN:
                              //print to LCD
                         break;
                         
                         case DOOR_CLOSE:
                              //print to LCD
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
}



////////////////////////////////////////////////// Interrupts/Utility Functions





#endif