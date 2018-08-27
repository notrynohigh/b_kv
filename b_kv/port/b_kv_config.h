#ifndef __B_KV_CONFIG_H__
#define __B_KV_CONFIG_H__


typedef unsigned char      bKVU8;
typedef unsigned short     bKVU16;
typedef unsigned int       bKVU32;

typedef signed char        bKVS8;
typedef signed short       bKVS16;
typedef signed int         bKVS32;

#define b_KV_NULL          ((void *)0)


#define B_KV_ITEM_MAX_NUMBER    100                 // <= 256 
#define B_KV_MIN_ERASE_UNIT     0X1000


#define B_KV_START_ADDRESS      0X0000
#define B_KV_SIZE               (3 * B_KV_MIN_ERASE_UNIT)		















#endif


