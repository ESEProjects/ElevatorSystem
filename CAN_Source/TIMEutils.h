#ifndef _TIMEUTILS_H
#define _TIMEUTILS_H

#include "BASEutils.h"

/************************* RTI Module Operations *****************************/

#define RTI_MS_DELTA 0x23
#define SET_RTI_INTERVAL(value)           FORCE_BITS(RTICTL,0x7F,(value))
#define CLR_RTI_FLAG                      CRGFLG=CRGFLG_RTIF_MASK
#define ENABLE_RTI                        SET_BITS(CRGINT, CRGINT_RTIE_MASK)
#define DISABLE_RTI                       CLR_BITS(CRGINT, CRGINT_RTIE_MASK)


/************************* Timer Module Operations ***************************/
// TSCR2 (Timer System Control Register 2) Macro Operations
#define SET_TIMER_PRESCALE(scale)         FORCE_BITS(TSCR2,(TSCR2_PR0_MASK|TSCR2_PR1_MASK|TSCR2_PR2_MASK),scale)
          
#define ENABLE_TO_INTERRUPT               SET_BITS(TSCR2,0x80)
#define DISABLE_TO_INTERRUPT              CLR_BITS(TSCR2,0x80)

#define ENABLE_CNTR_FREERUN               CLR_BITS(TSCR2,0x08)
#define ENABLE_CNTR_RESET                 SET_BITS(TSCR2,0x08)

// TSCR1 (Timer System Control Register 1) Macro Operation
#define ENABLE_TIMER                      SET_BITS(TSCR1,0x80)
#define DISABLE_TIMER                     CLR_BITS(TSCR1,0x80)

#define ENABLE_TIMER_WAIT                 SET_BITS(TSCR1,0x40)
#define DISABLE_TIMER_WAIT                CLR_BITS(TSCR1,0x40)

#define ENABLE_TSFRZ                      SET_BITS(TSCR1,0x20)
#define DISABLE_TSFRZ                     CLR_BITS(TSCR1,0x20)

#define ENABLE_FAST_CLEAR                 SET_BITS(TSCR1,0x10)
#define DISABLE_FAST_CLEAR                CLR_BITS(TSCR1,0x10)

//TIOS (Timer Input Capture/Output Compare Slelct Register) Macro Operations
#define MAKE_CHNL_OC(chnl)                SET_BITS(TIOS, 1<<(chnl))
#define MAKE_CHNL_IC(chnl)                CLR_BITS(TIOS, 1<<(chnl))

//TIE (Timer Interrupt Enable Register)
#define ENABLE_CHNL_INTERRUPT(chnl)       SET_BITS(TIE, 1<<(chnl))
#define DISABLE_CHNL_INTERRUPT(chnl)      CLR_BITS(TIE, 1<<(chnl))

//TFLG1 (Timer Interrupt Flag 1 Register)
#define CLR_TIMER_CHNL_FLAG(chnl)         TFLG1=(1<<(chnl))

//TCTL1 + TCTL2 (Timer Control Registers 1 and 2)
#define TCTL_1_2                          (*(volatile word * const)&TCTL1)
#define OC_OFF                            (0x00)
#define OC_TOGGLE                         (0x01)
#define OC_GO_LO                          (0x02)
#define OC_GO_HI                          (0x03)
#define SET_OC_ACTION(chnl, action)       FORCE_WORD( TCTL_1_2,0x03<<((chnl)*2),(action)<<((chnl)*2) )

//TCTL3 + TCTL4 (Timer Control Registers 3 and 4)
#define TCTL_3_4                          (*(volatile word * const)&TCTL3)
#define NO_ACTION                         (0x00)
#define EDGE_RISING                       (0x01)
#define EDGE_FALLING                      (0x02)
#define EDGE_EITHER                       (0x03)
#define SET_IC_EDGE(chnl, action)         FORCE_WORD( TCTL_3_4,0x03<<((chnl)*2),(action)<<((chnl)*2) )

//CFORC 
#define FORCE_OC_ACTION_NOW(chnl,action)  SET_OC_ACTION((chnl),(action));\
                                          SET_BITS(CFORC, 1<<(chnl))
          
/************************* Function Prototypes *******************************/          
void msDelay(unsigned int delay);
void Heartbeat_Init(void);



#endif _TIMEUTILS_H