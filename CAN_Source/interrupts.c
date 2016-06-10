#include "interrupts.h"


unsigned int keypad_analog_values[9]= {
          3,         //BT1
          135,       //BT2
          269,       //BT3
          464,       //...
          596,
          761,
          867,
          921
          
};



/// resets volitaile variables 
unsigned char Interrupt_Reset_Variables(void){
  DisableInterrupts;
    //// PING Variables
    ping_counter=0;
    ping_process=0; 
    ping_unplugged_flag=0; 

    ///// PROCESS VARs
    process_counter =0; 
    process_structure_flag =0;
    process_structure_number=0;
    /// MASTER DISTANCE SENSOR STUFF
    process_dr_counter=0;
    process_distance_reading=0;
  EnableInterrupts;
  
  return;
  
}



void Initalize_Interrupts(void){
  // INTERRUPT FOR FLOOR AQUISITION 
    TIOS_OUTPUT_PIN(TIOS_IOS5_MASK);
    TIE_ENABLE_PIN(TIE_C5I_MASK);
 
  // INTERRUPT FOR KEYPAD
    TIOS_OUTPUT_PIN(TIOS_IOS4_MASK);
    TIE_ENABLE_PIN(TIE_C4I_MASK);

  // INTERRUPT FOR PING
    TIOS_OUTPUT_PIN(TIOS_IOS3_MASK);
    TIE_ENABLE_PIN(TIE_C3I_MASK);
   
   /// Interrupt for the CAN MESSAGES 
    TIOS_OUTPUT_PIN(TIOS_IOS2_MASK);
    TIE_ENABLE_PIN(TIE_C2I_MASK);
    
     ping_unplugged_flag=0;
     ping_rmr =0; 
     
     /// PROCESS ONLY FOR THE CALL BOXES // THIS IS 
     /// THE CALL BOX INTERRUPTS 
     if(CAN_CURRENT_ADDRESS != CAN_MASTER_AD)
      Init_Components(); 
    
      
   return;

}


//////////////////// CALL BOX ONLY //////////////////// 
/*-----------------------------------------------------------------------*/
/*Interrupt/Utility Functions
/*
/*INTERRUPT (24=Vportj)
/*Handles the button presses of the callboxs
/* by reading what button has been pressed.
/*-----------------------------------------------------------------------*/
interrupt 24 void BtnPress_Handler(void){

     if(CAN_CURRENT_ADDRESS == CAN_MASTER_AD)
      return; 
   
     //Capture what button has been pressed.
     READ_BTN_PRESS(btn_id_flag);
      
     //Switch to see what flag needs to be cleared.
     //This may not need to be here if clearing both is valid.
     /// 
     switch(btn_id_flag) {
     
          case BTN1:
               CLR_J6_INT_FLAG;
               process_call_box = TRUE; 
               call_box_press = BUTTON_UP; 
               elevator_button_trigger= TRUE;
               Set_IndicationLEDs(LED1,LED_OFF); 
               Set_IndicationLEDs(LED2,LED_ON);
                
          break;
          
          case BTN2:
               CLR_J7_INT_FLAG;
               process_call_box = TRUE;
               elevator_button_trigger = TRUE; 
               call_box_press = BUTTON_DOWN;
               Set_IndicationLEDs(LED1,LED_ON); 
               Set_IndicationLEDs(LED2,LED_OFF); 
          break;
          
          default:
               CLR_J6_INT_FLAG;
               CLR_J7_INT_FLAG;
               process_call_box = FALSE; 
          break;
          
     }
}



//////////////////// MASTER ONLY //////////////////////////////////////////
/*-----------------------------------------------------------------------*/
/*  INTERRUPT 13        
/*  ANALOG DISTANCE counter  /// this is a dumb loop used for the 
     analog distance sensor       
                                       */
/*-----------------------------------------------------------------------*/
interrupt 13 void TC5handler(void){
  
  if( process_dr_counter >= PROCESS_DR_DELAY){
    process_distance_reading = TRUE;
    process_dr_counter =0;
  }
      
    process_dr_counter++; 
      
    TFLG1 = TFLG1_C5F_MASK;
      
    TC5 = TCNT + 60000; 
    
}
        

