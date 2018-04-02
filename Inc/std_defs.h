
/***********************************************************************
 *
 *  std_defs.h
 *
 *    public definitions of pseudonyms for unsigned integer types,
 *    fixed-size integer types, and other commonly needed definitions
 *
 **********************************************************************/

/*
 *  Usage:
 *    #include "std_defs.h"
 *
 *  std_defs.h and compiler.h should be in project sub-directory
 */

#ifndef STD_DEFS_H
#define STD_DEFS_H

//#include "compiler.h"

/* pseudonyms for existing unsigned integer types */

//typedef unsigned char  uchar;
//typedef unsigned short ushort;
//typedef unsigned int   uint;
//typedef unsigned long  ulong;

//typedef u8 byte;

/*
 * The C99 standard defines standard integer data types as follows:
 *
 *             signed     unsigned
 *   8 bit:    int8_t      uint8_t
 *  16 bit:   int16_t     uint16_t
 *  32 bit:   int32_t     uint32_t
 *  64 bit:   int64_t     uint64_t
 */

/* other definitions */

#define U8_MAX    0xFFU
#define U16_MAX   0xFFFFU
#define U32_MAX   0xFFFFFFFFUL
//#define U64_MAX   0xFFFFFFFFFFFFFFFF /* no UL suffix? */

#define S8_MAX    127
#define S16_MAX   32767
#define S32_MAX   2147483647
//#define S64_MAX   9223372036854775807 /* any suffix? */

#define S8_MIN    (-S8_MAX)
#define S16_MIN   (-S16_MAX)
#define S32_MIN   (-S32_MAX)
//#define S64_MIN   (-S64_MAX)

#define NIL ((void *)0) /* pointer constant */
#define NUL ('\0')      /* ASCII constant */


#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define ARRAY_COUNT(array) (sizeof (array) / sizeof ((array)[0]))

#define ABS(x)        ((0 <= (x)) ? (x) : -(x))
#define ABSDIFF(x,y)  (((x) <= (y)) ? ((y) - (x)) : ((x) - (y)))

#define SIGN(x)       (((x) == 0) ? (0) : ((0 < (x)) ? (1) : (-1)))

//#define MAX(x,y)      (((x) < (y)) ? (y) : (x))
//#define MIN(x,y)      (((x) < (y)) ? (x) : (y))
#define LIMIT(x,a,b)  (((x) < (a)) ? (a) : (((b) < (x)) ? (b) : (x)))

#define FP_ZERO(x,e)  (((x) < (e)) ? TRUE : FALSE)  /* e > 0! */

#define MK_FP(seg,off)  ((void *)(((u16)(seg) << 16) | (u16)(off)))

/* these may change var */
#define SET_BITS(var,mask)  ((var) |=  (mask))
#define CLR_BITS(var,mask)  ((var) &= ~(mask))
#define TGL_BITS(var,mask)  ((var) ^=  (mask))

/* these do not change var */
#define TST_BITS(var,mask)  (((var) & (mask)) != 0)
#define HAS_BITS(var,mask)  (((var) & (mask)) == (mask))

#define HAS_ONE_1(val)  (!((val) & ((val) - 1)))

// does not address different languages...
// or, Unicode
#define TRUE_OR_FALSE_STR(var) ((var) ? "True" : "False")
#define T_OR_F_STR(var)        ((var) ? "T" : "F")

#ifndef __cplusplus

//enum bool
//{
//  false, true
//};
//typedef enum bool bool;

#define false 0
#define true  1
#endif /* __cplusplus */

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE  1
#endif


#endif /* STD_DEFS_H */

