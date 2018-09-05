/**
 ****************************************************************************
 * MIT License
 * @file b_kv.h 
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
#ifndef __B_KV_H__
#define __B_KV_H__



#include "../port/b_kv_config.h"



#define B_KV_INFO_SADDR           0   
#define B_KV_INFO_SIZE            B_KV_MIN_ERASE_UNIT

#define B_KV_INFOBAK_SADDR       (B_KV_INFO_SADDR + B_KV_INFO_SIZE)
#define B_KV_INFOBAK_SIZE         B_KV_INFO_SIZE

#define B_KV_VALUE_SADDR         (B_KV_INFOBAK_SADDR + B_KV_INFOBAK_SIZE)
#define B_KV_VALUE_SIZE           B_KV_MIN_ERASE_UNIT

#define B_KV_VALUEBAK_SADDR      (B_KV_VALUE_SADDR + B_KV_VALUE_SIZE)
#define B_KV_VALUEBAK_SIZE        B_KV_VALUE_SIZE

#define B_KV_MAX_ADDRESS         (B_KV_VALUEBAK_SADDR + B_KV_VALUEBAK_SIZE)
#define B_KV_TOTAL_SIZE          (4 * B_KV_MIN_ERASE_UNIT)

#if (B_KV_TOTAL_SIZE > B_KV_SIZE)
#error "FLASH SIZE ERROR ..."
#endif





#pragma pack(1)


typedef struct
{
    bKVU32  code;
    bKVU32  address;
    bKVU32  length;
    bKVU32  reserved;
}b_kv_info_t;



#pragma pack()





bKVS32 b_kv_init(bKVU32 start_addr);
bKVS32 b_kv_add_value(bKVU8 *pname, bKVU8* pbuf, bKVU32 len);
bKVS32 b_kv_get_value(bKVU8 *pname, bKVU8* pbuf, bKVU32* len);
bKVS32 b_kv_delete_value(bKVU8 *pname);



#endif