//////////////////////// ELEVATOR ONLY  //////////////////// 
/*----------------------------------------------------------------------*/
/*  INTERRUPT 12             --- KEYPAD ANALOG READ 
/*     - Responsible for reading the input from the keypad --
/*    would use an analog interrupt, however only a conversion interrupt exists
/*                                          */
/*-----------------------------------------------------------------------*/
interrupt 12 void TC4handler(void){
    unsigned char analog_tollerance_ = ((ANALOG_TOLLERANCE*1024)/100);
    unsigned int analog_compare =0; 
    unsigned int analog_current = 0; 
    unsigned char x=0, y=0; 
  
     //Clear the interupt flag
    TFLG1 = TFLG1_C4F_MASK;
   
   
    if(analog_delay_counter == ANALOG_DELAY){
        analog_current = Analog_Read(KEYPAD_INPUT); 
        analog_compare = abs(analog_previous-analog_current); // 
        
        
        if( analog_compare >= analog_tollerance_) {
         /// process keypress
           y = (sizeof(keypad_analog_values))/2; 
           for(x= 0;x<y;x++){
                analog_compare = abs(keypad_analog_values[x]-analog_current);
          
                if(analog_compare < (analog_tollerance_)) {
                  keypad_press = x; 
                    // trigger a button event 
                  keypad_press_trigger = 1;
                  break; 
                }
                 
           }
        
        }
        
  
        
        analog_previous = analog_current; 
      #if ANALOG_DEBUG 
        printf("\nAnalog(%d):  Cur:%d     Prev:%d        ",KEYPAD_INPUT, Analog_Read(KEYPAD_INPUT),analog_previous);
      #endif  
    // reset analog counter   
        analog_delay_counter =0;    
    } 
    
    else 
     analog_delay_counter++; 
       
    TC4 = TCNT + 60000; 
    
}
        


/*-----------------------------------------------------------------------*/
/*  INTERRUPT 11             ---- PING SYSTEM
/*     - Responsible for sending a heart beat signal to the master
/*   From Slave -> Master   
/*                                          */
/*-----------------------------------------------------------------------*/
interrupt 11 void TC3handler(void){
     //Clear the interupt flag
    TFLG1 = TFLG1_C3F_MASK;
    
    ping_counter++; 
           
 
  #if PING_DEBUG 
    printf("%d ",ping_counter);
  #endif 
    /// Roughly 1 second 
    if(ping_counter >= 10 && ping_process ==0){
          ping_process = 1; // time to process 
          ping_counter = 0; 
          ping_unplugged_flag+=1;    
    }
    
    // roughtly 10 seconds //
    if(ping_unplugged_flag >= 5){
          printf("UNPLUGED");         
    }
   
    // if ping recieve master response has been set
    // reset the ping counter// 
    if(ping_rmr){
       ping_counter = 0; 
       ping_process =0; 
       ping_rmr = 0;
       ping_unplugged_flag=0;
    }
   
    TC3 = TCNT + 60000; 
    
}
        
        
/*-----------------------------------------------------------------------*/
/* INTERRUPT 10               ------   PROCESS CAN MESSAGES
/*                             Search through array - find any unprocessed 
/*          #define CAN_NOT_PROCESSED 1
/*        #define CAN_PROCESSED     0xFF 
/*                                          */
/*-----------------------------------------------------------------------*/
interrupt 10 void TC2handler(void){     
     unsigned char curr_struct =0, i, apsize,cp_value = 0x00; 
     unsigned char size_struct = CAN_RRQUEUE_COUNTER; 
      TFLG1 = TFLG1_C2F_MASK;
     
      //CAN_RRQUEUE_COUNTER
      // CAN_RRQUEUE_SIZE
     if( process_counter >= PROCESS_DELAY){
   
          for (i= 0; i < apsize; i++){
             curr_struct = ((CAN_RRQUEUE_COUNTER+i)%CAN_RRQUEUE_SIZE);  
             cp_value =  CAN_RRQUEUE[(curr_struct)].Processed; 
              
         
             if(cp_value == CAN_NOT_PROCESSED){
                 // CAN_Process_Structure
                 process_structure_number = curr_struct; 
                 process_structure_flag = CAN_PROCESS_STRUCT; 
               //  CAN_Process_Structure(CAN_RRQUEUE[curr_struct]); 
                // printf("process me %d", curr_struct); // process_data = 1; 
                  CAN_RRQUEUE[(curr_struct)].Processed = CAN_PROCESSED; 
                 break; 
             }
             
              else {
                 process_structure_flag = 0;
              } 
        } 
         process_counter =0; 
     }
      
      process_counter++;  
   
   
      TC2 = TCNT + 20000; 
}        
                                                 


