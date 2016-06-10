                                         /*
 * Terminal.c
 *
 * Created: 5/30/2013 4:22:18 PM
 *  Author: mfolz
 

 Version 1.0
 ---------------
	- Beta build, kinda crappy but still functional 
1.2
	- Works better with the new rotation, made it Dependant on the devices current orientation. 
	- need to add functionality with float variables 
*/ 
#include "Terminal.h"




/* 
add scroll bar and add function to log data store it and reprint it  when you have access to the SD card 

Use terminal emulator first then use the terminal set properties, then printf is good for several lines
 roughly 20 lines of text. I don t recommend using any size above 1, it looks really ugly. I mean really REALLY UGLY.
 Due to the possible limitations of space for font (2-3kb of pgm) i didn't bother adding a larger font, hence anything 
 greater 1 will look terrible. Every pixel will be a 2x2 box or higher depending on the font. 
 

throw this in your main code if you want to use your display to output printf 

	FILE lcd_str = FDEV_SETUP_STREAM(TerminalChar, NULL, _FDEV_SETUP_RW);
	stdout = &lcd_str;

*/  /// TERMINATL CHAR GARBAGE
void terminalemulator(void){
	struct Terminal_Program_Values	*pointTerminal = &TerminalProperties;
  #if LCD_ILI9327_PRINTF
    struct LCD_Geometry *pointCurrent= &Current;
    struct LCD_Properties *pointProperties= &Properties;
  #endif 
	
	pointTerminal->xResolution = 240;
	pointTerminal->yResolution = 400; 

	pointTerminal->currentline = 3;
	pointTerminal->previousx= 0;
	pointTerminal->previousy= 0;
		
    #if LCD_ILI9327_PRINTF
     	LCD_Fill_Rectangle(0,0,pointTerminal->xResolution,pointTerminal->yResolution, 0x000000);
		  LCD_Write_Line("Microcrap Winderp [version 1.2]",10, (pointTerminal->xResolution - 15),1,0xFFFFF);
     	LCD_Write_Line("Command Line Debugging System, use with printf",10, (pointTerminal->xResolution - 25),1,0xFFFFF);
	    Terminal_Set_Properties(2, 1, 0xFFFFFF); //set default properties /
    #endif 
}

void Terminal_Set_Line(char line){
	struct Terminal_Program_Values	*pointTerminal = &TerminalProperties;
	pointTerminal->currentline = line+3;	
}

void Terminal_Set_Properties(char line, char size, long int color){
	struct Terminal_Program_Values	*pointTerminal = &TerminalProperties;
  #if LCD_ILI9327_PRINTF
    struct LCD_Geometry *pointCurrent= &Current;
    struct LCD_Properties *pointProperties= &Properties;
  	pointCurrent->color = color;
   	pointCurrent->size = size; 
  #endif 
	
	
	
	pointTerminal->currentline = line+2;
}

void TerminalChar(char c){
	struct Terminal_Program_Values	*pointTerminal = &TerminalProperties;

 #if LCD_ILI9327_PRINTF
    TerminalChar_LCD(c); 
 #endif
  
 #if RS232_PRINTF
    TerminalChar_RS232(c); 
 #endif

}

#if RS232_PRINTF
void TerminalChar_RS232(char c){
  	struct Terminal_Program_Values	*pointTerminal = &TerminalProperties;   
  if (pointTerminal->currentchar > 80){ 
  	  	RS232_Write(c);
  	  	RS232_Write(0x0A);
  	  	RS232_Write(0x0d);
  	    c ='>';  
  	    pointTerminal->currentchar =0;
  	}

  if (c == '\n'){
    	  RS232_Write(0x0A);
  	  	RS232_Write(0x0d);
  	    c ='>';  
  	    pointTerminal->currentchar =0;
  }

  	RS232_Write(c);
  	pointTerminal->currentchar+= 1;

}
#endif 


 /// END OF TERMINAL CHAR 
#if LCD_ILI9327
void TerminalChar_LCD(char c){

  struct Terminal_Program_Values	*pointTerminal = &TerminalProperties;
	struct LCD_Geometry *pointCurrent= &Current;
	struct LCD_Properties *pointProperties= &Properties;
	
	//backspace detected --- needs to clear char  
	if (c == 0x08) {
	 	pointTerminal->previousx -= (6* pointCurrent->size)*2;
	  LCD_Fill_Rectangle((pointCurrent->xPosition),(pointCurrent->yPosition+1),8,5,0x000000);
	  c =0x20; 
	  
	}
	
	if (c == 0x0D || c == 0x0A) {
		 pointTerminal->currentline += 1;  
		 pointTerminal->previousx = 5;  
		 c = 0x3E;
	}
	
	else if (pointTerminal->previousx  > (pointTerminal->yResolution -(6*pointCurrent->size)) ){
		pointTerminal->currentline += 1;
		pointTerminal->previousx = 15;
	}
	
	else if( pointTerminal->currentline*(10*pointCurrent->size) > pointTerminal->xResolution){
	//	_delay_ms(100);
	 // 	Timer_Delay_10ms(400);
		LCD_Fill_Rectangle(0,0,pointTerminal->xResolution,pointTerminal->yResolution, 0x000000);
		LCD_Write_Line("Microcrap Winderp [version 1.2]",10, (pointTerminal->xResolution - 15),1,0xFFFFF);
  	LCD_Write_Line("Command Line Debugging System, use with printf",10, (pointTerminal->xResolution - 25),1,0xFFFFF);
		Terminal_Set_Properties(2, 1, 0xFFFFFF);
	} 
	
	pointCurrent->xPosition = pointTerminal->previousx;
	pointCurrent->yPosition = pointTerminal->xResolution - pointTerminal->currentline*(10*pointCurrent->size);
	LCD_Write_Char(c);
	pointTerminal->previousx += (6* pointCurrent->size);
}
#endif 
