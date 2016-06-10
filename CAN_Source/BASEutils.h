#ifndef _BASEUTILS_H
#define _BASEUTILS_H

//Bit masking macros
#define LOW(x) ((x)&0xFF)

//Byte operation macros
#define SET_BITS(port, pattern) ((port)|=(pattern))
#define CLR_BITS(port, pattern) ((port)&=LOW(~(pattern)))
#define FLIP_BITS(port, pattern) ((port)^=(pattern))
#define FORCE_BITS(reg, mask, value) (reg)=(((reg)&LOW(~(mask)) ) | ((value)&LOW((mask)))) 

//Word operations
#define FORCE_WORD(reg, mask, value) (reg)=(((reg)&(~(mask))) | ((value)&(mask)))

#define PRESCALE_1 (TSCR2_PR_MASK & 0x00)
#define PRESCALE_2 (TSCR2_PR_MASK & 0x01)
#define PRESCALE_4 (TSCR2_PR_MASK & 0x02)
#define PRESCALE_8 (TSCR2_PR_MASK & 0x03)
#define PRESCALE_16 (TSCR2_PR_MASK & 0x04)
#define PRESCALE_32 (TSCR2_PR_MASK & 0x05)
#define PRESCALE_64 (TSCR2_PR_MASK & 0x06)
#define PRESCALE_128 (TSCR2_PR_MASK & 0x07)

#define MS_DELAY_1 8000
#define MS_DELAY_2 4000
#define MS_DELAY_4 2000
#define MS_DELAY_8 1000
#define MS_DELAY_16 500
#define MS_DELAY_32 250
#define MS_DELAY_64 125
#define MS_DELAY_128 62

#define ENABLE_ANALOG   DDRB=(DDRB&(~(0x20)))|(DDRB_BIT5_MASK);\
                        PORTB=(PORTB&(~(0x20)))|(PORTB_BIT5_MASK)

#define DISABLE_ANALOG  DDRB=(DDRB&(~(0x20)))|(DDRB_BIT5_MASK);\
                        PORTB=(PORTB&(~(0x20)))|(LOW(~PORTB_BIT5_MASK))  


#define TOKEN_CAT2(x2, y2) x2 ## y2
#define TOKEN_CAT(x, y) TOKEN_CAT2(x, y)

#endif _BASEUTILS_H