/*-----------------------------------------------------------------------*/
/*  CAN BUS RX interrupt VECTOR                                          */
/*-----------------------------------------------------------------------*/
interrupt 38 void RxISR(void)
{
       
     unsigned long int final_address = 0x00; 
     unsigned long int temp_address = 0x00;   
     unsigned char address_temp =0;
     unsigned char address_bytes = 0; 
     unsigned int time;
     unsigned char process_data =0;
     
     char *data_in ;
     char *time_high;
     char *identity_in;
     char *time_low;
        
     unsigned char  filter_in =0 , data_length=0, i=0, apsize; 
     char data;   
     
     data_in = (char *)&CANRXDSR0;
    time_low = (char *)&CANRXTSRL;
    time_high = (char *)&CANRXTSRH;
    identity_in = (char *)&CANRXIDR0;     
    
    
      // interrupt not caused by RXF, return  do nothing 
     if (!(CANRFLG & CANRFLG_RXF)) 
        return;
   
       // Filter 7 hit,   Two 32bit Acceptance Filters 
     filter_in= CANIDAC & 0x07;  
     data_length = (CANRXDLR&0x0F); // kill the upper bits 
    
    // No data no process, return 
     if (data_length == 0)    
       return; 
     
     if (filter_in == 0){ // filter 0 hit       

/// Address Bus  /// 
  /// Find 11 bit or 29 bit addresss   .// PAGE 317 NXP 
      address_bytes =  ((CANRXIDR1 & CANRXIDR1_IDE_MASK) | (CANRXIDR1 &   CANRXIDR1_SRR_MASK )); 
     
     /// Process - Extended Bit 
      if(address_bytes == (CANRXIDR1_IDE_MASK |CANRXIDR1_SRR_MASK )) {
            
          final_address += (CANRXIDR0);
          final_address <<= 21; 
                         
          address_temp =   (CANRXIDR1 & 0xE0) |  (((CANRXIDR1) & 0x07) << 2);
               
          temp_address = address_temp; 
          temp_address <<= 13;    
          final_address += temp_address;  

          temp_address =  (CANRXIDR2); 
          temp_address <<= 7;
          final_address += temp_address;  

          temp_address = (CANRXIDR3)&0xFE;
          temp_address >>=1; 
          final_address += temp_address;  
               
      } else {
      // Process Normal Bit
          address_temp  = (*identity_in);
          address_temp  <<=3;
          final_address = address_temp; 
          *identity_in++; 
          address_temp =    (*identity_in) & 0xE0; 
          address_temp >>= 5; 
          final_address += address_temp;  
      }
      
    time = (*time_low);
    time += ((*time_high  & 0xFF) << 8);
    
      
  //STRUCTURE CODE /// 
  /// STORE the data into structures, for use later.
  /*     apsize = sizeof(Can_Approved_Addresses)/sizeof(int); 
      for (i= 0; i < apsize; i++){
           if(Can_Approved_Addresses[i] == (unsigned int)(final_address&0xFFFF))
                process_data = 1; 
      }  */
      if((unsigned int)(final_address&0xFFFF) ==  CAN_CURRENT_ADDRESS){
               process_data = 1;     
      } else 
         process_data = 0; 
  
        if(process_data){ 
            CAN_RRQUEUE[(CAN_RRQUEUE_COUNTER)].Identifier = final_address;
            CAN_RRQUEUE[(CAN_RRQUEUE_COUNTER)].Data_Length = data_length;

            for (i = 0; i < data_length; i++){     
                data = (*data_in++);
                 CAN_RRQUEUE[(CAN_RRQUEUE_COUNTER)].Data[i] = data; 
            }

            CAN_RRQUEUE[(CAN_RRQUEUE_COUNTER)].Time_Stamp = time; 
            CAN_RRQUEUE[(CAN_RRQUEUE_COUNTER)].Error = CANRXERR; 
            CAN_RRQUEUE[(CAN_RRQUEUE_COUNTER)].Processed = CAN_NOT_PROCESSED;

            //CAN_Print_Structure(CAN_RRQUEUE[(CAN_RRQUEUE_COUNTER)]); 
              
              
            if(CAN_RRQUEUE_COUNTER < (CAN_RRQUEUE_SIZE-1))
              CAN_RRQUEUE_COUNTER++;
            else 
              CAN_RRQUEUE_COUNTER = 0; 
             // clear that flag and return   
               
             
           } 
           
        CANRFLG |= CANRFLG_RXF; 
   
        return;         
     }
      else return;        // other hit, do nothing  

}

