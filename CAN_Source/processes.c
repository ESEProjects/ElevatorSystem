#include "processes.h"



/// ELEVATOR LOCATION 
unsigned char ELEVATOR_ANALOG[4] = {
      0x00,
      ELEVATOR_FLOOR_1,
      ELEVATOR_FLOOR_2,
      ELEVATOR_FLOOR_3,
};





char ping_slave_send[3]= { (unsigned char)(CAN_CURRENT_ADDRESS), 0xDE, 0xAD }; 
char ping_master_respond[3]= { (unsigned char)(CAN_MASTER_AD), 0xBE, 0xE5 }; 






 #if (CAN_CURRENT_ADDRESS != CAN_MASTER_AD)
   
void CAN_Process_CallBox(void){
    unsigned char data[3]; 
    unsigned char  call_box_press_local; 
     /// PROCESS THE CALL BOX BUTTONS // only if call box ignore for master 
     if(CAN_CURRENT_ADDRESS != CAN_MASTER_AD){
          DisableInterrupts;
             process_call_box = FALSE;  
             call_box_press_local = call_box_press; 
          EnableInterrupts;
         
          data[0] = CAN_CURRENT_ADDRESS;   
          data[1] = 0xFA;
          data[2] = call_box_press_local; 
          
          Can_Send(CAN_MASTER_AD,data,3);
     
     }
}
             
#endif      
    
 /*
   MASTER ONLY PROCESSES 
 
 */
#if (CAN_CURRENT_ADDRESS == CAN_MASTER_AD)

  void CAN_Process_Distance_Sensor(void){
       unsigned char analog_value;
       unsigned char  distance_reading_compare;  
          if( CAN_CURRENT_ADDRESS == CAN_MASTER_AD){
              DisableInterrupts;
                 process_distance_reading = FALSE;    
              EnableInterrupts;  

              analog_value = ((Analog_Read(DISTANCE_SENSOR_INPUT )) >> 2);   
             
             distance_reading_compare = abs(analog_value- distance_reading_previous);
              printf("\nAnalog(%d):  Cur:%d      ",DISTANCE_SENSOR_INPUT, analog_value);
             if( distance_reading_compare > 2){
              
                 printf("\nAnalog(%d):  Cur:%d      ",DISTANCE_SENSOR_INPUT, analog_value);

                   CAN_Process_FloorAQ((unsigned char)CAN_FLOOR_1_AD,0xFA,(unsigned char)analog_value);
                   CAN_Process_FloorAQ((unsigned char)CAN_FLOOR_2_AD,0xFA,(unsigned char)analog_value);
                   CAN_Process_FloorAQ((unsigned char)CAN_FLOOR_3_AD,0xFA,(unsigned char)analog_value);
                   distance_reading_previous= analog_value; 
             }
           }
  }
         
         
#endif    
    
/*-----------------------------------------------------------------------*/
/*   PROCESS FLOOR AQUISITION             
/*  
/*                                                                       
/*-----------------------------------------------------------------------*/
/// 
/// SLAVE
#if (CAN_CURRENT_ADDRESS != CAN_MASTER_AD && CAN_CURRENT_ADDRESS != CAN_ELEV_CONT_AD)

  void CAN_Process_FloorAQ(unsigned char senderid, unsigned char compid, unsigned char compinst){
     unsigned char analog_in; 
     unsigned char tollerance = 10;
     int comp=0;
     unsigned char data[3];
     unsigned char Floor_AQ_Current=0; 
     unsigned char  process_call_box_local; 
    // compare analog in with the known elevator address 
     DisableInterrupts;
        process_call_box_local =  process_call_box; 
     EnableInterrupts;
      
      for(Floor_AQ_Current=1; Floor_AQ_Current  < 4; Floor_AQ_Current++){
            
        comp = abs( ELEVATOR_ANALOG[Floor_AQ_Current] - compinst);
                 
       #if  PRINTF_DEBUG_FLOORAQ
          printf("\n Comp  %d in %d ", comp, compinst); 
       #endif
          
          
         //// if compare is less than value, it is at the current floor 
          if(comp < 20){
          
            Floor_Elevator_LCD(Floor_AQ_Current);
                           
              #if  PRINTF_DEBUG_FLOORAQ
                 printf("\n AT FLOOR %d", Floor_AQ_Current );
              #endif
           /// turn off leds if the counter value 
           /// and send out the packet 
            if((Floor_AQ_Current == CAN_CURRENT_ADDRESS) && (process_call_box_local == TRUE)) { 
              Set_IndicationLEDs(LED1,LED_OFF); 
              Set_IndicationLEDs(LED2,LED_OFF); 
              
              DisableInterrupts;
                   process_call_box= FALSE;
              EnableInterrupts;
      
  
            
             /// Send out the data to the elevator controller. 
              data[0] = CAN_CURRENT_ADDRESS;   
              data[1] = FLOOR_ACQUISITION_PROCESS;
              data[2] = ELEVATOR_ANALOG[CAN_CURRENT_ADDRESS];   
              
              #if  PRINTF_DEBUG_FLOORAQ
                    printf("C%d F%d ",CAN_CURRENT_ADDRESS, Floor_AQ_Current ); 
              #endif
               /// verify we are not triggering the same event 
             if(Floor_AQ_Previous_Floor != Floor_AQ_Current) 
                Can_Send(CAN_ELEV_CONT_AD,data,3); 
            
            }
            
          
            Floor_AQ_Previous_Floor = Floor_AQ_Current;
            return; 
          } 
            
         

      
      }
    /// RESET PING 
         EnableInterrupts;
          ping_rmr = 1;
         DisableInterrupts;
    
      return;
  } 
  
