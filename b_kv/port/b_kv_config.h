/**
 ****************************************************************************
 * MIT License
 * @file b_kv_config.h 
 * @version v0.0.2
 * Copyright (c) [2018-2019] [Bean  email: notrynohigh@outlook.com]
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************
 */
#ifndef __B_KV_CONFIG_H__
#define __B_KV_CONFIG_H__


typedef unsigned char      bKVU8;
typedef unsigned short     bKVU16;
typedef unsigned int       bKVU32;

typedef signed char        bKVS8;
typedef signed short       bKVS16;
typedef signed int         bKVS32;

#define bKVNULL           ((void *)0)


#define B_KV_ITEM_MAX_NUMBER    100                 // <= 256 
#define B_KV_MIN_ERASE_UNIT     0X1000

#define B_KV_START_ADDRESS      0X0000
#define B_KV_SIZE               (4 * B_KV_MIN_ERASE_UNIT)
     


#define B_KV_STA_NO_ITEM         -5
#define B_KV_STA_NO_SIZE         -4
#define B_KV_STA_MEM_ERR         -3
#define B_KV_STA_FULL            -2
#define B_KV_STA_ERR             -1
#define B_KV_STA_OK              0




#endif


