/*
 * Board_Defines.h
 */ 


#ifndef BOARD_DEFINES_H_INCLUDED
#define BOARD_DEFINES_H_INCLUDED


#define CAN_CURRENT_ADDRESS  CAN_ELEV_CONT_AD




//       REFRENCE VALUES 
/// MANY ARRAYS ARE RELIENT ON THESE VALUES 
#define CAN_MASTER_AD        0x0000
#define CAN_FLOOR_1_AD       0x0001
#define CAN_FLOOR_2_AD       0x0002
#define CAN_FLOOR_3_AD       0x0003

#define CAN_ELEV_CONT_AD     0x0004

#define ELEVATOR_FLOOR_1     0x02
#define ELEVATOR_FLOOR_2     0x80
#define ELEVATOR_FLOOR_3     0xF0







     
#define RS232_BAUD      499200

/*-----------------------------------------------------------------------*/
/*  ELEVATOR CONDITIONS                                                  */
/*-----------------------------------------------------------------------*/
#if (CAN_CURRENT_ADDRESS == CAN_ELEV_CONT_AD)

    #define LCD_HATACHI 0
    #define LCD_ILI9327 1
    #define LCD_ILI9327_PRINTF 1
    #define RS232_PRINTF    0
    
        //What node is it
    #define CALLBOX     0  // lcd thiny do module 
    #define ELEVATOR    1
    #define LAN_CAN     0
 
#endif

/*-----------------------------------------------------------------------*/
/*  ELEVATOR CONDITIONS                                                  */
/*-----------------------------------------------------------------------*/
#if (CAN_CURRENT_ADDRESS == CAN_FLOOR_1_AD || CAN_CURRENT_ADDRESS == CAN_FLOOR_2_AD || CAN_CURRENT_ADDRESS == CAN_FLOOR_3_AD)

    #define LCD_HATACHI 1
    #define LCD_ILI9327 0
    #define LCD_ILI9327_PRINTF 0
    #define RS232_PRINTF    1
    
      //What node is it
  #define CALLBOX     1  // lcd thiny do module 
  #define ELEVATOR    0
  #define LAN_CAN     0
 
#endif        
       
/*-----------------------------------------------------------------------*/
/*  ELEVATOR CONDITIONS                                                  */
/*-----------------------------------------------------------------------*/
#if (CAN_CURRENT_ADDRESS == CAN_MASTER_AD)
    #define LCD_HATACHI 1
    #define LCD_ILI9327 0
    #define LCD_ILI9327_PRINTF 0
    #define RS232_PRINTF    1
    
      //What node is it
   #define MASTER_CON     1
   #define CALLBOX        0  // lcd thiny do module 
   #define ELEVATOR       0
   #define LAN_CAN        0
#endif       
       
      
      


/*
DEBUG DEFINES !!!
*/

#define PRINTF_DEBUG_PING      0 
#define PRINTF_DEBUG_ELEVATOR  1
#define PRINTF_DEBUG_FLOORAQ   1



#define CONTROLLER          CAN_FLOOR_1_AD

#endif /* BOARD_DEFINES*/