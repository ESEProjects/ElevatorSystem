/*
 * ILI9327_Lib.c
 *
 * Created: 3/6/2015 4:40:57 PM
 *  Author: mfolz-cc
 */ 
 
#include "ILI9327.h"


int ILI9327_WRITE_DATA(unsigned char data){
	// Set RS High 
	ILI9327_RS_HIGH(); 
	ILI9327_WRITE_BUS(data); 
//	ILI9327_DELAY; //	Timer_Delay_1us(1); 
	return 0; 
}

int ILI9327_WRITE_COMMAND(unsigned char command){
	ILI9327_RS_LOW(); 
	ILI9327_WRITE_BUS(command); 
  ILI9327_DELAY; //	Timer_Delay_1us(1); 
	return 0;
}

int ILI9327_WRITE_BUS(unsigned char bus_data){
	ILI9327_DATA_PORT  &= 0xFF; // clear all bits except (1 and 0)
	
	// 1111 1100 clear first 2 bits
	\
	ILI9327_DATA_PORT = (bus_data)&0xFF; // send 

	ILI9327_WR_LOW();
  ILI9327_DELAY; //		Timer_Delay_1us(1); 
	ILI9327_WR_HIGH(); 	
	return 0;
}

int ILI9327_INITALIZE(void){
  LCD_Settings(240,400, 8,0 );
	// Set Ports to be output 
	ILI9327_INIT_COM(); // Set COM pins 
	ILI9327_INIT_DATA(); // Set data pins 2- 7 

	ILI9327_CONTROL_PORT|= (1 << ILI9327_CS) | (1 << ILI9327_RS) | (1 << ILI9327_RESET) | (1 << ILI9327_WR) | (1 << ILI9327_RD);
	//Send the Commands to the display 
	ILI9327_RESET_HIGH();
	Timer_Delay_1ms(5); 
	ILI9327_RESET_LOW();
	Timer_Delay_1ms(15);
	ILI9327_RESET_HIGH();
	Timer_Delay_1ms(15);
	
	/// CS HIGH // WR HIGH // CS LOW
	ILI9327_CS_HIGH();
	ILI9327_WR_HIGH(); 
	ILI9327_CS_LOW();

	
	
	ILI9327_WRITE_COMMAND(0xE9); // not sure
	ILI9327_WRITE_DATA(0x20); 
	
	ILI9327_WRITE_COMMAND(0x11); 
	Timer_Delay_10ms(52);       // should be 100ms 
	
	
	ILI9327_WRITE_COMMAND(0xD1);
	ILI9327_WRITE_DATA(0x00);
	ILI9327_WRITE_DATA(0x71);
	ILI9327_WRITE_DATA(0x19);

	ILI9327_WRITE_COMMAND(0xD0);
	ILI9327_WRITE_DATA(0x07);
	ILI9327_WRITE_DATA(0x01);
	ILI9327_WRITE_DATA(0x08);

	ILI9327_WRITE_COMMAND(0x36); 
	ILI9327_WRITE_DATA(0x48);

	ILI9327_WRITE_COMMAND(ILI9327_SET_PIXEL_FORMAT); // set pixle format 
	ILI9327_WRITE_DATA(0x55); /// 

	ILI9327_WRITE_COMMAND(0xC1);
	ILI9327_WRITE_DATA(0x10);
	ILI9327_WRITE_DATA(0x10);
	ILI9327_WRITE_DATA(0x02);
	ILI9327_WRITE_DATA(0x02);

	ILI9327_WRITE_COMMAND(0xC0); //Set Default Gamma
	ILI9327_WRITE_DATA(0x00);
	ILI9327_WRITE_DATA(0x35);
	ILI9327_WRITE_DATA(0x00);
	ILI9327_WRITE_DATA(0x00);
	ILI9327_WRITE_DATA(0x01);
	ILI9327_WRITE_DATA(0x02);

	ILI9327_WRITE_COMMAND(0xC5); //Set frame rate
	ILI9327_WRITE_DATA(0x04);

	ILI9327_WRITE_COMMAND(0xD2); //power setting
	ILI9327_WRITE_DATA(0x01);
	ILI9327_WRITE_DATA(0x44);

	ILI9327_WRITE_COMMAND(0xC8); //Set Gamma
	ILI9327_WRITE_DATA(0x04);
	ILI9327_WRITE_DATA(0x67);
	ILI9327_WRITE_DATA(0x35);
	ILI9327_WRITE_DATA(0x04);
	ILI9327_WRITE_DATA(0x08);
	ILI9327_WRITE_DATA(0x06);
	ILI9327_WRITE_DATA(0x24);
	ILI9327_WRITE_DATA(0x01);
	ILI9327_WRITE_DATA(0x37);
	ILI9327_WRITE_DATA(0x40);
	ILI9327_WRITE_DATA(0x03);
	ILI9327_WRITE_DATA(0x10);
	ILI9327_WRITE_DATA(0x08);
	ILI9327_WRITE_DATA(0x80);
	ILI9327_WRITE_DATA(0x00);
// 240
	ILI9327_WRITE_COMMAND(ILI9327_SET_COLUMN_ADDRESS); 
	ILI9327_WRITE_DATA(0x00);
	ILI9327_WRITE_DATA(0x00);
	ILI9327_WRITE_DATA(0x00);
	ILI9327_WRITE_DATA(0xeF);

// 400
	ILI9327_WRITE_COMMAND(ILI9327_SET_PAGE_ADDRESS); 
	ILI9327_WRITE_DATA(0x00);
	ILI9327_WRITE_DATA(0x00);
	ILI9327_WRITE_DATA(0x01);
	ILI9327_WRITE_DATA(0x3F);
	ILI9327_WRITE_DATA(0x8F); // on internet

	ILI9327_WRITE_COMMAND(0x29); //display on      

	ILI9327_WRITE_COMMAND(0x2C); //display on 

	ILI9327_CS_HIGH();
	
	return 0;
}


