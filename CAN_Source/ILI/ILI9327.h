/*
 * ILI9327_.h
 *
 * Created: 3/6/2015 4:41:18 PM
 *  Author: mfolz-cc
 */ 

#ifndef ILI9327_H_INCLUDED
#define ILI9327_H_INCLUDED


#include "Macros.h" 
#include "Timer.h" 
#include "LCD_Draw.h"

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


#define ILI9327_DATA_DDR        DDRA
#define ILI9327_DATA_PORT       PORTA

#define ILI9327_CONTROL_DDR               DDRAD 
#define ILI9327_CONTROL_PORT              PTAD
#define ILI9327_CONTROL_DDR_ANALOG        ATDDIEN


#define ILI9327_RD         PTAD_PTAD0  //  0x01  // PORTAD_PTAD0
#define ILI9327_WR         PTAD_PTAD1  //  0x02  // PORTAD0_PTAD1
#define ILI9327_RS         PTAD_PTAD2  //  0x03  // PORTAD0_PTAD2
#define ILI9327_CS         PTAD_PTAD3  //  0x04  // PORTAD0_PTAD3
#define ILI9327_RESET      PTAD_PTAD4  //  0x05  // PORTAD0_PTAD4


#define ILI9327_INIT_DATA()    { ILI9327_DATA_DDR	= 0xFF;  }  // all high nothing special here

 //#define ILI9327_INIT_COM()	{ SETBIT(ILI9327_CONTROL_DDR,(ILI9327_RD | ILI9327_WR | ILI9327_RS | ILI9327_CS | ILI9327_RESET);    \
//                                     ILI9327_CONTROL_DDR_ANALOG &= (~(1 << ILI9327_RESET) | ~(1 << ILI9327_CS) | ~(1 << ILI9327_RS) | ~(1 << ILI9327_WR) | ~(1 << ILI9327_RD) ); }


#define ILI9327_INIT_COM()	{ ILI9327_CONTROL_DDR |= ((PTAD_PTAD0_MASK ) | (PTAD_PTAD1_MASK ) | (PTAD_PTAD2_MASK ) | (PTAD_PTAD3_MASK ) | (PTAD_PTAD4_MASK ) );    \
                                     ILI9327_CONTROL_DDR_ANALOG &=  ~((PTAD_PTAD0_MASK ) | (PTAD_PTAD1_MASK ) | (PTAD_PTAD2_MASK ) | (PTAD_PTAD3_MASK ) | (PTAD_PTAD4_MASK ) ); }
	


#define ILI9327_RESET_LOW()  { CLEARBIT( ILI9327_RESET,1);	} // Reset Macros 
#define ILI9327_RESET_HIGH() { SETBIT( ILI9327_RESET,1);	}

#define ILI9327_CS_LOW()  {  CLEARBIT( ILI9327_CS,1);	} // CS Macros
#define ILI9327_CS_HIGH() {  SETBIT( ILI9327_CS,1);	}

#define ILI9327_RS_LOW()  {  CLEARBIT( ILI9327_RS,1);	} // RS Macros
#define ILI9327_RS_HIGH() {  SETBIT( ILI9327_RS,1);	}

#define ILI9327_WR_LOW()  {  CLEARBIT( ILI9327_WR,1);	} // WR Macros
#define ILI9327_WR_HIGH() {  SETBIT( ILI9327_WR,1);	}

#define ILI9327_RD_LOW()  {  CLEARBIT( ILI9327_RD,1);	} // RD Macros 
#define ILI9327_RD_HIGH() {  SETBIT( ILI9327_RD,1);	}	


#define ILI9327_DELAY  { asm("nop");   }


// DISPLAY CONSTANTS 
#define ILI9327_NOP					                0x00              //Page 48
#define ILI9327_SOFT_RESET			            0x01              //49
#define ILI9327_GET_POWERMODE		            0x0A              //50
#define ILI9327_GET_ADDRESS_MODE	          0x0B              //52
#define ILI9327_GET_PIXEL_FORMAT		        0x0C              //54
#define ILI9327_GET_DISPLAY_MODE		        0x0D              //56 
#define ILI9327_GET_SIGNAL_MODE             0x0E              //58 
#define ILI9327_GET_DIAGNOSTIC_RESULT       0x0F              //59 

