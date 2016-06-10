#ifndef _LCDUTILS_H
#define _LCDUTILS_H

#include "BASEutils.h"

/******************************** LCD Macros *********************************/
//LCD port registers
#define LCD_PORT                      PORTA
#define LCD_PORT_DDR                  DDRA

#define MAX_LCD_SIZE                  (30)
#define LO_NYBBLE(value)              ((value)&0x0F)
#define HI_NYBBLE(value)              (((value)>>4)&0x0F)

//LCD pin masks
#define LCD_E_MASK                    PORTA_BIT4_MASK
#define LCD_RS_MASK                   PORTA_BIT5_MASK
#define LCD_RW_MASK                   PORTA_BIT6_MASK
#define LCD_BUS_MASK                  (PORTA_BIT0_MASK|PORTA_BIT1_MASK|PORTA_BIT2_MASK|PORTA_BIT3_MASK)
#define LCD_ENABLE_BACK_LIGHT         SET_BITS(PORTA, PORTA_BIT7_MASK)
#define LCD_DISABLE_BACK_LIGHT        CLR_BITS(PORTA, PORTA_BIT7_MASK)
#define LCD_NEW_LINE_MASK             (0x40)     

//LCD enable line actions
#define LCD_E_HI                      SET_BITS(LCD_PORT, LCD_E_MASK)
#define LCD_E_LO                      CLR_BITS(LCD_PORT, LCD_E_MASK)

#define LCD_RS_DR                     SET_BITS(LCD_PORT, LCD_RS_MASK)
#define LCD_RS_IR                     CLR_BITS(LCD_PORT, LCD_RS_MASK)
#define LCD_RW_WRITE                  CLR_BITS(LCD_PORT, LCD_RW_MASK)
#define LCD_RW_READ                   SET_BITS(LCD_PORT, LCD_RW_MASK)

//LCD bus value
#define LCD_BUS(value)                FORCE_BITS(LCD_PORT,LCD_BUS_MASK,(value))
          
//LCD command select values
#define LCD_CMD_CLR_DISPLAY           (0x01)
#define LCD_CMD_RTN_HOME              (0x02)
#define LCD_CMD_ENTRY                 (0x04)
#define LCD_CMD_DISPLAY               (0x08)
#define LCD_CMD_CURSOR                (0x10)
#define LCD_CMD_FUNCTION              (0x20)
#define LCD_CMD_SET_CGRAM             (0x40)
#define LCD_CMD_SET_DDRAM             (0x80)
          
//LCD entery mode command actions
#define LCD_ENTRY_MOVE_CURSOR         (0x01)
#define LCD_ENTTY_LEAVE_CURSOR        (0x00)
#define LCD_ENTRY_INC                 (0x02)
#define LCD_ENTRY_DEC                 (0x00)

//LCD display control command actions
#define LCD_DISPLAY_ON                (0x04)
#define LCD_DISPLAY_OFF               (0x00)
#define LCD_DISPLAY_CURSOR_ON         (0x02)
#define LCD_DISPLAY_CURSOR_OFF        (0x00)
#define LCD_DISPLAY_CURSOR_BLINK      (0x01)
#define LCD_DISPLAY_CURSOR_NO_BLINK   (0x00)

//LCD cursor control command actions
#define LCD_CURSOR_SHIFT              (0x00)
#define LCD_DISPLAY_SHIFT             (0x08)
#define LCD_SHIFT_RIGHT               (0x04)
#define LCD_SHIFT_LEFT                (0x00)

//LCD function control command actions
#define LCD_FUNCTION_4BIT             (0x00)
#define LCD_FUNCTION_8BIT             (0x10)
#define LCD_FUNCTION_1LINES           (0x00)
#define LCD_FUNCTION_2LINES           (0x08)
#define LCD_FUNCTION_5X8_FONT         (0x00)
#define LCD_FUNCTION_5X10_FONT        (0x04)


/******************************** LCD Macros *********************************/
void LCDinit(void);
void LCDputc(unsigned char cx);
void LCDputs(char *buffer);
void LCDclear(void);
void LCDprintf(const char *format, ...);

void Floor_Elevator_LCD(unsigned char current_floor);



#endif _LCDUTILS_H