int ILI9327_ADDRESS_SET(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2){

//check if greater make equal to max res 
    if( x2 > ILI9327_X_RESOLUTION) 
      x2 = ILI9327_X_RESOLUTION; 
    
    if( y2 > ILI9327_Y_RESOLUTION)
      y2 = ILI9327_Y_RESOLUTION;

//// write X1 (Start) -> X2 (END)
/// if X2 > frame memory the parameter is not updated// 
	ILI9327_WRITE_COMMAND(ILI9327_SET_COLUMN_ADDRESS); 
	ILI9327_WRITE_DATA(x1>>8);
	ILI9327_WRITE_DATA(x1); 
	ILI9327_WRITE_DATA(x2>>8);
	ILI9327_WRITE_DATA(x2); 
	
	ILI9327_WRITE_COMMAND(ILI9327_SET_PAGE_ADDRESS); 
	ILI9327_WRITE_DATA(y1>>8);
	ILI9327_WRITE_DATA(y1); 
	ILI9327_WRITE_DATA(y2>>8);
	ILI9327_WRITE_DATA(y2); 
	
	ILI9327_WRITE_COMMAND(0x2C); 
	return 0; 
}                    



void ILI9327_SendPixel(unsigned int x, unsigned int y, unsigned long color){
  unsigned int j,i; 
  unsigned char color_out =0; 
  

  
  color_out = Color_Convert(color); 


  ILI9327_WRITE_COMMAND(0x2C);
	ILI9327_RS_HIGH();
	ILI9327_CS_LOW(); 
  ILI9327_ADDRESS_SET(x,y, x+1, y+1);
    
 	ILI9327_WRITE_DATA(color_out);
 	ILI9327_WRITE_DATA(color_out);

	
	ILI9327_CS_HIGH(); 

  
  return; 
}




void ILI9327_Fill_Rectangle(unsigned int x, unsigned int y,unsigned  int width, unsigned int height, unsigned long color){
  unsigned int j,i; 
  unsigned int x_loop =0, y_loop =0;  
  unsigned char color_out =0; 

  color_out = Color_Convert(color); 

   x_loop = (width)*2;
   y_loop = (height);

  ILI9327_WRITE_COMMAND(0x2C);
	ILI9327_RS_HIGH();
	ILI9327_CS_LOW(); 
  ILI9327_ADDRESS_SET(y,x, (y+height-1), x+width-1);
    
	for(j =0; j <= x_loop; j++)   // rgb
		for(i= 0; i <= y_loop; i++)
	    	ILI9327_WRITE_DATA(color_out);

	
	ILI9327_CS_HIGH(); 

  
  return; 
}