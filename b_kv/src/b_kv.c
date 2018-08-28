/**
 ****************************************************************************
 * MIT License
 * @file b_kv.c  
 * @version v0.0.1
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
#include "./inc/b_kv.h"
#include "./port/b_kv_port.h"
#include "string.h"

static bKVU32 b_kv_code_table[B_KV_ITEM_MAX_NUMBER];
static bKVU32 b_kv_current_number = 0;

#define bKV_PER_INFO_SIZE      (sizeof(b_kv_info_t))

static bKVU32 _b_kv_create_code(bKVU8 *pstr)
{
    bKVU32 u32_tmp = 0, i = 0, sum = 0;
    bKVU32 len = strlen((const char *)pstr);
    if(pstr == bKVNULL || len == 0)
    {
        return 0;
    }
    for(i = 0;i < len;i++)
    {
        sum += pstr[i] * (i + 1);
    }
    sum = sum / len;
    for(i = 0;i < len;i++)
    {
        u32_tmp += pstr[i] * sum; 
        u32_tmp ^= pstr[i];
    }
    return u32_tmp;
}



static bKVS32 _b_kv_search_code(bKVU32 code)
{
    bKVU32 i;
    if(code == 0)   
    {
        return B_KV_STA_ERR;
    }
    for(i = 0;i < B_KV_ITEM_MAX_NUMBER;i++)
    {
        if(b_kv_code_table[B_KV_ITEM_MAX_NUMBER - i - 1] == code)
        {
            return (B_KV_ITEM_MAX_NUMBER - i - 1);
        }
    }
    return B_KV_STA_NO_ITEM;
}


bKVS32 b_kv_init()
{
    bKVU32 i = 0, addr = B_KV_START_ADDRESS;
    b_kv_info_t b_kv_info;
    memset(b_kv_code_table, 0, sizeof(b_kv_code_table));
    for(i = 0;i < B_KV_ITEM_MAX_NUMBER;i++)
    {
        b_kv_port_read(addr + (i * bKV_PER_INFO_SIZE), (bKVU8 *)&b_kv_info, bKV_PER_INFO_SIZE);
        if(b_kv_info.length == 0x0)
        {
            continue;
        }
        b_kv_code_table[i] = b_kv_info.code;
    }
    b_kv_current_number = 0;
    for(i = 0;i < B_KV_ITEM_MAX_NUMBER;i++)
    {
        if(b_kv_code_table[B_KV_ITEM_MAX_NUMBER - i - 1] != 0)
        {
            break;
        }
    }   
    b_kv_current_number = B_KV_ITEM_MAX_NUMBER - i;
    return B_KV_STA_OK;
}

static bKVS32 _b_kv_get_last_info(b_kv_info_t *pinfo)
{
    if(pinfo == bKVNULL)
    {
        return B_KV_STA_ERR;
    }
    if(b_kv_current_number == 0)
    {
        pinfo->address = B_KV_VALUE_ADDRESS;
        pinfo->length = 0;
        return B_KV_STA_OK;
    }
    return b_kv_port_read(B_KV_START_ADDRESS + ((b_kv_current_number - 1) * bKV_PER_INFO_SIZE), (bKVU8 *)pinfo, bKV_PER_INFO_SIZE);
}


static bKVS32 _b_kv_add_new(bKVU32 code, bKVU8* pbuf, bKVU32 len)
{
    b_kv_info_t b_kv_info, b_kv_last_info;
    if(b_kv_current_number == B_KV_ITEM_MAX_NUMBER)
    {
        return B_KV_STA_FULL;
    }
    _b_kv_get_last_info(&b_kv_last_info);
    b_kv_info.code = code;
    b_kv_info.address = b_kv_last_info.address + b_kv_last_info.length;
    b_kv_info.length = len;
    b_kv_info.reserved = 0xffffffff;
    if((b_kv_info.address + b_kv_info.length) > B_KV_MAX_ADDRESS)
    {
        return B_KV_STA_NO_SIZE;
    }
    if(B_KV_STA_OK == b_kv_port_write(b_kv_info.address, pbuf, len))
    {
        if(B_KV_STA_OK == b_kv_port_write(B_KV_START_ADDRESS + (b_kv_current_number * bKV_PER_INFO_SIZE), (bKVU8*)&b_kv_info, bKV_PER_INFO_SIZE))
        {
            b_kv_code_table[b_kv_current_number] = code;
            b_kv_current_number++;
            return B_KV_STA_OK;
        }
    }
    return B_KV_STA_ERR;
}

bKVS32 b_kv_add_value(bKVU8 *pname, bKVU8* pbuf, bKVU32 len)
{
    bKVU32 code_tmp = 0;
    if(pname == bKVNULL || pbuf == bKVNULL || len == 0)
    {
        return B_KV_STA_ERR;
    }
    code_tmp = _b_kv_create_code(pname);
    if(code_tmp == 0)
    {
        return B_KV_STA_ERR;
    }
    return _b_kv_add_new(code_tmp, pbuf, len);
}


bKVS32 b_kv_get_value(bKVU8 *pname, bKVU8* pbuf, bKVU32* len)
{
    bKVS32 index = -1;
    bKVU32 code_tmp = 0;
    b_kv_info_t b_kv_info;
    if(pname == bKVNULL || pbuf == bKVNULL || len == 0)
    {
        return B_KV_STA_ERR;
    }
    code_tmp = _b_kv_create_code(pname); 
    if(code_tmp == 0)
    {
        return B_KV_STA_ERR;
    }    
    index = _b_kv_search_code(code_tmp);
    if(index >= 0)
    {
        if(B_KV_STA_OK == b_kv_port_read(B_KV_START_ADDRESS + (index * bKV_PER_INFO_SIZE), (bKVU8*)&b_kv_info, bKV_PER_INFO_SIZE))
        {
            if(B_KV_STA_OK == b_kv_port_read(b_kv_info.address, pbuf, b_kv_info.length))
            {
                *len = b_kv_info.length;
                return B_KV_STA_OK;
            }
        }
    }
    return B_KV_STA_NO_ITEM;
}
















