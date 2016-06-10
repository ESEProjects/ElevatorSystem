
#ifndef TOUCH_H_INCLUDED_
#define TOUCH_H_INCLUDED_

#include "ILI9327.h"  
#include "Analog.h"

#define TOUCH_POLL    5
#define TOUCH_TOLLERANCE    3
#define TOUCH_ANALOG_RES    8 
#define TOUCH_X_RES         400
#define TOUCH_Y_RES         240
#define TOUCH_X_MAX         920
#define TOUCH_X_MIN         80
#define TOUCH_Y_MAX         800
#define TOUCH_Y_MIN         160
 

#define YP_R ATDDIEN_IEN3_MASK   // must be an analog pin, use "An" notation!
#define XM_R ATDDIEN_IEN2_MASK  // must be an analog pin, use "An" notation!

 
#define YP PTAD_PTAD1 
#define XM PTAD_PTAD2


#define YM PTAD_PTAD3 
#define XP PTAD_PTAD4 


               

#define TOUCH_DDR               DDRAD 
#define TOUCH_PORT              PTAD
#define TOUCH_DDR_ANALOG        ATDDIEN

#define YP_INPUT()  {  TOUCH_DDR &= ~(YP_R);    \
                                     TOUCH_DDR_ANALOG  |=  (YP_R); }
                                     
#define YP_OUTPUT() {  TOUCH_DDR |= (YP_R);    \
                                     TOUCH_DDR_ANALOG  &=  ~(YP_R); }



#define XM_INPUT()  {  TOUCH_DDR &= ~(XM_R);    \
                                     TOUCH_DDR_ANALOG |=  (XM_R); }
                                     
#define XM_OUTPUT() {  TOUCH_DDR |= (XM);    \
                                     TOUCH_DDR_ANALOG &=  ~(XM); }


/// DDR

//#define YP_INPUT()   {  CLEARBIT(TOUCH_DDR,YP);	}	 
//#define XM_INPUT()   {  CLEARBIT(TOUCH_DDR,XM);	}	  
#define YM_INPUT()   {  CLEARBIT(TOUCH_DDR,YM);	}	
#define XP_INPUT()   {  CLEARBIT(TOUCH_DDR,XP);	}	


//#define YP_OUTPUT()   {  SETBIT(TOUCH_DDR,YP);	}	 
//#define XM_OUTPUT()   {  SETBIT(TOUCH_DDR,XM);	}	  
#define YM_OUTPUT()   {  SETBIT(TOUCH_DDR,YM);	}	
#define XP_OUTPUT()   {  SETBIT(TOUCH_DDR,XP);  }
	 
// PINS 
#define XP_LOW()   {  CLEARBIT(TOUCH_PORT ,XP);	}	  
#define YM_LOW()   {  CLEARBIT(TOUCH_PORT ,YM);	}	 

#define XP_HIGH()   {  SETBIT(TOUCH_PORT ,XP); 	}	 
#define YP_HIGH()   {  SETBIT(TOUCH_PORT ,YM);	}	 


#define YP_LOW() {  CLEARBIT( PTAD_PTAD2,1);	}	   
#define XM_LOW() {  CLEARBIT( PTAD_PTAD1,1);	}	   

   

unsigned int Touch_Screen_Y(void);
unsigned int Touch_Screen_X(void);

unsigned int * Touch_Screen_XY_Res(void); 




extern struct Touch_Screen{
	unsigned int xResolution;
	unsigned int yResolution;
	unsigned char Analog_Resolution;
  unsigned char Touch_Poll;
  unsigned char Touch_Tollerance;
}Touch_Properties;



//#define YM 8   // can be a digital pin    PA0 = 8 
//#define XP 9   // can be a digital pin    PA1 = 9


// macros to setup analog input then set to output 100% necessary 


#endif
