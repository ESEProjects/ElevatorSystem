/******************************************************************************
Author:             Joshua Findlay
Date:               April 7, 2015
Description:        This program aims to create a front end for the LCD. It has
                    several internal functions that perform the timing and 
                    sequencing responsible for the actual write command to 
                    be detected on the LCD module and the program has higher
                    level function visible to the user to pass formatted data
                    into for printing to the LCD.
Version:            1
******************************************************************************/
#include <hidef.h>      /* common defines and macros */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "BASEutils.h"
#include "TIMEutils.h"
#include "LCD.h"


static void LCDdata(unsigned char byte);
static void LCDcmd(unsigned char byte);

/******************************************************************************
Name:               LCDinit()
Arguments:          void - uses hard coded values to set defaults.
Returns:            void - doing hardware stuff so no error codes.
Description:        This function starts by pulsing a specific syncronization
                    pattern to enable the LCD for 4-bit operation. It then sets
                    the default operation of the LCD.
******************************************************************************/
void LCDinit(void){	
          LCD_PORT=0x00;
          LCD_PORT_DDR=0xFF;
          
          

          //Syncronization sequence for 4-bit operation
          LCD_BUS(0x03);
          LCD_E_HI;
          LCD_E_LO;
          msDelay(5);
          
          LCD_BUS(0x03);
          LCD_E_HI;
          LCD_E_LO;
          msDelay(1);
          
          LCD_BUS(0x03);
          LCD_E_HI;
          LCD_E_LO;
          
          LCD_BUS(0x02);
          LCD_E_HI;
          LCD_E_LO;
          msDelay(2);
          
          //Configure the display to default settings for this board
          LCDcmd(LCD_CMD_FUNCTION | LCD_FUNCTION_4BIT | LCD_FUNCTION_2LINES | LCD_FUNCTION_5X8_FONT);
          LCDcmd(LCD_CMD_DISPLAY | LCD_DISPLAY_OFF);
          LCDclear();
          LCDcmd(LCD_CMD_ENTRY | LCD_ENTTY_LEAVE_CURSOR | LCD_ENTRY_INC);
          LCDcmd(LCD_CMD_DISPLAY | LCD_DISPLAY_ON | LCD_DISPLAY_CURSOR_OFF | LCD_DISPLAY_CURSOR_NO_BLINK);
          return;
}


/******************************************************************************
Name:               LCDdata()
Arguments:          unsigned char byte - the byte to be written to the LCD.
Returns:            void
Description:        This function will blind wait for 2 ms before setting the
                    LCD control lines for a write to the DR then perform the 
                    write sequence to the LCD using the specified byte.
******************************************************************************/
static void LCDdata(unsigned char byte) {
          msDelay(2);
          LCD_E_LO;
          LCD_RS_DR;
          LCD_RW_WRITE;
          
          LCD_BUS(HI_NYBBLE(byte));
          LCD_E_HI;
          LCD_E_LO;
          
          LCD_BUS(LO_NYBBLE(byte));
          LCD_E_HI;
          LCD_E_LO;
          return;
}


/******************************************************************************
Name:               LCDcmd()
Arguments:          unsigned char byte - the command to be written to the LCD.
Returns:            void
Description:        This function waits for 2 ms like LCDdata then sets the
                    control lines for a write to the IR of the LCD. The
                    function carries out the write sequence for the command.
******************************************************************************/
static void LCDcmd(unsigned char byte){
          msDelay(2);
          LCD_E_LO;
          LCD_RS_IR;                    
          LCD_RW_WRITE;
          
          LCD_BUS(HI_NYBBLE(byte));
          LCD_E_HI;
          LCD_E_LO;
          
          LCD_BUS(LO_NYBBLE(byte));
          LCD_E_HI;
          LCD_E_LO;
          return;
}

/******************************************************************************
Name:               LCDputc()
Arguments:          unsigned char cx - the charcter to be written to LCD.
Returns:            void
Description:        This function calles the internal LCDdata function to write
                    a character to the LCD but, it also handles the cases of 
                    carriage returns and line feeds.
******************************************************************************/
void LCDputc(unsigned char cx)
{
          static unsigned char ddRAM_address=0x00;
          
          if(cx == 0x0A){
                    FLIP_BITS(ddRAM_address, LCD_NEW_LINE_MASK);
                    LCDcmd(LCD_CMD_SET_DDRAM|ddRAM_address);   
          } 
          else if(cx == 0x0D){
                    CLR_BITS(ddRAM_address, LCD_NEW_LINE_MASK);
                    LCDcmd(LCD_CMD_SET_DDRAM|ddRAM_address);
          } 
          else          
                    LCDdata(cx);
          return;
}

/******************************************************************************
Name:               LCDputs()
Arguments:          char *buffer - the buffer to print to the LCD.
Returns:            void
Description:        This function calls LCDputc in a loop for the maximum
                    number of LCD spaces. If a null terminator is seen before
                    the maximum number of LCD spaces writing stops.
******************************************************************************/
void LCDputs(char *buffer) {
          int i;
          for(i=0;i<MAX_LCD_SIZE;i++) {
                    if(buffer[i]==0x00)
                              break;
                    LCDputc(buffer[i]);
          }
          return;
}

/******************************************************************************
Name:               LCDprintf()
Arguments:          const char *format - the format the buffer is to take on.
Returns:            ... - any number of arguments to become formated.
Description:        This function will take a format and an argument list. The
                    vsprintf() function will then format the arguments acording
                    to the format string and put it into buffer to be printed
                    to the LCD using the LCDputs function.
******************************************************************************/
void LCDprintf(const char *format, ...) {

          va_list myArgs;
          char buffer[MAX_LCD_SIZE]={'0'};
          
          va_start(myArgs, format);
          (void)vsprintf(buffer, format, myArgs);
          va_end(myArgs);
          
          buffer[MAX_LCD_SIZE-1]=0;
          LCDputs(buffer);
          return;
}


void LCDclear(void){
          LCDcmd(LCD_CMD_CLR_DISPLAY);
          LCDcmd(LCD_CMD_RTN_HOME);
          msDelay(10);
}