#define ILI9327_VCOM_CONTROL		          	0xD1
#define ILI9327_POWER_SETTING		          	0xD0	
#define ILI9327_SET_ADDRESS_MODE          	0x36
#define ILI9327_SET_PIXEL_FORMAT	          0x3A
#define ILI9327_DISPLAY_TIMING_SETTING_PT	  0xC1

#define ILI9327_SET_PAGE_ADDRESS            0x2B               // y
#define ILI9327_SET_COLUMN_ADDRESS          0x2A               // x
#define ILI9327_GAMMA_SETTING		            0xC8
#define ILI9327_FRAME_RATE_CONTROL	        0xC5
#define ILI9327_POWER_SETTING_NORMAL_MODE   0xD2
#define ILI9327_PANEL_DRIVING_SETTING       0xC0 

#define ILI9327_SET_DISPLAY_ON	            0x29
#define ILI9327_WRITE_MEMORY_START	        0x2C 

#define ILI9327_SET_ADDRESS_MODE            0x36

#define ILI9327_X_RESOLUTION                 240
#define ILI9327_Y_RESOLUTION                 400

int ILI9327_WRITE_DATA(unsigned char data);
int ILI9327_WRITE_COMMAND(unsigned char command);
int ILI9327_WRITE_BUS(unsigned char bus_data);
int ILI9327_INITALIZE(void);
int ILI9327_ADDRESS_SET(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

void ILI9327_Fill_Rectangle(unsigned int x, unsigned int y,unsigned  int width, unsigned int height, unsigned long color);
void ILI9327_SendPixel(unsigned int x, unsigned int y, unsigned long color);

#endif /* ILI9327__H_ */

/*
#define ILI9327_INIT_COM()	{ ILI9327_CONTROL_DDR |= (1 << ILI9327_RESET) | (1 << ILI9327_CS) | (1 << ILI9327_RS) | (1 << ILI9327_WR) | (1 << ILI9327_RD);    \
                                     ILI9327_CONTROL_DDR_ANALOG &= (~(1 << ILI9327_RESET) | ~(1 << ILI9327_CS) | ~(1 << ILI9327_RS) | ~(1 << ILI9327_WR) | ~(1 << ILI9327_RD) ); }


// Pin settings macros 
#define ILI9327_RESET_LOW()  { CLEARBIT_SHIFT(ILI9327_CONTROL_PORT, ILI9327_RESET);	} // Reset Macros 
#define ILI9327_RESET_HIGH() { SETBIT_SHIFT(ILI9327_CONTROL_PORT, ILI9327_RESET);	}

#define ILI9327_CS_LOW()  {  CLEARBIT_SHIFT(ILI9327_CONTROL_PORT, ILI9327_CS);	} // CS Macros
#define ILI9327_CS_HIGH() {  SETBIT_SHIFT(ILI9327_CONTROL_PORT, ILI9327_CS);	}

#define ILI9327_RS_LOW()  {  CLEARBIT_SHIFT(ILI9327_CONTROL_PORT, ILI9327_RS);	} // RS Macros
#define ILI9327_RS_HIGH() {  SETBIT_SHIFT(ILI9327_CONTROL_PORT, ILI9327_RS);	}

#define ILI9327_WR_LOW()  {  CLEARBIT_SHIFT(ILI9327_CONTROL_PORT, ILI9327_WR);	} // WR Macros
#define ILI9327_WR_HIGH() {  SETBIT_SHIFT(ILI9327_CONTROL_PORT, ILI9327_WR);	}

#define ILI9327_RD_LOW()  {  CLEARBIT_SHIFT(ILI9327_CONTROL_PORT, ILI9327_RD);	} // RD Macros 
#define ILI9327_RD_HIGH() {  SETBIT_SHIFT(ILI9327_CONTROL_PORT, ILI9327_RD);	}	

                      */
