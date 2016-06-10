/*
 * Terminal.h
 *
 * Created: 5/30/2013 4:22:36 PM
 *  Author: mfolz
 */ 


#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include "board_defines.h"

#if LCD_ILI9327
  #include "ILI9327.h"
#endif 


void terminalemulator(void);
void Terminal_Set_Line(char line);
void Terminal_Set_Properties(char line, char size, long int color);
void TerminalChar(char c);
void TerminalChar_LCD(char c);     
void TerminalChar_RS232(char c);        
 
 extern struct Terminal_Program_Values{
	char currentline;
	char currentchar;
	int previousx;
	int previousy;
	char previousline;
	int xResolution;
	int yResolution;  
}TerminalProperties;

#endif /* TERMINAL_H_ */