//ELEVATOR
#elif (CAN_CURRENT_ADDRESS == CAN_ELEV_CONT_AD)
  void CAN_Process_FloorAQ(unsigned char senderid, unsigned char compid, unsigned char compinst){
     unsigned char data[3]; 
     unsigned char  keypad_press_trigger_local;
     unsigned char keypad_press_local=0; 
     unsigned char wait=0; 
     
     //filters floors above 3// 
     if(senderid >= 4)
     return; 
     
        printf("\n OPEN DOOR SEQUENCE"); 
        printf("\nArrived On Floor %d \n please select a floor --------------------", senderid);
        
         DisableInterrupts;
             keypad_press_trigger_local =keypad_press_trigger;     
          EnableInterrupts; 
        while(!keypad_press_trigger_local){
        
         DisableInterrupts;
             keypad_press_trigger_local =keypad_press_trigger;     
          EnableInterrupts; 
       
          msDelay(100);
          wait++;
          if(wait > 200){ 
             printf("MAKE A SELECTION ALREADY"); 
             wait =0;  
          }
         
        
        }
           DisableInterrupts;
            keypad_press_trigger = FALSE;     
            keypad_press_local =keypad_press;    
           EnableInterrupts;
          
         printf("\n Floor %d selected",keypad_press_local ); 
         printf("\n CLOSE DOOR SEQUENCE"); 
     /// SEND TO MASTER CURRENT selection 
         data[0] = CAN_CURRENT_ADDRESS;
         data[1] = FLOOR_ACQUISITION_PROCESS; 
         data[2] = keypad_press_local; 
         Can_Send(CAN_MASTER_AD,data,3);
  }
 
///
#elif (CAN_CURRENT_ADDRESS == CAN_MASTER_AD)
 void CAN_Process_FloorAQ(unsigned char senderid, unsigned char compid, unsigned char compinst){
     unsigned char data[3];
    
    
    if(senderid != CAN_ELEV_CONT_AD){
      data[0] = CAN_CURRENT_ADDRESS;   
      data[1] = compid;
      data[2] = compinst;     
    Can_Send(senderid,data,3);
    }
    else if(compinst >0 && compinst < 4) {   
     
      printf(" \n Floor %d selected from call box, running motor now \n",compinst); 
    
    }
    
 }


#endif





/*-----------------------------------------------------------------------*/
/*   PROCESS PING              
/*      -interrupt driven, the slave will send a massage DEAD
/*         the master will respond with BEES
/*      -if the slave doesnt recieve the responce within 5 seconds 
/*        the slave will flag itself as unplugged 
/*      - the master does nothing special. 
/*                                                                       
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/*  MASTER PROCESS PING                                                  */
/*-----------------------------------------------------------------------*/
#if (CAN_CURRENT_ADDRESS == CAN_MASTER_AD)

  void CAN_Process_Ping(unsigned char senderid, unsigned char compid, unsigned char compinst){
     
      Can_Send(senderid,ping_master_respond,3);
      
      #if  PRINTF_DEBUG_PING
         printf("\n Send Ping to Slave %d", senderid); 
      #endif 
      
    
         EnableInterrupts;
          ping_rmr = 1;
         DisableInterrupts;

    return; 
  }

      
       
/*-----------------------------------------------------------------------*/
/*  SLAVE PROCESS PING                                                   */
/*-----------------------------------------------------------------------*/
#else if 
 void CAN_Process_Ping(unsigned char senderid, unsigned char compid, unsigned char compinst){   
      EnableInterrupts;
        ping_rmr = 1;
       DisableInterrupts;
            
       #if  PRINTF_DEBUG_PING
          printf("\n Ping %d Recieve from master", (unsigned char)compinst);
       #endif
  return; 
}
#endif 




/*-----------------------------------------------------------------------*/
/*  CAN_Process_Structure                                                */
/*-----------------------------------------------------------------------*/
void CAN_Process_Structure(struct CAN_Structure Current_Can){
unsigned char i, data_length, data;
unsigned char process,senderid,recieveid,compinst;
unsigned char keypad_press_trigger_local =0;
unsigned char keypad_press_local =0; 

unsigned char elevator_button_trigger_local; 

  data_length = Current_Can.Data_Length;  
  if(data_length > 7)
     data_length = 7; 
  
  recieveid = (unsigned char)Current_Can.Identifier;
  senderid = Current_Can.Data[0];
  process = Current_Can.Data[1];
  compinst = Current_Can.Data[2];
  //  printf(" 0x%x  0x%x   0x%x ", senderid, process, compinst); 
  switch(process){
    case 0x00:
      printf("do elevator?");
      break; 
    case 0x01:
      printf("lol wut");
      break;
    case 0xDE:
      /// nothing ?
    case 0xBE:
        if(recieveid == CAN_CURRENT_ADDRESS) 
           compinst = recieveid;  
        CAN_Process_Ping(senderid,process,compinst);
      break;
      
    case FLOOR_ACQUISITION_PROCESS:
       CAN_Process_FloorAQ(senderid,process,compinst);      
     break;
 
  }
   
    Current_Can.Processed = CAN_PROCESSED; 
  return; 
}


/*
#if (PRINTF_DEBUG_FLOORAQ)

#endif */

