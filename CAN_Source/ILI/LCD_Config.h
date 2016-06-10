/*
 * LCD_Config.h
 *
 * Created: 7/11/2013 1:10:10 PM
 *  Author: mfolz
 */ 


#ifndef LCD_CONFIG_H_INCLUDED__
#define LCD_CONFIG_H_INCLUDED__


/* 0 Disable  1 Enable // Allows for the use of the analog touch display */
#define LCD_ANALOG_TOUCH 1  
/* All libraries containe ind Touch.h    */

/*0 Disable  1 Enable //Controls the basic string libraries */ 
#define LCD_STRINGS 1
 /* LCD_Write_Char, LCD_Write_Line  */

 /* 0 Disable  1 Enable // Controls  Terminal and Char Libraries -- Must Be enabled with LCD STRINGS*/ 
#define LCD_PRINTF 1
 /* Enables the Terminal.c libraries -- ALL */


 /*0 Disable  1 Enable // Allows for the use of draw circle, line and extras */ 
#define LCD_DRAW   0  
 /* LCD_Draw_Shape, LCD_Draw_EllipseRect, LCD_Draw_FillCircle ,LCD_plotEllipseRect ,LCD_Draw_FastSprite  */


 /*0 Disable 1 Enable // Allows for the use of Arial fonts when using printf */
#define LCD_STYLE_FONT 0 
/* LCD_Write_Style_Char, LCD_Write_Style_Line  */

/*
LCD Driver:
---------------------------------------
	-Set the connected LCD to 1 leave all others 0 
*/ 
#define ST7735R 0
#define ST7735	0
#define SSD1289 0 
#define ILI9481 0 
#define ILI9327 1

/*
SSD1289:               - Set Variables 
----------------------------------------------
	-Can be programmed using either SPI or Parallel communications
	-Parallel is highly recommended as with SPI it can take almost 1 second to wipe
	a screen.
*/


#define SSD1289_SPI 0   // not written or enabled ... yet
#define SSD1289_Parallel 1

/*
Set Rotation:
---------------------------------------
	-Set the desired layout, landscape or portrait. 
	-Left or Right 
*/
#define Landscape 0 
#define Portrait 0 
#define Display_Left 0 
#define Display_Right 0 




#if SSD1289
	#define MAX_X_RES 240
	#define MAX_Y_RES 320
//	#include "_SSD1289.h"
#endif

#if ST7735
	#define MAX_X_RES 120
	#define MAX_Y_RES 160
//	#include "ST7735_SPI.h"
#endif	
	
#if ILI9481
	#define MAX_X_RES 320
	#define MAX_Y_RES 480
// 	#include "ILI9481.h"	
#endif






//#define 


#endif /* LCD_CONFIG_H_ */