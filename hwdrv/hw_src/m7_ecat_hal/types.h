/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    types.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2013.09.10

Description:    data type definition

Others:


History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#ifndef TYPES_H
#define TYPES_H
#ifndef __ASSEMBLY__
//#undef false
//#undef true
//typedef enum{false = 0, true} bool;
typedef signed char            s8;
typedef unsigned char        u8;
typedef u8                    __u8;

typedef signed short        s16;
typedef unsigned short        u16;
typedef u16                    __u16;



typedef signed int            s32;
typedef unsigned int        u32;
typedef u32                    __u32;

typedef signed long long    s64;
typedef unsigned long long    u64;

typedef    u8                    uint8_t;
typedef    u16                    uint16_t;
typedef    u32                    uint32_t;
typedef    u64                    uint64_t;
typedef    s8                    int8_t;
typedef    s16                    int16_t;
typedef    s32                    int32_t;
typedef    s64                    int64_t;

typedef unsigned char       BYTE;
typedef unsigned short int    WORD;
typedef long                LONG;
typedef unsigned long       DWORD;
typedef unsigned char       BOOL;

#ifndef     TRUE
#define         TRUE      1
#endif                                                                  /*  TRUE                        */

#ifndef     FALSE
#define     FALSE     0
#endif                   

#define     __IO    volatile                  /*!< defines 'read / write' permissions   */

typedef unsigned char        unchar;
typedef unsigned short        ushort;
typedef unsigned int        uint;
typedef unsigned long        ulong;
typedef __u32                dev_t;

#ifndef NULL
#define NULL                0
#endif
typedef int                    ssize_t;

typedef unsigned long        cycles_t;

typedef unsigned int        size_t;

typedef int                    ptrdiff_t;
typedef unsigned long        phys_size_t;

typedef long                off_t;

typedef volatile unsigned long    vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char    vu_char;

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

//typedef __u32 __bitwise __wsum
typedef u32 __wsum;
#define INT_MAX        ((int)(~0U>>1))
#define INT_MIN        (-INT_MAX - 1)
#endif //end of ifndef __ASSEMBLY__

#endif

