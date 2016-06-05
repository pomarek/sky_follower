#ifndef _COMMON_MACROS_H_
    #define _COMMON_MACROS_H_

#define MHZ(X) ((X) * 1000000)
#define KHZ(X) ((X) * 1000)

#define BIT(X) (1<<(X))

#define BYTE_MASK  0x000000FF
#define WORD_MASK  0x0000FFFF
#define DWORD_MASK 0xFFFFFFFF

//#define SET_BIT(WHERE, WHAT) (WHERE) |= BIT(WHAT)
//#define CLEAR_BIT(WHERE, WHAT) (WHERE) &= ~BIT(WHAT)

#endif //_COMMON_MACROS_H_
